
#include "gfx.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "gfx_task.h"
#include "gfx_readlock.h"
#include "gfx_writelock.h"
#include "gfx_ref.h"
#include "../core/dpthread/dpthread_lock.h"
#include "../core/dptaskpool/dptaskpool_writelock.h"
#include "../core/dptaskpool/dptaskpool_ref.h"
#include "../renderer/renderers.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "model/model.h"
#include "model/model_ref.h"
#include "model/model_writelock.h"
#include "model/model_readlock.h"
#include "model/model_loader/model_loader.h"
#include "model/model_loader/model_loader_readlock.h"

namespace dragonpoop
{

    //ctor
    gfx::gfx( core *c, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        gfx_writelock *l;
        shared_obj_guard o;

        this->c = c;
        this->r = 0;
        this->gtsk = new gfx_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 500, 1 );
        tp->addTask( this->tsk );
        this->tpr = (dptaskpool_ref *)tp->getRef();

        l = (gfx_writelock *)o.writeLock( this );
        this->r = new openglx_1o5_renderer( c, l, tp );
    }

    //dtor
    gfx::~gfx( void )
    {
        this->kill();
        
        this->deleteLoaders();
        this->deleteModels();
        
        delete this->r;
        this->r = 0;
        delete this->gtsk;
        delete this->tpr;
    }

    //returns true if running
    bool gfx::isRunning( void )
    {
        if( !this->tsk )
            return 0;
        return this->tsk->isLinked();
    }

    //return core
    core *gfx::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *gfx::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gfx_readlock( (gfx *)p, l );
    }

    //generate write lock
    shared_obj_writelock *gfx::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gfx_writelock( (gfx *)p, l );
    }

    //generate ref
    shared_obj_ref *gfx::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gfx_ref( (gfx *)p, k );
    }

    //stop gfx task
    void gfx::kill( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk );
        tl->kill();
        o.unlock();

        delete this->tsk;
        this->tsk = 0;
    }

    //run gfx from task
    void gfx::run( dpthread_lock *thd, gfx_writelock *g )
    {
        this->runLoaders();
        this->runModels();
    }

    //delete all models
    void gfx::deleteModels( void )
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

    //delete all loaders
    void gfx::deleteLoaders( void )
    {
        std::list<model_loader *> *l, d;
        std::list<model_loader *>::iterator i;
        model_loader *p;
        
        l = &this->loaders;
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
    
    //delete old models
    void gfx::runModels( void )
    {
        std::list<model *> *l, d;
        std::list<model *>::iterator i;
        model *p;
        model_readlock *pl;
        shared_obj_guard o;
        
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_readlock *)o.tryReadLock( p, 10 );
            if( !pl )
                continue;
//            d.push_back( p );
        }
        o.unlock();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->models.remove( p );
            delete p;
        }
    }
    
    //delete old loaders
    void gfx::runLoaders( void )
    {
        std::list<model_loader *> *l, d;
        std::list<model_loader *>::iterator i;
        model_loader *p;
        model_loader_readlock *pl;
        shared_obj_guard o;
        
        l = &this->loaders;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_loader_readlock *)o.tryReadLock( p, 100 );
            if( !pl || pl->isRunning() )
                continue;
            d.push_back( p );
        }
        o.unlock();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->loaders.remove( p );
            delete p;
        }
        
    }
    
    //create model using name (if not exists, reuses if does), returns ref in pointer arg
    bool gfx::createModel( dpthread_lock *thd, const char *mname, model_ref **r )
    {
        model_ref *pr;
        model *p;
        model_writelock *pl;
        shared_obj_guard o, otp;
        dptaskpool_writelock *tp;
        std::string s;
        
        pr = this->findModel( mname );
        if( pr )
        {
            if( r )
                *r = pr;
            else
                delete pr;
            return 1;
        }
        
        tp = (dptaskpool_writelock *)otp.tryWriteLock( this->tpr, 1000 );
        if( !tp )
            return 0;
        
        p = new model( this->c, tp, thd->genId() );
        this->models.push_back( p );
        
        pl = (model_writelock *)o.writeLock( p );
        if( !pl )
            return 0;
        s.assign( mname );
        pl->setName( &s );
        
        if( r )
            *r = (model_ref *)pl->getRef();
        
        return 1;
    }
    
    //create model and load model file into it
    bool gfx::loadModel( dpthread_lock *thd, const char *mname, const char *file_name, model_ref **r, model_loader **mldr )
    {
        model_ref *pr;
        model_loader *l;
        shared_obj_guard o, otp;
        dptaskpool_writelock *tp;

        if( !this->createModel( thd, mname, &pr ) )
            return 0;
        
        tp = (dptaskpool_writelock *)otp.tryWriteLock( this->tpr, 1000 );
        if( !tp )
            return 0;
        
        l = model_loader::loadFile( this->c, tp, pr, file_name );
        if( !l )
            return 0;
        
        if( r )
            *r = pr;
        else
            delete pr;
        
        if( mldr )
            *mldr = l;
        else
            this->loaders.push_back( l );
        
        return 1;
    }
    
    //find model by name
    model_ref *gfx::findModel( const char *cname )
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
            pl = (model_readlock *)o.tryReadLock( p, 100 );
            if( !pl )
                continue;
            
            if( !pl->compareName( &s ) )
                continue;
            
            pwl = (model_writelock *)o.tryWriteLock( p, 1000 );
            if( !pwl )
                continue;
            
            r = (model_ref *)pwl->getRef();
            return r;
        }
        
        return 0;
    }
    
    //find model by id
    model_ref *gfx::findModel( dpid id )
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
            pl = (model_readlock *)o.tryReadLock( p, 100 );
            if( !pl )
                continue;

            if( !pl->compareId( id ) )
                continue;
            
            pwl = (model_writelock *)o.tryWriteLock( p, 1000 );
            if( !pwl )
                continue;
            
            r = (model_ref *)pwl->getRef();
            return r;
        }
        
        return 0;
    }
    
    //get a model instance by name
    model_instance_ref *gfx::makeModelInstance( const char *cname )
    {
        model_ref *m;
        model_writelock *ml;
        shared_obj_guard o;
        
        m = this->findModel( cname );
        if( !m )
            return 0;
        ml = (model_writelock *)o.writeLock( m );
        delete m;
        if( !ml )
            return 0;
        return ml->makeInstance();
    }
    
    //get a model instance by id
    model_instance_ref *gfx::makeModelInstance( dpid id )
    {
        model_ref *m;
        model_writelock *ml;
        shared_obj_guard o;
        
        m = this->findModel( id );
        if( !m )
            return 0;
        ml = (model_writelock *)o.writeLock( m );
        delete m;
        if( !ml )
            return 0;
        return ml->makeInstance();
    }
    
};
