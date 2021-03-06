
#include "core.h"
#include "dpmutex/dpmutexes.h"
#include "dptaskpool/dptaskpools.h"
#include "../gfx/gfx.h"
#include "../gfx/gfx_writelock.h"
#include "../gfx/gfx_ref.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "../core/dpmutex/dpmutex_lock.h"
#include "../gfx/gui/root_gui/root_gui_factory.h"
#include "../gfx/gui/gui_man_writelock.h"

namespace dragonpoop
{

    //ctor
    core::core( void )
    {
        dptaskpool_writelock *tpl;
        shared_obj_guard g;

        this->bIsRun = 1;
        this->mm = new dpmutex_master();
        this->tp = new dptaskpool( this->mm, 0 );

        tpl = (dptaskpool_writelock *)g.writeLock( this->tp, "core::core" );
        this->g = new gfx( this, tpl );

        this->orig_root = new root_gui_factory( this );
        this->setRootGui( this->orig_root );
    }

    //dtor
    core::~core( void )
    {
        delete this->orig_root;
        delete this->g;
        delete this->tp;
//        delete this->mm;
    }

    //returns true if running
    bool core::isRunning( void )
    {
        return this->bIsRun;
    }

    //kills core
    void core::kill( void )
    {
        this->bIsRun = 0;
    }

    //returns mutex master
    dpmutex_master *core::getMutexMaster( void )
    {
        return this->mm;
    }

    //returns taskpool lock
    dptaskpool_ref *core::getTaskpool( void )
    {
        dptaskpool_ref *r;
        dptaskpool_writelock *l;
        shared_obj_guard g;

        l = (dptaskpool_writelock *)g.writeLock( this->tp, "core::getTaskpool" );
        r = (dptaskpool_ref *)l->getRef();

        return r;
    }

    //returns gfx ref
    gfx_ref *core::getGfx( void )
    {
        gfx_writelock *gl;
        shared_obj_guard o;

        gl = (gfx_writelock *)o.writeLock( this->g, "core::getGfx" );
        return (gfx_ref *)gl->getRef();
    }

    //add task, will create ref so caller retains original
    void core::addTask( dptask *t )
    {
        dptaskpool_writelock *rl;
        shared_obj_guard o;
        rl = (dptaskpool_writelock *)o.writeLock( this->tp, "core::addTask" );
        rl->addTask( t );
    }

    //add task, will create ref so caller retains original
    void core::addTask( dptask_ref *t )
    {
        dptaskpool_writelock *rl;
        shared_obj_guard o;
        rl = (dptaskpool_writelock *)o.writeLock( this->tp, "core::addTask" );
        rl->addTask( t );
    }

    //set root gui factory
    void core::setRootGui( gui_factory *g )
    {
        gfx_writelock *gl;
        gui_man_writelock *guil;
        shared_obj_guard o, o1;

        gl = (gfx_writelock *)o.writeLock( this->g, "core::setRootGui" );
        if( !gl )
            return;
        if( !gl->getGuis( &guil, &o1 ) )
            return;
        o.unlock();
        guil->setRootGui( g );
    }

};
