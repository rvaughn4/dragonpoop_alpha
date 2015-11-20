
#include "model_man.h"
#include "model_man_ref.h"
#include "model_man_readlock.h"
#include "model_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "model_man_task.h"
#include "../gfx_ref.h"
#include "../gfx.h"
#include "../gfx_writelock.h"
#include "../gfx_readlock.h"
#include "model.h"
#include "model_ref.h"
#include "model_readlock.h"
#include "model_writelock.h"
#include "model_loader/model_loader_man.h"
#include "model_loader/model_loader_man_writelock.h"
#include "../../core/dptaskpool/dptaskpool_ref.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "../../core/dptaskpool/dptaskpool_readlock.h"
#include "model_instance/model_instance_ref.h"
#include "model_instance/model_instance_writelock.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{

    //ctor
    model_man::model_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;

        this->c = c;
        this->model_cnt = 0;
        this->ms_each_frame = 10;

        this->tpr = (dptaskpool_ref *)tp->getRef();

        gl = (gfx_writelock *)o.writeLock( g, "model_man::model_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();

        this->_startTask( tp, 10 );

        this->loader_mgr = new model_loader_man( c, g, tp );
    }

    //dtor
    model_man::~model_man( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "model_man::~model_man" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_man::~model_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "model_man::~model_man" );
        this->_deleteTask();
        this->deleteModels();
        delete this->loader_mgr;
        delete this->g;
        delete this->tpr;
        o.unlock();
    }

    //return core
    core *model_man::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *model_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_man_readlock( (model_man *)p, l );
    }

    //generate write lock
    shared_obj_writelock *model_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_man_writelock( (model_man *)p, l );
    }

    //generate ref
    shared_obj_ref *model_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_man_ref( (model_man *)p, k );
    }

    //start task
    void model_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new model_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "model_man" );
        tp->addTask( this->tsk );
    }

    //kill task
    void model_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk, "model_man::_killTask" );
        tl->kill();
        o.unlock();
    }

    //delete task
    void model_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }

    //run
    void model_man::run( dpthread_lock *thd, model_man_writelock *g )
    {
        this->runModels( thd );
    }

    //delete all loaders
    void model_man::deleteModels( void )
    {
        std::list<model *> *l, d;
        std::list<model *>::iterator i;
        model *p;

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }

    //run all loaders
    void model_man::runModels( dpthread_lock *thd )
    {
        std::list<model *> *l, ll, d;
        std::list<model *>::iterator i;
        model *p;
        shared_obj_guard o;
        gfx_readlock *gl;
        model_writelock *pl;
        unsigned int ms_each_frame;

        gl = (gfx_readlock *)o.tryReadLock( this->g, 100, "model_man::runModels" );
        if( gl )
            this->ms_each_frame = gl->getMsEachFrame();
        o.unlock();

        this->instance_cnt = 0;
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_writelock *)o.tryWriteLock( p, 100, "model_man::runModels" );
            if( !pl )
                continue;
            pl->run( thd, ms_each_frame );
            if( !pl->getRefCount() && !pl->getInstanceCount() && !pl->isLinked() )
                d.push_back( p );
            this->instance_cnt += pl->getInstanceCount();
        }
        o.unlock();

        if( d.empty() )
            return;

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->models.remove( p );
            this->model_cnt--;
            delete p;
        }
    }

    //create model using name (if not exists, reuses if does), returns ref in pointer arg
    bool model_man::createModel( const char *mname, model_ref **r )
    {
        model_ref *pr;
        model *p;
        model_writelock *pl;
        shared_obj_guard o, otp;
        std::string s;
        dpid nid;
        dptaskpool_writelock *tp;

        pr = this->findModel( mname );
        if( pr )
        {
            if( r )
                *r = pr;
            else
                delete pr;
            return 1;
        }

        tp = (dptaskpool_writelock *)otp.tryWriteLock( this->tpr, 1000, "model_man::createModel" );
        if( !tp )
            return 0;
        nid = tp->genId();
        otp.unlock();

        p = new model( this->c, nid );
        this->model_cnt++;

        pl = (model_writelock *)o.writeLock( p, "model_man::createModel" );
        if( !pl )
        {
            delete p;
            return 0;
        }

        this->models.push_back( p );

        s.assign( mname );
        pl->setName( &s );

        if( r )
            *r = (model_ref *)pl->getRef();

        return 1;
    }

    //create model and load model file into it
    bool model_man::loadModel( const char *mname, const char *path_name, const char *file_name, model_ref **r, model_loader_ref **mldr )
    {
        model_ref *pr;
        shared_obj_guard o;
        model_loader_man_writelock *l;

        l = (model_loader_man_writelock *)o.tryWriteLock( this->loader_mgr, 3000, "gfx::loadModel" );
        if( !l )
            return 0;

        if( !this->createModel( mname, &pr ) )
            return 0;

        if( !l->load( pr, path_name, file_name, mldr ) )
        {
            delete pr;
            return 0;
        }

        if( r )
            *r = pr;
        else
            delete pr;

        return 1;
    }

    //find model and save model file
    bool model_man::saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr )
    {
        model_ref *pr;
        shared_obj_guard o;
        model_loader_man_writelock *l;

        l = (model_loader_man_writelock *)o.tryWriteLock( this->loader_mgr, 3000, "model_man::loadModel" );
        if( !l )
            return 0;

        pr = this->findModel( mname );
        if( !pr )
            return 0;

        if( !l->save( pr, path_name, file_name, msvr ) )
        {
            delete pr;
            return 0;
        }

        return 1;
    }

    //find model by name
    model_ref *model_man::findModel( const char *cname )
    {
        std::list<model *> *l;
        std::list<model *>::iterator i;
        model *p;
        model_ref *r;
        model_readlock *pl;
        model_writelock *pwl;
        shared_obj_guard o;
        std::string s( cname );

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_readlock *)o.tryReadLock( p, 1000, "model_man::findModel" );
            if( !pl )
                continue;

            if( !pl->compareName( &s ) )
                continue;

            pwl = (model_writelock *)o.tryWriteLock( p, 1000, "model_man::findModel" );
            if( !pwl )
                continue;

            pwl->addRefCount();
            r = (model_ref *)pwl->getRef();
            return r;
        }

        return 0;
    }

    //find model by id
    model_ref *model_man::findModel( dpid id )
    {
        std::list<model *> *l;
        std::list<model *>::iterator i;
        model *p;
        model_ref *r;
        model_readlock *pl;
        model_writelock *pwl;
        shared_obj_guard o;

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_readlock *)o.tryReadLock( p, 1000, "model_man::findModel" );
            if( !pl )
                continue;

            if( !pl->compareId( id ) )
                continue;

            pwl = (model_writelock *)o.tryWriteLock( p, 1000, "model_man::findModel" );
            if( !pwl )
                continue;

            r = (model_ref *)pwl->getRef();
            return r;
        }

        return 0;
    }

    //get models
    void model_man::getModels( std::list<model *> *ll )
    {
        std::list<model *> *l;
        std::list<model *>::iterator i;
        model *p;

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            ll->push_back( p );
        }
    }

    //return count of loaded models
    unsigned int model_man::getModelCount( void )
    {
        return this->model_cnt;
    }

    //return model instance count
    unsigned int model_man::getInstanceCount( void )
    {
        return this->instance_cnt;
    }

};
