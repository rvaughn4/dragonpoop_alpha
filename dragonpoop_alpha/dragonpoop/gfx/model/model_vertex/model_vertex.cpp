
#include "model_vertex.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{

    //ctor
    model_vertex::model_vertex( dpid id ) : model_component( id, model_component_type_vertex )
    {

    }

    //dtor
    model_vertex::~model_vertex( void )
    {

    }

    //get position
    void model_vertex::getPosition( dpxyz_f *x )
    {
        *x = this->x;
    }

    //set position
    void model_vertex::setPosition( dpxyz_f *x )
    {
        this->x = *x;
    }

    //set position
    void model_vertex::setPosition( model_vector *v )
    {
        v->getPosition( &this->x );
    }
    
    //get position
    void model_vertex::getPosition( model_vector *v )
    {
        v->setPosition( &this->x );
    }
    
    //write data to disk/memory
    bool model_vertex::writeData( dpbuffer *b )
    {
        model_vertex_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.pos = this->x;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
};
