
#include "dpbuffer_static.h"

namespace dragonpoop
{
    
    //ctor
    dpbuffer_static::dpbuffer_static( char *p, unsigned int sz )
    {
        this->buffer = p;
        this->size = sz;
    }
    
    //dtor
    dpbuffer_static::~dpbuffer_static( void )
    {
        
    }
    
    //returns pointer to internal buffer
    char *dpbuffer_static::getInternalBuffer( void )
    {
        return this->buffer;
    }
    
    //returns internal buffer size
    unsigned int dpbuffer_static::getInternalBufferSize( void )
    {
        return this->size;
    }
    
    //resizes internal buffer if necessary
    bool dpbuffer_static::autoResizeInternalBuffer( unsigned int sz )
    {
        if( sz <= this->size )
            return 1;
        return 0;
    }

};
