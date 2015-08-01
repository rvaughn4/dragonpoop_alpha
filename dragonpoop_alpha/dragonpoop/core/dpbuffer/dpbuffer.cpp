
#include "dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    dpbuffer::dpbuffer( void )
    {
        
    }
    
    //dtor
    dpbuffer::~dpbuffer( void )
    {
        
    }
    
    //get pointer to buffer
    char *dpbuffer::getBuffer( void )
    {
        return this->getInternalBuffer();
    }
    
    //get buffer size in bytes
    unsigned int dpbuffer::getSize( void )
    {
        unsigned int r, s;
        
        r = this->size;
        s = this->getInternalBufferSize();
        if( r > s )
            r = s;
        
        return r;
    }
    
    //get read byte cursor
    unsigned int dpbuffer::getReadCursor( void )
    {
        unsigned int r, s;
        
        r = this->read_cursor;
        s = this->getInternalBufferSize();
        if( r > s )
            r = s;
        
        return r;
    }
    
    //get write byte cursor
    unsigned int dpbuffer::getWriteCursor( void )
    {
        unsigned int r, s;
        
        r = this->write_cursor;
        s = this->getInternalBufferSize();
        if( r > s )
            r = s;
        
        return r;
    }
    
    //set read byte cursor
    void dpbuffer::setReadCursor( unsigned int c )
    {
        unsigned int r, s;
        
        r = this->size;
        s = this->getInternalBufferSize();
        if( r > s )
            r = s;
        if( c > r )
            c = r;
        
        this->read_cursor = c;
    }
    
    //set write byte cursor
    void dpbuffer::setWriteCursor( unsigned int c )
    {
        unsigned int r, s;
        
        r = this->size;
        s = this->getInternalBufferSize();
        if( r > s )
            r = s;
        if( c > r )
            c = r;
        
        this->read_cursor = c;
    }
    
    //read 1 byte
    bool dpbuffer::readByte( uint8_t *b )
    {
        uint8_t *p;
        unsigned int sz;
        
        p = (uint8_t *)this->getInternalBuffer();
        sz = this->getSize();
        
        if( !p || this->read_cursor >= sz )
            return 0;
        
        *b = p[ this->read_cursor ];
        this->read_cursor++;
        
        return 1;
    }
    
    //write 1 byte
    bool dpbuffer::writeByte( uint8_t *b )
    {
        uint8_t *p;
        unsigned int sz;
        
        if( !this->autoResizeInternalBuffer( this->write_cursor + 1 ) )
            return 0;
        
        p = (uint8_t *)this->getInternalBuffer();
        sz = this->getInternalBufferSize();
        
        if( !p || this->write_cursor >= sz )
            return 0;
        
        p[ this->write_cursor ] = *b;
        this->write_cursor++;
        if( this->write_cursor > this->size )
            this->size = this->write_cursor;
        
        return 1;
    }
    
    //read multiple bytes, returns size read
    unsigned int dpbuffer::readBytes( uint8_t *b, unsigned int size )
    {
        unsigned int r;
        
        for( r = 0; r < size; r++ )
        {
            if( !this->readByte( &b[ r ] ) )
                return r;
        }
        
        return r;
    }
    
    //write multiple bytes, returns size written
    bool dpbuffer::writeBytes( uint8_t *b, unsigned int size )
    {
        unsigned int r;
        
        for( r = 0; r < size; r++ )
        {
            if( !this->writeByte( &b[ r ] ) )
                return r;
        }
        
        return r;
    }
    
    //resets cursors and size to 0
    void dpbuffer::clear( void )
    {
        this->size = this->read_cursor = this->write_cursor = 0;
    }

};
