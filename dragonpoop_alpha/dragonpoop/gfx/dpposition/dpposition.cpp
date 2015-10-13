
#include "dpposition.h"
#include "../dpvertex/dpxyz_f.h"
#include <string>
#include <math.h>

namespace dragonpoop
{
    
    //ctor
    dpposition::dpposition( void )
    {
        memset( &this->i, 0, sizeof( this->i ) );
    }
    
    //ctor
    dpposition::dpposition( dpposition *c )
    {
        c->getData( &this->i );
    }
    
    //dtor
    dpposition::~dpposition( void )
    {
        
    }
    
    //express difference as float ( arg - this )
    void dpposition::getDifference( dpposition *arg, uint64_t tm_epoch_now, dpxyz_f *pout )
    {
        double dx0, dy0, dz0, dx1, dy1, dz1, dx2, dy2, dz2;
        
        this->getDiff( &dx0, &dy0, &dz0, tm_epoch_now );
        arg->getDiff( &dx1, &dy1, &dz1, tm_epoch_now );
        this->getDiff( &dx2, &dy2, &dz2, arg );
        
        dx2 += dx1 - dx0;
        dy2 += dy1 - dy0;
        dz2 += dz1 - dz0;
        
        pout->x = dx2;
        pout->y = dy2;
        pout->z = dz2;
    }
    
    //set location
    void dpposition::setLocation( dpposition *arg, uint64_t tm_epoch_now, uint64_t tm_epoch_when )
    {
        double dx, dy, dz;
        int64_t ix, iy, iz;
        
        this->stop( tm_epoch_now );
        
        arg->getDiff( &dx, &dy, &dz, tm_epoch_when );
        ix = (int64_t)dx;
        iy = (int64_t)dy;
        iz = (int64_t)dz;
        dx = dx - (double)ix;
        dy = dy - (double)iy;
        dz = dz - (double)iz;
        
        this->i.end = arg->i.start;
        this->i.end.bReverse = arg->i.end.bReverse;
        this->i.end.t = tm_epoch_when;
        this->i.end.whole.x += ix;
        this->i.end.whole.y += iy;
        this->i.end.whole.z += iz;
        this->i.end.fract.x += dx;
        this->i.end.fract.y += dy;
        this->i.end.fract.z += dz;
    }
    
    //get data
    void dpposition::getData( dpposition_inner *pout )
    {
        *pout = this->i;
    }
    
    //set data
    void dpposition::setData( dpposition_inner *pin )
    {
        this->i = *pin;
    }
    
    //copy
    void dpposition::copy( dpposition *c )
    {
        c->getData( &this->i );
    }
    
    //returns true if moving
    bool dpposition::isMoving( uint64_t tm_epoch_now )
    {
        return this->i.end.t > tm_epoch_now;
    }
    
    //gets velocity and speed
    float dpposition::getSpeed( dpxyz_f *pvel )
    {
        double dx, dy, dz, t;
        
        t = (double)( (uint64_t)( this->i.end.t - this->i.start.t ) );
        if( t <= 0 )
        {
            if( pvel )
            {
                pvel->x = 0;
                pvel->y = 0;
                pvel->z = 0;
            }
            return 0;
        }
        
        dx = (double)( (int64_t)( this->i.end.whole.x - this->i.start.whole.x ) ) + (double)( this->i.end.fract.x - this->i.start.fract.x );
        dy = (double)( (int64_t)( this->i.end.whole.y - this->i.start.whole.y ) ) + (double)( this->i.end.fract.y - this->i.start.fract.y );
        dz = (double)( (int64_t)( this->i.end.whole.z - this->i.start.whole.z ) ) + (double)( this->i.end.fract.z - this->i.start.fract.z );
        
        dx = dx / t;
        dy = dy / t;
        dz = dz / t;
        
        if( pvel )
        {
            pvel->x = dx;
            pvel->y = dy;
            pvel->z = dz;
        }
        
        dx = dx * dx + dy * dy + dz * dz;
        if( dx > 0.0 )
            dx = sqrt( dx );
        else
            dx = 0;
        
        return dx;
    }
    
    //get rotational direction
    void dpposition::getDirection( dpxyz_f *pout )
    {
        double dx, dy, dz;
        float f;
        
        this->getDiff( &dx, &dy, &dz );
        
        pout->y = atan2f( dx, dz );
        f = dx * dx + dz * dz;
        if( f <= 0.0f )
            f = 0.0f;
        else
            f = sqrtf( f );
        pout->x = atan2f( -dy, f );
        pout->z = 0;//atan2f( -dx, -dz );
    }
    
