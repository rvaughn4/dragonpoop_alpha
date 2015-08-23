
#include "model_instance_vertex.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_vertex::model_instance_vertex( model_vertex *v ) : model_component( v->getId(), model_component_type_vertex )
    {
        v->getPosition( &this->pos );
        memset( &this->start, 0, sizeof( this->start ) );
        this->end = this->start;
    }
    
    //dtor
    model_instance_vertex::~model_instance_vertex( void )
    {
        
    }
    
    //get position
    void model_instance_vertex::getPosition( dpxyzw *x )
    {
        *x = this->pos;
    }
    
    //set position
    void model_instance_vertex::setPosition( dpxyzw *x )
    {
        this->pos = *x;
    }
    
    //set start position
    void model_instance_vertex::setStartPosition( dpxyzw *x )
    {
        this->start.pos = *x;
    }
    
    //get start position
    void model_instance_vertex::getStartPosition( dpxyzw *x )
    {
        *x = this->start.pos;
    }
    
    //get start time
    uint64_t model_instance_vertex::getStartTime( void )
    {
        return this->start.t;
    }
    
    //set start time
    void model_instance_vertex::setStartTime( uint64_t t )
    {
        this->start.t = t;
    }
    
    //set end position
    void model_instance_vertex::setEndPosition( dpxyzw *x )
    {
        this->end.pos = *x;
    }
    
    //get end position
    void model_instance_vertex::getEndPosition( dpxyzw *x )
    {
        *x = this->end.pos;
    }
    
    //get end time
    uint64_t model_instance_vertex::getEndTime( void )
    {
        return this->end.t;
    }
    
    //set end time
    void model_instance_vertex::setEndTime( uint64_t t )
    {
        this->end.t = t;
    }
    
};
