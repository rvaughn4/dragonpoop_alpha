
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
    
};
