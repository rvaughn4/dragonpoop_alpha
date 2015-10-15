
#include "gfx.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "gfx_task.h"
#include "gfx_readlock.h"
#include "gfx_writelock.h"
#include "gfx_ref.h"
#include "../core/dpthread/dpthread_lock.h"
#include "../core/dptaskpool/dptaskpool_writelock.h"
#include "../core/dptaskpool/dptaskpool_readlock.h"
#include "../core/dptaskpool/dptaskpool_ref.h"
#include "../renderer/renderers.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "model/model.h"
#include "model/model_ref.h"
#include "model/model_writelock.h"
#include "model/model_readlock.h"
#include "model/model_instance/model_instance_ref.h"
#include "model/model_instance/model_instance_writelock.h"
#include "gui/gui_ref.h"
#include "gui/gui.h"
#include "gui/gui_writelock.h"
#include "gui/gui_factory.h"
#include "gui/gui_factory_ref.h"
#include "gui/gui_factory_writelock.h"
#include "gui/root_gui/root_gui_factory.h"
#include "dpactor/dpactor.h"
#include "dpactor/dpactor_ref.h"
#include "dpactor/dpactor_writelock.h"
#include "dpland/dpland_man.h"
#include "dpactor/dpactor_man.h"
#include "dpactor/dpactor_man_readlock.h"
#include "dpactor/dpactor_man_writelock.h"
#include "model/model_loader/model_loader_man.h"
#include "model/model_loader/model_loader_man_readlock.h"
#include "model/model_loader/model_loader_man_writelock.h"


namespace dragonpoop
{

    //ctor
    gfx::gfx( core *c, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        gfx_writelock *l;
        shared_obj_guard o;
        
        this->root_g = 0;
        this->root_factory = 0;
        this->gui_cnt = 0;
        this->model_cnt = 0;

        this->c = c;
        this->r = 0;
        this->_startTasks( c, tp );
        this->tpr = (dptaskpool_ref *)tp->getRef();

        this->loader_mgr = new model_loader_man( c, this, tp );
        this->land_mgr = new dpland_man( c, this, tp );
        this->actor_mgr = new dpactor_man( c, this, tp );
        
        l = (gfx_writelock *)o.writeLock( this, "gfx::gfx" );
        this->r = new openglx_1o5_renderer( c, l, tp );
    }

    //dtor
    gfx::~gfx( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "gfx::~gfx" );
        if( this->root_g )
            delete this->root_g;
        this->root_g = 0;
        if( this->root_factory )
            delete this->root_factory;
        this->root_factory = 0;
        o.unlock();
        this->unlink();
        
        this->kill();
        this->_deleteTasks();
        
        o.tryWriteLock( this, 5000, "gfx::~gfx" );
        
        delete this->actor_mgr;
        delete this->land_mgr;
        delete this->loader_mgr;
        
