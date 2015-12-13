
#include "gui_man.h"
#include "gui_man_ref.h"
#include "gui_man_readlock.h"
#include "gui_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "gui_man_task.h"
#include "../gfx_ref.h"
#include "../gfx.h"
#include "../gfx_writelock.h"
#include "../gfx_readlock.h"
#include "gui.h"
#include "gui_ref.h"
#include "gui_readlock.h"
#include "gui_writelock.h"
#include "../../core/dptaskpool/dptaskpool_ref.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "../../core/dptaskpool/dptaskpool_readlock.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "gui_factory_writelock.h"
#include "gui_factory_ref.h"
#include "gui_factory.h"

namespace dragonpoop
{

    //ctor
    gui_man::gui_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;

        this->c = c;
        this->gui_cnt = 0;

        this->tpr = (dptaskpool_ref *)tp->getRef();
        this->root_factory = 0;
        this->root_g = 0;

        gl = (gfx_writelock *)o.writeLock( g, "gui_man::gui_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();

        this->_startTask( tp, 100 );
    }

    //dtor
    gui_man::~gui_man( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "gui_man::~gui_man" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "gui_man::~gui_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "gui_man::~gui_man" );
        this->_deleteTask();
        this->deleteGuis();
        delete this->g;
        delete this->tpr;
        o.unlock();
    }

    //return core
    core *gui_man::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *gui_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gui_man_readlock( (gui_man *)p, l );
    }

    //generate write lock
    shared_obj_writelock *gui_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gui_man_writelock( (gui_man *)p, l );
    }

    //generate ref
    shared_obj_ref *gui_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gui_man_ref( (gui_man *)p, k );
    }

    //start task
    void gui_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new gui_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, 0, "gui_man" );
        tp->addTask( this->tsk );
    }

    //kill task
    void gui_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk, "gui_man::_killTask" );
        tl->kill();
        o.unlock();
    }

    //delete task
    void gui_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }

    //run
    void gui_man::run( dpthread_lock *thd, gui_man_writelock *g )
    {
        shared_obj_guard o;
        gui_factory_writelock *fl;

        this->runGuis( thd );

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

    //delete all guis
    void gui_man::deleteGuis( void )
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

    //run all guis
    void gui_man::runGuis( dpthread_lock *thd )
    {
        std::list<gui_ref *> *l, d;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        shared_obj_guard o, og;
        gui_writelock *pl;

        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
            {
                d.push_back( p );
                continue;
            }
            pl = (gui_writelock *)o.tryWriteLock( p, 100, "gui_man::runGuis" );
            if( !pl )
                continue;
            pl->run( thd );
        }
        o.unlock();

        if( d.empty() )
            return;

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->guis.remove( p );
            delete p;
            this->gui_cnt--;
        }
        og.unlock();
    }

    //add gui
    void gui_man::addGui( gui *g )
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
    void gui_man::addGui( gui_ref *g )
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
    void gui_man::getGuis( std::list<gui_ref *> *ll )
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

    //return gui count
    unsigned int gui_man::getGuiCount( void )
    {
        if( this->gui_cnt < 0 )
            return 0;
        return (unsigned int)this->gui_cnt;
    }

    //set root gui factory
    void gui_man::setRootGui( gui_factory *g )
    {
        shared_obj_guard o;
        gui_factory_writelock *gl;

        if( this->root_g )
            delete this->root_g;
        this->root_g = 0;
        if( this->root_factory )
            delete this->root_factory;
        this->root_factory = 0;

        gl = (gui_factory_writelock *)o.tryWriteLock( g, 2000, "gui_man::setRootGui" );
        if( !gl )
            return;
        this->root_factory = (gui_factory_ref *)gl->getRef();
    }

};
