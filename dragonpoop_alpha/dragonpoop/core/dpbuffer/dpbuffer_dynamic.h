
#ifndef dragonpoop_dpbuffer_dynamic_h
#define dragonpoop_dpbuffer_dynamic_h

#include "dpbuffer.h"

namespace dragonpoop
{
    
    class dpbuffer_dynamic : public dpbuffer
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
        dpbuffer_dynamic( void );
        //dtor
        virtual ~dpbuffer_dynamic( void );
        
    };
    
};

#endif