    //move position incrementally
    void dpposition::move( dpxyz_f *pin, uint64_t t_epoch_now, uint64_t t_epoch_when, bool doReverse )
    {
        double dx, dy, dz;
        int64_t ix, iy, iz;

        this->stop( t_epoch_now );
        
        ix = (int64_t)pin->x;
        iy = (int64_t)pin->y;
        iz = (int64_t)pin->z;
        dx = pin->x - (double)ix;
        dy = pin->y - (double)iy;
        dz = pin->z - (double)iz;
        
        this->i.end.t = t_epoch_when;
        this->i.end.whole.x += ix;
        this->i.end.whole.y += iy;
        this->i.end.whole.z += iz;

        dx += this->i.start.fract.x;
        ix = (int64_t)dx;
        dx -= (double)ix;
        this->i.start.fract.x = dx;
        this->i.start.whole.x += ix;
        
        dy += this->i.start.fract.y;
        iy = (int64_t)dy;
        dy -= (double)iy;
        this->i.start.fract.y = dy;
        this->i.start.whole.y += iy;
        
        dz += this->i.start.fract.z;
        iz = (int64_t)dz;
        dz -= (double)iz;
        this->i.start.fract.z = dz;
        this->i.start.whole.z += iz;

        this->i.end.bReverse = doReverse;
    }
    
    //stop moving
    void dpposition::stop( uint64_t t_epoch_now )
    {
        double dx, dy, dz;
        int64_t ix, iy, iz;
        
        if( !this->i.start.t )
            this->i.start.t = t_epoch_now;
        if( !this->i.end.t )
            this->i.end.t = t_epoch_now;
        
        this->getDiff( &dx, &dy, &dz, t_epoch_now );
        
        ix = (int64_t)dx;
        iy = (int64_t)dy;
        iz = (int64_t)dz;
        dx -= (double)ix;
        dy -= (double)iy;
        dz -= (double)iz;
        
        this->i.start.whole.x += ix;
        this->i.start.whole.y += iy;
        this->i.start.whole.z += iz;
        
        dx += this->i.start.fract.x;
        ix = (int64_t)dx;
        dx -= (double)ix;
        this->i.start.fract.x = dx;
        this->i.start.whole.x += ix;
        
        dy += this->i.start.fract.y;
        iy = (int64_t)dy;
        dy -= (double)iy;
        this->i.start.fract.y = dy;
        this->i.start.whole.y += iy;
        
        dz += this->i.start.fract.z;
        iz = (int64_t)dz;
        dz -= (double)iz;
        this->i.start.fract.z = dz;
        this->i.start.whole.z += iz;

        this->i.start.bReverse = this->i.end.bReverse;
        this->i.start.t = t_epoch_now;
        
        this->i.end = this->i.start;
    }

    //get pos difference
    void dpposition::getDiff( double *dx, double *dy, double *dz )
    {
        *dx = (double)( (int64_t)( this->i.end.whole.x - this->i.start.whole.x ) ) + (double)( this->i.end.fract.x - this->i.start.fract.x );
        *dy = (double)( (int64_t)( this->i.end.whole.y - this->i.start.whole.y ) ) + (double)( this->i.end.fract.y - this->i.start.fract.y );
        *dz = (double)( (int64_t)( this->i.end.whole.z - this->i.start.whole.z ) ) + (double)( this->i.end.fract.z - this->i.start.fract.z );
    }
    
    //get pos difference with time
    void dpposition::getDiff( double *dx, double *dy, double *dz, uint64_t t_now )
    {
        double td, tt;
        
        this->getDiff( dx, dy, dz );
    
        tt = (double)( (uint64_t)( this->i.end.t - this->i.start.t ) );
        td = (double)( (uint64_t)( t_now - this->i.start.t ) );
        if( tt > td && tt > 0 )
            td = td / tt;
        else
            td = 1;

        *dx *= td;
        *dy *= td;
        *dz *= td;
    }
    
    //get pos difference
    void dpposition::getDiff( double *dx, double *dy, double *dz, dpposition *arg )
    {
        *dx = (double)( (int64_t)( arg->i.start.whole.x - this->i.start.whole.x ) ) + (double)( arg->i.start.fract.x - this->i.start.fract.x );
        *dy = (double)( (int64_t)( arg->i.start.whole.y - this->i.start.whole.y ) ) + (double)( arg->i.start.fract.y - this->i.start.fract.y );
        *dz = (double)( (int64_t)( arg->i.start.whole.z - this->i.start.whole.z ) ) + (double)( arg->i.start.fract.z - this->i.start.fract.z );
    }
    
};
