
#include "model_triangle.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{

    //ctor
    model_triangle::model_triangle( dpid id ) : model_component( id, model_component_type_triangle )
    {

    }

    //dtor
    model_triangle::~model_triangle( void )
    {

    }

    //set vertex 0 id
    void model_triangle::setVertex0( dpid id )
    {
        this->v0 = id;
    }

    //set vertex 1 id
    void model_triangle::setVertex1( dpid id )
    {
        this->v1 = id;
    }

    //set vertex 2 id
    void model_triangle::setVertex2( dpid id )
    {
        this->v2 = id;
    }

    //get vertex 0 id
    dpid model_triangle::getVertex0( void )
    {
        return this->v0;
    }

    //get vertex 1 id
    dpid model_triangle::getVertex1( void )
    {
        return this->v1;
    }

    //get vertex 2 id
    dpid model_triangle::getVertex2( void )
    {
        return this->v2;
    }
    
    //write data to disk/memory
    bool model_triangle::writeData( dpbuffer *b )
    {
        model_triangle_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.v0 = this->v0;
        h.v1 = this->v1;
        h.v2 = this->v2;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }

    //read data from disk/memory
    bool model_triangle::readData( dpbuffer *b )
    {
        model_triangle_header_v1 h;
        unsigned int rc;
        
        rc = b->getReadCursor();
        if( !b->readBytes( (uint8_t *)&h, sizeof( h.h ) ) )
            return 0;
        b->setReadCursor( rc );
        
        if( h.h.version < 1 || h.h.size < sizeof( h ) )
            return 0;
        if( !b->readBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        b->setReadCursor( rc + h.h.size );

        this->v0 = h.v0;
        this->v1 = h.v1;
        this->v2 = h.v2;
        
        return 1;
    }
    
};
