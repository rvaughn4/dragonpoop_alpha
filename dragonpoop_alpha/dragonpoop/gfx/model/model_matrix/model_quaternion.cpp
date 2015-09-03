
#include "model_quaternion.h"
#include "../../dpvertex/dpxyz_f.h"
#include <math.h>

namespace dragonpoop
{
    
    //ctor
    model_quaternion::model_quaternion( void )
    {
        this->setIdentity();
    }
    
    //dtor
    model_quaternion::~model_quaternion( void )
    {
        
    }
    
    //get raw data
    model_quaternion_f *model_quaternion::getData( void )
    {
        return &this->f;
    }
    
    //set identity
    void model_quaternion::setIdentity( void )
    {
        this->f.sv.x = 0;
        this->f.sv.y = 0;
        this->f.sv.z = 0;
        this->f.sv.w = 0;
    }
    
    //set position
    void model_quaternion::setPosition( float x, float y, float z, float w )
    {
        this->f.sv.x = x;
        this->f.sv.y = y;
        this->f.sv.z = z;
        this->f.sv.w = w;
    }
    
    //set angle
    void model_quaternion::setAngle( float x, float y, float z )
    {
        float		angle;
        float		sr, sp, sy, cr, cp, cy;
        model_quaternion_f *quaternion;
        
        quaternion = &this->f;
        
        // FIXME: rescale the inputs to 1/2 angle
        angle = z * 0.5;
        sy = sin(angle);
        cy = cos(angle);
        angle = y * 0.5;
        sp = sin(angle);
        cp = cos(angle);
        angle = x * 0.5;
        sr = sin(angle);
        cr = cos(angle);
        
        quaternion->fv[0] = sr*cp*cy-cr*sp*sy; // X
        quaternion->fv[1] = cr*sp*cy+sr*cp*sy; // Y
        quaternion->fv[2] = cr*cp*sy-sr*sp*cy; // Z
        quaternion->fv[3] = cr*cp*cy+sr*sp*sy; // W
    }
    
    //set angle
    void model_quaternion::setAngle( dpxyz_f *x )
    {
        this->setAngle( x->x, x->y, x->z );
    }
    
    void model_quaternion::slerp( model_quaternion *p, model_quaternion *q, float t )
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
