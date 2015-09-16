
#include "model_group.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{

    //ctor
    model_group::model_group( dpid id ) : model_component( id, model_component_type_group )
    {
        dpid_zero( &this->m_id );
    }

    //dtor
    model_group::~model_group( void )
    {

    }

    //set material id
    void model_group::setMaterialId( dpid id )
    {
        this->m_id = id;
    }

    //get material id
    dpid model_group::getMaterialId( void )
    {
        return this->m_id;
    }
    
    //write data to disk/memory
    bool model_group::writeData( dpbuffer *b )
    {
        model_group_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.m_id = this->m_id;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
    //read data from disk/memory
    bool model_group::readData( dpbuffer *b )
    {
        model_group_header_v1 h;
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
        
        this->m_id = h.m_id;
        
        return 1;
    }

};