        this->deleteGuis();
        this->deleteModels();
        delete this->r;
        this->r = 0;
        o.unlock();
    }

    //start all tasks
    void gfx::_startTasks( core *c, dptaskpool_writelock *tp )
    {
        this->_startTask( c, tp, &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk, 1, 0, 0, 0, 100 );
        this->_startTask( c, tp, &this->tasks.tmodels.gtsk, &this->tasks.tmodels.tsk, 0, 1, 0, 0, 10 );
        this->_startTask( c, tp, &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk, 0, 0, 1, 0, 50 );
        this->_startTask( c, tp, &this->tasks.tactors.gtsk, &this->tasks.tactors.tsk, 0, 0, 0, 1, 200 );
        this->bIsRun = 1;
    }
    
    //start task
    void gfx::_startTask( core *c, dptaskpool_writelock *tp, gfx_task **pgtsk, dptask **ptsk, bool bRunGfx, bool bRunModels, bool bRunGuis, bool bRunActors, unsigned int ms_delay )
    {
        *pgtsk = new gfx_task( this, bRunGfx, bRunModels, bRunGuis, bRunActors );
        *ptsk = new dptask( c->getMutexMaster(), *pgtsk, ms_delay, 0, "gfx" );
        tp->addTask( *ptsk );
    }
    
    //kill all tasks
    void gfx::_killTasks( void )
    {
        this->_killTask( &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk );
        this->_killTask( &this->tasks.tmodels.gtsk, &this->tasks.tmodels.tsk );
        this->_killTask( &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk );
        this->_killTask( &this->tasks.tactors.gtsk, &this->tasks.tactors.tsk );
        this->bIsRun = 0;
    }
    
    //kill task
    void gfx::_killTask( gfx_task **pgtsk, dptask **ptsk )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !*ptsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( *ptsk, "gfx::genRef" );
        tl->kill();
        o.unlock();
    }
    
    //delete all tasks
    void gfx::_deleteTasks( void )
    {
        this->_deleteTask( &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk );
        this->_deleteTask( &this->tasks.tmodels.gtsk, &this->tasks.tmodels.tsk );
        this->_deleteTask( &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk );
        this->_deleteTask( &this->tasks.tactors.gtsk, &this->tasks.tactors.tsk );
    }
    
    //delete task
    void gfx::_deleteTask( gfx_task **pgtsk, dptask **ptsk )
    {
        if( *ptsk )
            delete *ptsk;
        *ptsk = 0;
        if( *pgtsk )
            delete *pgtsk;
        *pgtsk = 0;
    }
    
    //returns true if running
    bool gfx::isRunning( void )
    {
        return this->bIsRun;
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
        this->_killTasks();
    }

    //run gfx from task
    void gfx::run( dpthread_lock *thd, gfx_writelock *g )
    {
        uint64_t t;
        shared_obj_guard o;
        renderer_readlock *l;
        gui_factory_writelock *fl;
        
        t = thd->getTicks();
        if( this->r && t - this->last_r_poll > 4000 )
        {
            l = (renderer_readlock *)o.tryReadLock( this->r, 3, "gfx::run" );
            if( l )
            {
                this->ms_each_frame = l->getMsPerFrame();
                this->fps = l->getFps();
                this->last_r_poll = t;
            }
            o.unlock();
        }
        
        if( !this->root_g && this->root_factory )
        {
            fl = (gui_factory_writelock *)o.tryWriteLock( this->root_factory, 10, "gfx::run" );
            if( fl )
            {
                this->root_g = fl->makeGui( thd, 0 );
                if( this->root_g )
                    this->addGui( this->root_g );
            }
            o.unlock();
        }

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

    //delete all guis
    void gfx::deleteGuis( void )
    {
        std::list<gui_ref *> *l, d;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        
        l = &this->guis;
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
    void gfx::runModels( dpthread_lock *thd, gfx_ref *g )
    {
        std::list<model *> *l, d;
        std::list<model_ref *> lr;
        std::list<model *>::iterator i;
        std::list<model_ref *>::iterator ir;
        model *p;
        model_ref *pr;
        shared_obj_guard o, og;
        model_writelock *pl;
        gfx_readlock *grl;
        gfx_writelock *gwl;
        unsigned int ms_each_frame;
        
        grl = (gfx_readlock *)og.tryReadLock( g, 100, "gfx::runModels" );
        if( !grl )
            return;
        ms_each_frame = grl->t->ms_each_frame;
        
        l = &grl->t->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
            else
            {
                pl = (model_writelock *)o.tryWriteLock( p, 100, "gfx::runModels" );
                if( pl )
                {
                    pr = (model_ref *)pl->getRef();
                    if( pr )
                        lr.push_back( pr );
                }
                o.unlock();
            }
        }
        og.unlock();
        
        for( ir = lr.begin(); ir != lr.end(); ++ir )
        {
            pr = *ir;
            pl = (model_writelock *)o.tryWriteLock( pr, 100, "gfx::runModels" );
            if( pl )
            {
                pl->run( thd, ms_each_frame );
                if( !pl->getRefCount() && !pl->getInstanceCount() && !pl->isLinked() )
                    d.push_back( p );
            }
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        gwl = (gfx_writelock *)og.tryReadLock( g, 100, "gfx::runModels" );
        if( !gwl )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            gwl->t->models.remove( p );
            delete p;
            gwl->t->model_cnt--;
        }
        og.unlock();
    }
    
    //run all guis
    void gfx::runGuis( dpthread_lock *thd, gfx_ref *g )
    {
        std::list<gui_ref *> *l, ll, d;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        shared_obj_guard o, og;
        gui_writelock *pl;
        gfx_readlock *grl;
        gfx_writelock *gwl;
        
        grl = (gfx_readlock *)og.tryReadLock( g, 100, "gfx::runGuis" );
        if( !grl )
            return;
        
        l = &grl->t->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
            else
            {
                pl = (gui_writelock *)o.tryWriteLock( p, 100, "gfx::runGuis" );
                if( pl )
                {
                    p = (gui_ref *)pl->getRef();
                    if( p )
                    ll.push_back( p );
                }
                o.unlock();
            }
        }
        og.unlock();
        
        l = &ll;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (gui_writelock *)o.tryWriteLock( p, 100, "gfx::runGuis" );
            if( pl )
                pl->run( thd );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        gwl = (gfx_writelock *)og.tryReadLock( g, 100, "gfx::runGuis" );
        if( !gwl )
            return;

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            gwl->t->guis.remove( p );
            delete p;
            gwl->t->gui_cnt--;
        }
        og.unlock();
    }
    
    //create model using name (if not exists, reuses if does), returns ref in pointer arg
    bool gfx::createModel( const char *mname, model_ref **r )
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
        
        tp = (dptaskpool_writelock *)otp.tryWriteLock( this->tpr, 1000, "gfx::createModel" );
        if( !tp )
            return 0;
        nid = tp->genId();
        otp.unlock();
        
        p = new model( this->c, nid );
        this->model_cnt++;
        
        pl = (model_writelock *)o.writeLock( p, "gfx::createModel" );
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
    bool gfx::loadModel( const char *mname, const char *path_name, const char *file_name, model_ref **r, model_loader_ref **mldr )
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
    bool gfx::saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr )
    {
        model_ref *pr;
        shared_obj_guard o;
        model_loader_man_writelock *l;
        
        l = (model_loader_man_writelock *)o.tryWriteLock( this->loader_mgr, 3000, "gfx::loadModel" );
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
            pl = (model_readlock *)o.tryReadLock( p, 1000, "gfx::findModel" );
            if( !pl )
                continue;
            
            if( !pl->compareName( &s ) )
                continue;
            
            pwl = (model_writelock *)o.tryWriteLock( p, 1000, "gfx::findModel" );
            if( !pwl )
                continue;
            
            pwl->addRefCount();
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
            pl = (model_readlock *)o.tryReadLock( p, 1000, "gfx::findModel" );
            if( !pl )
                continue;

            if( !pl->compareId( id ) )
                continue;
            
            pwl = (model_writelock *)o.tryWriteLock( p, 1000, "gfx::findModel" );
            if( !pwl )
                continue;
            
            r = (model_ref *)pwl->getRef();
            return r;
        }
        
        return 0;
    }
    
    //get a model instance by name
    dpid gfx::makeModelInstance( const char *cname, model_instance_ref **r )
    {
        model_ref *m;
        dpid rid;
        
        m = this->findModel( cname );
        if( !m )
        {
            dpid_zero( &rid );
            return rid;
        }
        
        rid = this->makeModelInstance( m, r );
        
        delete m;
        return rid;
    }
    
    //get a model instance by id
    dpid gfx::makeModelInstance( dpid id, model_instance_ref **r )
    {
        model_ref *m;
        dpid rid;
        
        m = this->findModel( id );
        if( !m )
        {
            dpid_zero( &rid );
            return rid;
        }
        
        rid = this->makeModelInstance( m, r );
        
        delete m;
        return rid;
    }
    
    //get a model instance (returns instance id)
    dpid gfx::makeModelInstance( model_ref *m, model_instance_ref **rr )
    {
        dpid rid, nid;
        model_writelock *ml;
        shared_obj_guard o, ot;
        dptaskpool_readlock *tpl;
        dpthread_lock *tl;
        model_instance_ref *r;

        dpid_zero( &rid );
        if( !m )
            return rid;

        ml = (model_writelock *)o.writeLock( m, "gfx::makeModelInstance" );
        if( !ml )
            return rid;
        tpl = (dptaskpool_readlock *)ot.tryReadLock( this->tpr, 1000, "gfx::makeModelInstance" );
        if( !tpl )
            return rid;
        
        tl = tpl->lockThread();
        if( !tl )
            return rid;
        nid = tl->genId();
        
        r = ml->makeInstance( tl, nid );
        delete tl;

        if( !r )
            return rid;
        if( rr )
            *rr = r;
        else
            delete r;
        
        return nid;
    }
    
    //get models
    void gfx::getModels( std::list<model *> *ll )
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
    
    //start animation by name (returns animation instance id)
    dpid gfx::startAnimation( const char *mname, dpid minstance_id, const char *anim_name, bool do_repeat, float speed )
    {
        dpid rid;
        model_instance_ref *mr;
        model_instance_writelock *mil;
        model_ref *m;
        model_readlock *ml;
        shared_obj_guard om, omr, ot;
        dptaskpool_readlock *tl;
        
        dpid_zero( &rid );
        m = this->findModel( mname );
        if( !m )
            return rid;
        
        ml = (model_readlock *)om.tryWriteLock( m, 300, "gfx::startAnimation" );
        delete m;
        if( !ml )
            return rid;
        
        mr = ml->findInstance( minstance_id );
        if( !mr )
            return rid;
        
        mil = (model_instance_writelock *)omr.tryWriteLock( mr, 300, "gfx::startAnimation" );
        delete mr;
        if( !mil )
            return rid;
        
        tl = (dptaskpool_readlock *)ot.tryReadLock( this->tpr, 300, "gfx::startAnimation" );
        if( !tl )
            return rid;
        
        rid = mil->playAnimation( tl->genId(), ml, anim_name, do_repeat, speed );
     
        return rid;
    }
    
    //stop animation by name
    void gfx::stopAnimation( const char *mname, dpid minstance_id, const char *anim_name )
    {
        model_instance_ref *mr;
        model_instance_writelock *mil;
        model_ref *m;
        model_readlock *ml;
        shared_obj_guard om, omr;
        
        m = this->findModel( mname );
        if( !m )
            return;
        
        ml = (model_readlock *)om.tryWriteLock( m, 300, "gfx::stopAnimation" );
        delete m;
        if( !ml )
            return;
        
        mr = ml->findInstance( minstance_id );
        if( !mr )
            return;
        
        mil = (model_instance_writelock *)omr.tryWriteLock( mr, 300, "gfx::stopAnimation" );
        delete mr;
        if( !mil )
            return;
        
        mil->stopAnimation( anim_name );
    }
    
    //stop all animations
    void gfx::stopAllAnimations( const char *mname, dpid minstance_id )
    {
        model_instance_ref *mr;
        model_instance_writelock *mil;
        model_ref *m;
        model_readlock *ml;
        shared_obj_guard om, omr;
        
        m = this->findModel( mname );
        if( !m )
            return;
        
        ml = (model_readlock *)om.tryWriteLock( m, 300, "gfx::stopAnimation" );
        delete m;
        if( !ml )
            return;
        
        mr = ml->findInstance( minstance_id );
        if( !mr )
            return;
        
        mil = (model_instance_writelock *)omr.tryWriteLock( mr, 300, "gfx::stopAnimation" );
        delete mr;
        if( !mil )
            return;
        
        mil->stopAllAnimations();
    }
    
    //add gui
    void gfx::addGui( gui *g )
    {
        shared_obj_guard o;
        gui_writelock *l;
        gui_ref *r;
        
        l = (gui_writelock *)o.writeLock( g, "gfx::addGui" );
        if( !l )
            return;
        
        r = (gui_ref *)l->getRef();
        if( r )
        {
            this->guis.push_back( r );
            this->gui_cnt++;
        }
    }
    
    //add gui
    void gfx::addGui( gui_ref *g )
    {
        shared_obj_guard o;
        gui_writelock *l;
        gui_ref *r;
        
        l = (gui_writelock *)o.writeLock( g, "gfx::addGui" );
        if( !l )
            return;
        
        r = (gui_ref *)l->getRef();
        if( r )
        {
            this->guis.push_back( r );
            this->gui_cnt++;
        }
    }
    
    //get guis
    void gfx::getGuis( std::list<gui_ref *> *ll )
    {
        std::list<gui_ref *> *l;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            ll->push_back( p );
        }
    }
    
    //return fps
    float gfx::getFps( void )
    {
        return this->fps;
    }
    
    //return ms each frame
    unsigned int gfx::getMsEachFrame( void )
    {
        return this->ms_each_frame;
    }
    
    //set root gui factory
    void gfx::setRootGui( gui_factory *g )
    {
        shared_obj_guard o;
        gui_factory_writelock *gl;
        
        if( this->root_g )
            delete this->root_g;
        this->root_g = 0;
        if( this->root_factory )
            delete this->root_factory;
        this->root_factory = 0;
        
        gl = (gui_factory_writelock *)o.tryWriteLock( g, 2000, "gfx::setRootGui" );
        if( !gl )
            return;
        this->root_factory = (gui_factory_ref *)gl->getRef();
    }
    
    //return renderer
    renderer_ref *gfx::getRenderer( void )
    {
        shared_obj_guard o;
        renderer_writelock *l;
        
        if( !this->r )
            return 0;
        l = (renderer_writelock *)o.tryWriteLock( this->r, 1000, "gfx::getRenderer" );
        if( !l )
            return 0;
        
        return (renderer_ref *)l->getRef();
    }
    
    //return model count
    unsigned int gfx::getModelCount( void )
    {
        return this->model_cnt;
    }
    
    //return gui count
    unsigned int gfx::getGuiCount( void )
    {
        return this->gui_cnt;
    }
    
    //get camera position
    void gfx::getCameraPosition( dpposition *p )
    {
        p->copy( &this->cam_pos );
    }
    
    //set camera position
    void gfx::setCameraPosition( dpposition *p )
    {
        shared_obj_guard o;
        renderer_writelock *l;
        
        this->cam_pos.copy( p );

        if( !this->r )
            return;
        l = (renderer_writelock *)o.tryWriteLock( this->r, 1000, "gfx::setCameraPosition" );
        if( !l )
            return;
        l->syncCamera();
    }
    
    //add actor
    void gfx::addActor( dpactor *a )
    {
        shared_obj_guard o;
        dpactor_man_writelock *l;
        
        l = (dpactor_man_writelock *)o.tryWriteLock( this->actor_mgr, 500, "gfx::addActor" );
        if( !l )
            return;
        
        return l->addActor( a );
    }
    
    //add actor
    void gfx::addActor( dpactor_ref *a )
    {
        shared_obj_guard o;
        dpactor_man_writelock *l;
        
        l = (dpactor_man_writelock *)o.tryWriteLock( this->actor_mgr, 500, "gfx::addActor" );
        if( !l )
            return;
        
        return l->addActor( a );
    }
    
    //return actor count
    unsigned int gfx::getActorCount( void )
    {
        shared_obj_guard o;
        dpactor_man_readlock *l;
        
        l = (dpactor_man_readlock *)o.tryReadLock( this->actor_mgr, 500, "gfx::getActorCount" );
        if( !l )
            return 0;
        
        return l->getActorCount();
    }
    
};
