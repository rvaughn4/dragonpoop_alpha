
#include "core.h"
#include "dpmutex/dpmutexes.h"
#include "dptaskpool/dptaskpools.h"
#include "../gfx/gfx.h"
#include "../gfx/gfx_writelock.h"
#include "../gfx/gfx_ref.h"
#include "../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    core::core( void )
    {
        dptaskpool_writelock *tpl;
        shared_obj_guard g;

        this->bIsRun = 1;
        this->mm = new dpmutex_master();
        this->tp = new dptaskpool( this->mm, 2 );

        tpl = (dptaskpool_writelock *)g.writeLock( this->tp );
        this->g = new gfx( this, tpl );
    }

    //dtor
    core::~core( void )
    {
        delete g;
        delete this->tp;
        delete this->mm;
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

        l = (dptaskpool_writelock *)g.writeLock( this->tp );
        r = (dptaskpool_ref *)l->getRef();

        return r;
    }

    //returns gfx ref
    gfx_ref *core::getGfx( void )
    {
        gfx_writelock *gl;
        shared_obj_guard o;

        gl = (gfx_writelock *)o.writeLock( this->g );
        return (gfx_ref *)gl->getRef();
    }

    //add task, will create ref so caller retains original
    void core::addTask( dptask *t )
    {
        dptaskpool_writelock *rl;
        shared_obj_guard o;
        rl = (dptaskpool_writelock *)o.writeLock( this->tp );
        rl->addTask( t );
    }

    //add task, will create ref so caller retains original
    void core::addTask( dptask_ref *t )
    {
        dptaskpool_writelock *rl;
        shared_obj_guard o;
        rl = (dptaskpool_writelock *)o.writeLock( this->tp );
        rl->addTask( t );
    }

};
