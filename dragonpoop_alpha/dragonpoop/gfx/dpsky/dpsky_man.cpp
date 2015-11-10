
#include "dpsky_man.h"
#include "dpsky_man_ref.h"
#include "dpsky_man_readlock.h"
#include "dpsky_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "dpsky_man_task.h"
#include "../gfx_ref.h"
#include "../gfx_readlock.h"
#include "../gfx.h"
#include "../gfx_writelock.h"
#include "dpsky.h"
#include "../../core/dpthread/dpthread_lock.h"

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    dpsky_man::dpsky_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        
        this->c = c;
        this->loadSky();
        this->buildSky();
        
        gl = (gfx_writelock *)o.writeLock( g, "dpsky_man::dpsky_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();
        
        this->_startTask( tp, 200 );
    }
    
    //dtor
    dpsky_man::~dpsky_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "dpsky_man::~dpsky_man" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "dpsky_man::~dpsky_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "dpsky_man::~dpsky_man" );
        this->_deleteTask();
        delete this->g;
        o.unlock();
    }
    
    //return core
    core *dpsky_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *dpsky_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpsky_man_readlock( (dpsky_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *dpsky_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpsky_man_writelock( (dpsky_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *dpsky_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpsky_man_ref( (dpsky_man *)p, k );
    }
    
    //start task
    void dpsky_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new dpsky_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "dpsky_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void dpsky_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "dpsky_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void dpsky_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //run
    void dpsky_man::run( dpthread_lock *thd, dpsky_man_writelock *g )
    {

    }
    
    //build skydome
    void dpsky_man::buildSky( void )
    {
        dpvertex_buffer *vb;
        dpindex_buffer *ib;
        dpvertex v;
        
        vb = &this->stuff.skybox.front.vb;
        ib = &this->stuff.skybox.front.ib;
        
        v.pos.x = -1;
        v.pos.y = 1;
        v.pos.z = -1;
        v.texcoords[ 0 ].s = 0;
        v.texcoords[ 0 ].t = 0;
        vb->addVertex( &v );
        
        v.pos.x = 1;
        v.pos.y = 1;
        v.pos.z = -1;
        v.texcoords[ 0 ].s = 1;
        v.texcoords[ 0 ].t = 0;
        vb->addVertex( &v );
        
        v.pos.x = -1;
        v.pos.y = -1;
        v.pos.z = -1;
        v.texcoords[ 0 ].s = 0;
        v.texcoords[ 0 ].t = 1;
        vb->addVertex( &v );
        
        v.pos.x = 1;
        v.pos.y = -1;
        v.pos.z = -1;
        v.texcoords[ 0 ].s = 1;
        v.texcoords[ 0 ].t = 1;
        vb->addVertex( &v );
        
        ib->addIndex( 0 );
        ib->addIndex( 2 );
        ib->addIndex( 1 );
        ib->addIndex( 1 );
        ib->addIndex( 2 );
        ib->addIndex( 3 );
        
    }
    
    //load sky textures
    void dpsky_man::loadSky( void )
    {
        
        this->stuff.skyboxtex.stars.front.loadFile( "skybox_stars_front.bmp" );
    }
    
    //return sky stuff
    dpsky_stuff *dpsky_man::getSky( void )
    {
        return &this->stuff;
    }
    
    
};
