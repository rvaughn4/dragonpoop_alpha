
#include "model_instance_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_joint::model_instance_joint( model_joint *j ) : model_component( j->getId(), model_component_type_joint )
    {
        std::string s;
        
        j->getPosition( &this->pos );
        j->getRotation( &this->rot );
        
        j->getName( &s );
        this->setName( &s );
    }
    
    //dtor
    model_instance_joint::~model_instance_joint( void )
    {
        
    }
    
    //set position
    void model_instance_joint::setPosition( dpxyzw *x )
    {
        this->pos = *x;
    }
    
    //get position
    void model_instance_joint::getPosition( dpxyzw *x )
    {
        *x = this->pos;
    }
    
    //set rotation
    void model_instance_joint::setRotation( dpxyzw *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_instance_joint::getRotation( dpxyzw *x )
    {
        *x = this->rot;
    }
    
};
