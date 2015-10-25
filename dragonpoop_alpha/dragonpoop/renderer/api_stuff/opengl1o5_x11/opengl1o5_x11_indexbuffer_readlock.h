
#ifndef dragonpoop_opengl1o5_x11_indexbuffer_readlock_h
#define dragonpoop_opengl1o5_x11_indexbuffer_readlock_h

#include "../render_api/render_api_indexbuffer_readlock.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_indexbuffer;
    class dpindex_buffer;
    
    class opengl1o5_x11_indexbuffer_readlock : public render_api_indexbuffer_readlock
    {
        
    private:
        
        opengl1o5_x11_indexbuffer *t;
        
    protected:
        
        //ctor
        opengl1o5_x11_indexbuffer_readlock( opengl1o5_x11_indexbuffer *t, dpmutex_readlock *l );
        //dtor
        virtual ~opengl1o5_x11_indexbuffer_readlock( void );
        
    public:
        
        //return vb
        dpindex_buffer *getBuffer( void );
        
        friend class opengl1o5_x11_indexbuffer;
    };
    
};

#endif