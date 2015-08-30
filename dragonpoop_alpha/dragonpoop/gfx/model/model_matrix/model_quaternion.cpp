
#include "model_quaternion.h"

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
    
};
