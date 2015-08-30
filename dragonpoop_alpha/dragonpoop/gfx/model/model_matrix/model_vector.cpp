
#include "model_vector.h"

namespace dragonpoop
{
    
    //ctor
    model_vector::model_vector( void )
    {
        this->setIdentity();
    }
    
    //dtor
    model_vector::~model_vector( void )
    {
        
    }
    
    //get raw data
    model_vector_f *model_vector::getData( void )
    {
        return &this->f;
    }
    
    //set identity
    void model_vector::setIdentity( void )
    {
        this->f.sv.x = 0;
        this->f.sv.y = 0;
        this->f.sv.z = 0;
    }
    
    //set position
    void model_vector::setPosition( float x, float y, float z )
    {
        this->f.sv.x = x;
        this->f.sv.y = y;
        this->f.sv.z = z;
    }
    
    //set position
    void model_vector::setPosition( dpxyz_f *p )
    {
        this->f.sv.x = p->x;
        this->f.sv.y = p->y;
        this->f.sv.z = p->z;
    }
    
    //get position
    void model_vector::getPosition( dpxyz_f *p )
    {
        p->x = this->f.sv.x;
        p->y = this->f.sv.y;
        p->z = this->f.sv.z;
    }
    
};
