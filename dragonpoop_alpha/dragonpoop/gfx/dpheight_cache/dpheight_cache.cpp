
#include "dpheight_cache.h"
#include "dpheight_cache_ref.h"
#include "dpheight_cache_readlock.h"
#include "dpheight_cache_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache::dpheight_cache( dpmutex_master *mm ) : shared_obj( mm )
    {
        std::shared_ptr<std::atomic<uint64_t>> tt( new std::atomic<uint64_t>() );
        this->t = tt;
        this->fv = 0;
        this->w = 0;
        this->h = 0;
        this->tile_size = 0;
    }

    //dtor
    dpheight_cache::~dpheight_cache( void )
    {
        this->clear();
    }

    //generate write lock
    shared_obj_writelock *dpheight_cache::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpheight_cache_writelock( (dpheight_cache *)p, l );
    }

    //generate read lock
    shared_obj_readlock *dpheight_cache::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpheight_cache_readlock( (dpheight_cache *)p, l );
    }

    //generate ref
    shared_obj_ref *dpheight_cache::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpheight_cache_ref( (dpheight_cache *)p, k, this->t );
    }

    //set camera position
    void dpheight_cache::setPosition( dpposition *p )
    {
        dpposition_inner i;
        this->pos.copy( p );
        p->getData( &i );
        *( this->t.get() ) = i.end.t;
    }

    //get camera position
    dpposition *dpheight_cache::getPosition( void )
    {
        return &this->pos;
    }

    //get position time
    uint64_t dpheight_cache::getTime( void )
    {
        return *( this->t.get() );
    }

    //set time
    void dpheight_cache::setTime( uint64_t t )
    {
        *( this->t.get() ) = t;
    }

    //resize and set center
    void dpheight_cache::setDimensions( double w, double h, double x, double z, double tile_size )
    {
        unsigned int iw, ih, is;

        iw = (unsigned int)w;
        ih = (unsigned int)h;
        is = (unsigned int)tile_size;
        iw /= is;
        ih /= is;
        this->x = x - w / 2;
        this->z = z - h / 2;

        if( !this->fv || iw != this->w || ih != this->h )
        {
            this->clear();
            this->fv = new float[ iw * ih ];
            this->w = iw;
            this->h = ih;
            this->tile_size = is;
        }
    }

    //set height at coord
    void dpheight_cache::setHeight( double x, double z, float h )
    {
        unsigned int ix, iz, ii;

        if( !this->fv )
            return;

        x -= this->x;
        z -= this->z;
        x /= this->tile_size;
        z /= this->tile_size;
        x -= this->w / 2;
        z -= this->h / 2;

        if( x < 0 || z < 0 )
            return;
        ix = (unsigned int)x;
        iz = (unsigned int)z;
        if( ix >= this->w || iz >= this->h )
            return;

        ii = iz * this->w;
        ii += ix;
        this->fv[ ii ] = h;
    }

    //get height at tile
    float dpheight_cache::_getHeight( unsigned int ix, unsigned int iz )
    {
        unsigned int ii;

        if( ix >= this->w || iz >= this->h )
            return 0;

        ii = iz * this->w;
        ii += ix;
        return this->fv[ ii ];
    }

    //get height at coord
    float dpheight_cache::getHeight( double x, double z )
    {
        unsigned int ix, iz;
        float rx, rz, rx1, rz1, h00, h01, h10, h11;

        if( !this->fv )
            return 0;

        x -= this->x;
        z -= this->z;
        x /= this->tile_size;
        z /= this->tile_size;
        x -= this->w / 2;
        z -= this->h / 2;

        if( x < 0 || z < 0 )
            return 0;
        ix = (unsigned int)x;
        iz = (unsigned int)z;

        h00 = this->_getHeight( ix + 0, iz + 0 );
        h01 = this->_getHeight( ix + 1, iz + 0 );
        h10 = this->_getHeight( ix + 0, iz + 1 );
        h11 = this->_getHeight( ix + 1, iz + 1 );

        rx = (float)x - (float)ix;
        rz = (float)z - (float)iz;
        rx1 = 1.0f - rx;
        rz1 = 1.0f - rz;

        return h00 * rx1 * rz1 + h01 * rx * rz1 + h10 * rx1 * rz + h11 * rx * rz;
    }

    //clear
    void dpheight_cache::clear( void )
    {
        if( this->fv )
            delete[] this->fv;
        this->fv = 0;
        this->w = 0;
        this->h = 0;
        this->tile_size = 0;
    }

    //sync
    void dpheight_cache::sync( dpheight_cache_ref *r )
    {
        dpheight_cache_readlock *l;
        shared_obj_guard o;
        unsigned int iw, ih, is, ii, ie;

        if( r->getTime() == this->getTime() )
            return;

        l = (dpheight_cache_readlock *)o.tryReadLock( r, 30, "dpheight_cache::sync" );
        if( !l )
            return;

        this->pos.copy( l->getPosition() );
        *( this->t.get() ) = l->getTime();

        if( !l->t->fv )
            return;

        iw = l->t->w;
        ih = l->t->h;
        is = l->t->tile_size;
        this->x = l->t->x;
        this->z = l->t->z;

        if( !this->fv || iw != this->w || ih != this->h )
        {
            this->clear();
            this->fv = new float[ iw * ih ];
            this->w = iw;
            this->h = ih;
            this->tile_size = is;
        }

        if( !this->fv )
            return;

        ie = this->w * this->h;
        for( ii = 0; ii < ie; ii++ )
            this->fv[ ii ] = l->t->fv[ ii ];
    }


};
