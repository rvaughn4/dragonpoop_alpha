
#include "dpland_man.h"
#include "dpland_man_ref.h"
#include "dpland_man_readlock.h"
#include "dpland_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "dpland_man_task.h"
#include "../gfx_ref.h"
#include "../gfx_readlock.h"
#include "../gfx.h"
#include "../gfx_writelock.h"
#include "dpland.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "../dpposition/dpposition_share_readlock.h"
#include "../dpposition/dpposition_share_ref.h"
#include "../dpheight_cache/dpheight_cache_ref.h"
#include "../dpheight_cache/dpheight_cache_writelock.h"

#include <math.h>

namespace dragonpoop
{

    //ctor
    dpland_man::dpland_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;

        this->c = c;

        this->world_sz = 100.0f;
        this->land_sz = 20.0f;
        this->tile_sz = 5.0f;
        this->tex_sz = 50.0f;

        gl = (gfx_writelock *)o.writeLock( g, "dpland_man::dpland_man" );
        this->g = (gfx_ref *)gl->getRef();
        this->cam_pos = gl->getCameraPosition();
        this->heights = gl->getHeights();
        o.unlock();

        this->_startTask( tp, 200 );
    }

    //dtor
    dpland_man::~dpland_man( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        this->_deleteTask();
        delete this->g;
        delete this->cam_pos;
        delete this->heights;
        o.unlock();
    }

    //return core
    core *dpland_man::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *dpland_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpland_man_readlock( (dpland_man *)p, l );
    }

    //generate write lock
    shared_obj_writelock *dpland_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpland_man_writelock( (dpland_man *)p, l );
    }

    //generate ref
    shared_obj_ref *dpland_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpland_man_ref( (dpland_man *)p, k );
    }

    //start task
    void dpland_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new dpland_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, 0, "dpland_man" );
        tp->addTask( this->tsk );
    }

    //kill task
    void dpland_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk, "dpland_man::_killTask" );
        tl->kill();
        o.unlock();
    }

    //delete task
    void dpland_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }

    //run
    void dpland_man::run( dpthread_lock *thd, dpland_man_writelock *g )
    {
        this->runTiles( thd, g );
    }

    //run tiles
    void dpland_man::runTiles( dpthread_lock *thd, dpland_man_writelock *g )
    {
        std::list<dpland *> *l, d;
        std::list<dpland *>::iterator i;
        dpland *p;
        int64_t ix, iz, ex, ez, x, z, step, half_world;
        float f;
        int new_tile_ctr, old_tile_ctr;
        dpposition_share_readlock *gl;
        shared_obj_guard o, o1;
        dpposition_inner pi;
        bool wasUpdated;
        dpheight_cache_writelock *hl;

        l = &this->tiles;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }

        if( this->cam_pos && this->pos.t != this->cam_pos->getTime() )
        {

            gl = (dpposition_share_readlock *)o.tryReadLock( this->cam_pos, 100, "dpland_man::runTiles" );
            if( gl )
            {
                gl->getPosition()->getData( &pi );
                x = pi.start.x / (int64_t)this->land_sz;
                z = pi.start.z / (int64_t)this->land_sz;
                x *= (int64_t)this->land_sz;
                z *= (int64_t)this->land_sz;
                this->pos.x = x;
                this->pos.z = z;
                this->pos.t = gl->getTime();
                o.unlock();
            }

        }

        wasUpdated = 0;
        x = this->pos.x;
        z = this->pos.z;
        step = (int64_t)this->land_sz;
        f = this->world_sz * 0.5f - this->land_sz * 0.5f;
        half_world = (int64_t)f;
        ez = (int64_t)this->world_sz;
        ex = ez;

        new_tile_ctr = 0;
        for( iz = z - half_world; iz < ez; iz += step )
        {
            for( ix = x - half_world; ix < ex; ix += step )
            {
                p = this->getTile( ix, iz );
                if( !p )
                {
                    if( new_tile_ctr < 5 )
                    {
                        this->makeTile( thd, ix, iz );
                        wasUpdated = 1;
                        new_tile_ctr++;
                    }
                }
                else
                    d.remove( p );
            }
        }

        l = &d;
        old_tile_ctr = 0;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( old_tile_ctr < 10 )
            {
                this->tiles.remove( p );
                delete p;
                old_tile_ctr++;
            }
        }

        if( !wasUpdated || !this->heights )
            return;

        hl = (dpheight_cache_writelock *)o.tryWriteLock( this->heights, 100, "dpland_man::runTiles" );
        if( !hl )
            return;

        hl->setDimensions( this->world_sz, this->world_sz, this->pos.x, this->pos.z, this->tile_sz );

        l = &this->tiles;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->setHeights( hl );
        }

        hl->setTime( thd->getTicks() );
    }

    //returns true if tile exists
    dpland *dpland_man::getTile( int64_t x, int64_t z )
    {
        int64_t px, pz;
        std::list<dpland *> *l;
        std::list<dpland *>::iterator i;
        dpland *p;

        l = &this->tiles;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->getPosition( &px, &pz );
            if( px != x || pz != z )
                continue;
            return p;
        }

        return 0;
    }

    //makes a tile at coords
    void dpland_man::makeTile( dpthread_lock *thd, int64_t x, int64_t z )
    {
        dpland *p;

        p = new dpland( thd->genId(), x, z, this->land_sz, this->tile_sz, this->tex_sz );
        if( !p )
            return;
        this->tiles.push_back( p );
    }

    //get tiles
    void dpland_man::getTiles( std::list<dpland *> *ll )
    {
        std::list<dpland *> *l;
        std::list<dpland *>::iterator i;
        dpland *p;

        l = &this->tiles;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            ll->push_back( p );
        }
    }

};
