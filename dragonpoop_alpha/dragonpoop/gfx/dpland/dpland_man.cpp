
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
        this->land_sz = 10.0f;
        this->tile_sz = 1.0f;
        this->tex_sz = 10.0f;
        
        this->buildSky();
        
        gl = (gfx_writelock *)o.writeLock( g, "dpland_man::dpland_man" );
        this->g = (gfx_ref *)gl->getRef();
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
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "dpland_man" );
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
        gfx_readlock *gl;
        shared_obj_guard o;
        dpposition pp;
        dpposition_inner pi;
        
        l = &this->tiles;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        
        gl = (gfx_readlock *)o.tryReadLock( this->g, 10, "dpland_man::runTiles" );
        if( !gl )
            return;
        gl->getCameraPosition( &pp );
        o.unlock();
        
        pp.getData( &pi );
        x = pi.start.whole.x / (int64_t)this->land_sz;
        z = pi.start.whole.z / (int64_t)this->land_sz;
        x *= (int64_t)this->land_sz;
        z *= (int64_t)this->land_sz;
        
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
                    if( new_tile_ctr < 10 )
                    {
                        this->makeTile( thd, ix, iz );
                        new_tile_ctr++;
                    }
                }
                else
                    d.remove( p );
            }
        }

        l = &d;
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

    // Aux function
    inline int dpland_man__buildSky__index( int col_max, int i, int j )
    {
        return i + j * ( col_max + 1 );
    }
    
    //build skydome
    void dpland_man::buildSky( void )
    {
        int col_max, row_max, j, i;
        dpvertex v;
        float teta, fi, radius;
        
        col_max = 10;
        row_max = 10;
        radius = 100.0f;
        
        // Fill coordinate positions [to change]
        for( j = 0; j <= row_max; j++ )
        {
            for( i = 0; i <= col_max; i++ )
            {
                teta = ( (float)i / col_max ) * 2.0f * 3.14f;
                fi = ( (float)j / row_max ) * 3.14f;
                v.pos.x = radius * cosf( teta ) * sinf( fi );
                v.pos.y = radius * cosf( fi );
                v.pos.z = 1.0f * sinf( teta ) * sinf( fi );
                this->sky.vb.addVertex( &v );
            }
        }
        
        // Fill index
        for( j = 0; j < row_max; j++ )
        {
            for( i = 0; i < col_max; i++ )
            {
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i, j ) );
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i + 1, j + 1 ) );
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i, j + 1 ) );
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i, j ) );
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i + 1, j ) );
                this->sky.ib.addIndex( dpland_man__buildSky__index( col_max, i + 1, j + 1 ) );
            }
        }
        
        this->sky.ib.fixBounds( &this->sky.vb );
    }
    
    //get sky
    dpland_skydome *dpland_man::getSky( void )
    {
        return &this->sky;
    }
    
};
