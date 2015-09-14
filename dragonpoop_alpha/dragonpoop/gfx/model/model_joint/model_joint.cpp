
#include "model_joint.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_joint::model_joint( dpid id ) : model_component( id, model_component_type_joint )
    {
        dpid_zero( &this->parent_id );
    }
    
    //dtor
    model_joint::~model_joint( void )
    {
        
    }
    
    //set position
    void model_joint::setPosition( dpxyz_f *x )
    {
        this->pos = *x;
    }
    
    //get position
    void model_joint::getPosition( dpxyz_f *x )
    {
        *x = this->pos;
    }

    //set rotation
    void model_joint::setRotation( dpxyz_f *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_joint::getRotation( dpxyz_f *x )
    {
        *x = this->rot;
    }
    
    //set parent
    void model_joint::setParent( dpid id )
    {
        this->parent_id = id;
    }
    
    //get parent
    dpid model_joint::getParent( void )
    {
        return this->parent_id;
    }
    
    //write data to disk/memory
    bool model_joint::writeData( dpbuffer *b )
    {
        model_joint_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.parent_id = this->parent_id;
        h.pos = this->pos;
        h.rot = this->rot;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
};
