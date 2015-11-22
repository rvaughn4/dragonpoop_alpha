
#include "dpposition.h"
#include "../dpvertex/dpxyz_f.h"
#include "../dpmatrix/dpquaternion.h"
#include <string>
#include <string.h>
#include <math.h>

namespace dragonpoop
{

    //ctor
    dpposition::dpposition( void )
    {
        memset( &this->i, 0, sizeof( this->i ) );
        this->old_rot.x = this->old_rot.y = this->old_rot.z = 0;
        this->last_rot = this->old_rot;
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
        dpposition_inner_half h, h1;

        this->getPosition( &h, tm_epoch_now );
        arg->getPosition( &h1, tm_epoch_now );

        pout->x = h1.x - h.x;
        pout->y = h1.y - h.y;
        pout->z = h1.z - h.z;
    }

    //set location
    void dpposition::setLocation( dpposition *arg, uint64_t tm_epoch_now, uint64_t tm_epoch_when )
    {
        dpposition_inner_half h;

        this->stop( tm_epoch_now );
        arg->getPosition( &h, tm_epoch_now );

        this->i.end.x += h.x;
        this->i.end.y += h.y;
        this->i.end.z += h.z;
        this->i.end.t = tm_epoch_when;
        this->i.end.bReverse = arg->i.end.bReverse;
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
        double dt;

        if( this->i.end.t <= this->i.start.t )
            return 0;
        dt = this->i.end.t - this->i.start.t;

        pvel->x = (double)( ( this->i.end.x - this->i.start.x ) / dt );
        pvel->y = (double)( ( this->i.end.y - this->i.start.y ) / dt );
        pvel->z = (double)( ( this->i.end.z - this->i.start.z ) / dt );

        dt = pvel->x * pvel->x + pvel->y * pvel->y + pvel->z * pvel->z;
        if( dt > 0 )
            dt = sqrt( dt );
        else
            dt = 0;

        return (float)dt;
    }

    //get rotational direction
    void dpposition::getDirection( dpxyz_f *pout, uint64_t t_now )
    {
        dpxyz_f v;
        float tt, td;
        dpquaternion qa, qb, qc;

        v.x = this->i.end.x - this->i.start.x;
        v.y = this->i.end.y - this->i.start.y;
        v.z = this->i.end.z - this->i.start.z;

        *pout = this->old_rot;
        pout->x = 0;
        if( this->i.end.t < t_now )
        {
            this->last_rot = this->old_rot;
            return;
        }

        if( abs( v.x ) + abs( v.z ) > 0.1f )
            pout->y = atan2( -v.x, -v.z );
        if( abs( v.y ) > 0.1f )
        {
            if( abs( v.x ) < 0.1f )
                pout->y = 0;
            pout->x = atan2( v.y, -v.z );
        }
        else
            pout->x = 0;

        this->old_rot = *pout;

        tt = (float)( (uint64_t)( t_now - this->i.start.t ) );
        td = (float)( (uint64_t)( this->i.end.t - this->i.start.t ) );
        if( td > 500.0f )
            td = 500.0f;
        if( tt > td )
            tt = 1.0f;
        else
        {
            if( td <= 0.0f )
                tt = 1.0f;
            else
                tt = tt / td;
        }

        if( tt > 0.95f )
        {
            this->last_rot = this->old_rot;
            return;
        }
        qa.setAngle( &this->last_rot );
        qb.setAngle( pout );
        qc.slerp( &qa, &qb, tt );
        qc.getAngle( pout );
    }

    //move position incrementally
    void dpposition::move( dpxyz_f *pin, uint64_t t_epoch_now, uint64_t t_epoch_when, bool doReverse )
    {
        this->stop( t_epoch_now );

        this->i.end.x += pin->x;
        this->i.end.y += pin->y;
        this->i.end.z += pin->z;
        this->i.end.t = t_epoch_when;
        this->i.end.bReverse = doReverse;
    }

    //stop moving
    void dpposition::stop( uint64_t t_epoch_now )
    {
        dpposition_inner_half x;

        this->getPosition( &x, t_epoch_now );
        this->i.end = x;
        this->i.start = x;
    }

    //move immediatly
    void dpposition::move( dpxyz_f *x )
    {
        this->i.start.x = this->i.end.x + x->x;
        this->i.start.y = this->i.end.y + x->y;
        this->i.start.z = this->i.end.z + x->z;
        this->i.end = this->i.start;
    }

    //find current position
    void dpposition::getPosition( dpposition_inner_half *h, uint64_t t_now )
    {
        double td, tt;

        tt = (double)( (uint64_t)( this->i.end.t - this->i.start.t ) );
        td = (double)( (uint64_t)( t_now - this->i.start.t ) );

        if( tt < 0 )
            td = 1.0;
        else
        {
            if( td > tt )
                td = 1.0;
            else
                td = td / tt;
        }
        tt = 1.0 - td;

        h->x = this->i.start.x * tt + this->i.end.x * td;
        h->y = this->i.start.y * tt + this->i.end.y * td;
        h->z = this->i.start.z * tt + this->i.end.z * td;
        h->t = t_now;
        h->bReverse = this->i.end.bReverse;
    }


};
