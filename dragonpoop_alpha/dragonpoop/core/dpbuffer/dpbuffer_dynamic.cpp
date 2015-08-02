
#include "dpbuffer_dynamic.h"
#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{
    
    //ctor
    dpbuffer_dynamic::dpbuffer_dynamic( void ) : dpbuffer( 0 )
    {
        this->buffer = 0;
        this->size = 0;
    }
    
    //dtor
    dpbuffer_dynamic::~dpbuffer_dynamic( void )
    {
        if( this->buffer )
            free( this->buffer );
    }
    
    //returns pointer to internal buffer
    char *dpbuffer_dynamic::getInternalBuffer( void )
    {
        return this->buffer;
    }
    
    //returns internal buffer size
    unsigned int dpbuffer_dynamic::getInternalBufferSize( void )
    {
        return this->size;
    }
    
    //resizes internal buffer if necessary
    bool dpbuffer_dynamic::autoResizeInternalBuffer( unsigned int sz )
    {
        unsigned int n_size;
        char *n_buffer;
        
        if( this->buffer && sz <= this->size )
            return 1;
        
        n_size = size + 1;
        n_size *= 2;
        if( n_size < sz )
            n_size = sz + 1;
        
        n_buffer = (char *)malloc( n_size );
        if( !n_buffer )
            return 0;
        
        if( this->buffer )
        {
            memcpy( n_buffer, this->buffer, this->size );
            free( this->buffer );
        }
        
        this->buffer = n_buffer;
        this->size = n_size;
        return 1;
    }

};
