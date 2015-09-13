
#include "dpquaternion.h"
#include "../dpvertex/dpxyz_f.h"
#include <math.h>
#include "dpmatrix.h"

namespace dragonpoop
{
    
    //ctor
    dpquaternion::dpquaternion( void )
    {
        this->setIdentity();
    }
    
    //dtor
    dpquaternion::~dpquaternion( void )
    {
        
    }
    
    //get raw data
    dpquaternion_f *dpquaternion::getData( void )
    {
        return &this->f;
    }
    
    //set identity
    void dpquaternion::setIdentity( void )
    {
        this->f.sv.x = 0;
        this->f.sv.y = 0;
        this->f.sv.z = 0;
        this->f.sv.w = 0;
    }
    
    //set position
    void dpquaternion::setPosition( float x, float y, float z, float w )
    {
        this->f.sv.x = x;
        this->f.sv.y = y;
        this->f.sv.z = z;
        this->f.sv.w = w;
    }
    
    //set angle
    void dpquaternion::setAngle( float x, float y, float z )
    {
        float		angle;
        float		sr, sp, sy, cr, cp, cy;
        dpquaternion_f *quaternion;
        
        quaternion = &this->f;
        
        angle = z * 0.5;
        sy = sin(angle);
        cy = cos(angle);
        angle = y * 0.5;
        sp = sin(angle);
        cp = cos(angle);
        angle = x * 0.5;
        sr = sin(angle);
        cr = cos(angle);
        
        quaternion->fv[0] = sr*cp*cy-cr*sp*sy;
        quaternion->fv[1] = cr*sp*cy+sr*cp*sy;
        quaternion->fv[2] = cr*cp*sy-sr*sp*cy;
        quaternion->fv[3] = cr*cp*cy+sr*sp*sy;
    }
    
    //set angle
    void dpquaternion::setAngle( dpxyz_f *x )
    {
        this->setAngle( x->x, x->y, x->z );
    }

    //get angle
    void dpquaternion::getAngle( dpxyz_f *x )
    {
        dpmatrix m;
        m.setQuat( this );
        m.getAngles( x );
    }
    
    void dpquaternion::slerp( dpquaternion *p, dpquaternion *q, float t )
    {
        int i;
        float omega, cosom, sinom, sclp, sclq, a ,b;
        
        // decide if one of the quaternions is backwards
        a = b = 0;
        for( i = 0; i < 4; i++ )
        {
            a += ( p->f.fv[i] - q->f.fv[i] ) * ( p->f.fv[i] - q->f.fv[i] );
            b += ( p->f.fv[i] + q->f.fv[i] ) * ( p->f.fv[i] + q->f.fv[i] );
        }
        if( a > b )
        {
            for( i = 0; i < 4; i++ )
            {
                q->f.fv[i] = -q->f.fv[i];
            }
        }
        
        cosom = p->f.fv[0] * q->f.fv[0] + p->f.fv[1] * q->f.fv[1] + p->f.fv[2] * q->f.fv[2] + p->f.fv[3] * q->f.fv[3];
        
        if( ( 1.0 + cosom ) > 0.00000001 )
        {
            if( ( 1.0 - cosom ) > 0.00000001 )
            {
                omega = acos( cosom );
                sinom = sin( omega );
                sclp = sin( ( 1.0 - t ) * omega ) / sinom;
                sclq = sin( t * omega ) / sinom;
            }
            else
            {
                sclp = 1.0 - t;
                sclq = t;
            }
            for( i = 0; i < 4; i++ )
            {
                this->f.fv[i] = sclp * p->f.fv[i] + sclq * q->f.fv[i];
            }
        }
        else
        {
            this->f.fv[0] = -p->f.fv[1];
            this->f.fv[1] = p->f.fv[0];
            this->f.fv[2] = -p->f.fv[3];
            this->f.fv[3] = p->f.fv[2];
            sclp = sin( ( 1.0 - t ) * 0.5 * 3.14f );
            sclq = sin( t * 0.5 * 3.14f );
            for( i = 0; i < 3; i++ )
            {
                this->f.fv[i] = sclp * p->f.fv[i] + sclq * this->f.fv[i];
            }
        }
    }
};
