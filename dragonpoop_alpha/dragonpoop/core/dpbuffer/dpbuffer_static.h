
#ifndef dragonpoop_dpbuffer_static_h
#define dragonpoop_dpbuffer_static_h

#include "dpbuffer.h"

namespace dragonpoop
{
    
    class dpbuffer_static : public dpbuffer
    {
        
    private:
        
        unsigned int size;
        char *buffer;
        
    protected:
        
        //returns pointer to internal buffer
        virtual char *getInternalBuffer( void );
        //returns internal buffer size
        virtual unsigned int getInternalBufferSize( void );
        //resizes internal buffer if necessary
        virtual bool autoResizeInternalBuffer( unsigned int sz );
        
    public:
        
        //ctor
        dpbuffer_static( char *p, unsigned int sz );
        //dtor
        virtual ~dpbuffer_static( void );
        
    };
    
};

#endif