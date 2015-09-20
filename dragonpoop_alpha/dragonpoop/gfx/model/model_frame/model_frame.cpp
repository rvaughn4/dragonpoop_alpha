
#include "model_frame.h"
#include "../../../core/dpbuffer/dpbuffer.h"
#include "../model_writelock.h"
#include "../model_frame_joint/model_frame_joint.h"
#include <list>

namespace dragonpoop
{
    
    //ctor
    model_frame::model_frame( dpid id ) : model_component( id, model_component_type_frame )
    {
        this->w = 0;
    }
    
    //dtor
    model_frame::~model_frame( void )
    {
        
    }
    
    //write data to disk/memory
    bool model_frame::writeData( dpbuffer *b )
    {
        model_frame_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }

    //read data from disk/memory
    bool model_frame::readData( dpbuffer *b )
    {
        model_frame_header_v1 h;
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
        
        return 1;
    }
    
    //get weight
    float model_frame::getWeight( void )
    {
        return this->w;
    }
    
    //compute weight
    void model_frame::computeWeight( model_writelock *ml )
    {
        std::list<model_frame_joint *> l;
        std::list<model_frame_joint *>::iterator i;
        model_frame_joint *p;
        float w;
        
        ml->getFrameJoints( &l, this->getId() );
        this->w = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            w = p->getWeight( ml );
            if( w > this->w )
                this->w = w;
        }
    }
    
};
