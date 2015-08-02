
#ifndef dragonpoop_dpbuffer_h
#define dragonpoop_dpbuffer_h

#include <stdint.h>

namespace dragonpoop
{
    
    class dpbuffer
    {
        
    private:
        
        unsigned int size, read_cursor, write_cursor;
        
    protected:
        
        //ctor
        dpbuffer( unsigned int sz );
        //returns pointer to internal buffer
        virtual char *getInternalBuffer( void ) = 0;
        //returns internal buffer size
        virtual unsigned int getInternalBufferSize( void ) = 0;
        //resizes internal buffer if necessary
        virtual bool autoResizeInternalBuffer( unsigned int sz ) = 0;
        
    public:
        
        //dtor
        virtual ~dpbuffer( void );
        //get pointer to buffer
        char *getBuffer( void );
        //get buffer size in bytes
        unsigned int getSize( void );
        //get read byte cursor
        unsigned int getReadCursor( void );
        //get write byte cursor
        unsigned int getWriteCursor( void );
        //set read byte cursor
        void setReadCursor( unsigned int c );
        //set write byte cursor
        void setWriteCursor( unsigned int c );
        //read 1 byte
        bool readByte( uint8_t *b );
        //write 1 byte
        bool writeByte( uint8_t *b );
        //read multiple bytes, returns size read
        unsigned int readBytes( uint8_t *b, unsigned int size );
        //write multiple bytes, returns size written
        bool writeBytes( uint8_t *b, unsigned int size );
        //resets cursors and size to 0
        void clear( void );
        
    };
    
};

#endif