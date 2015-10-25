
#ifndef dragonpoop_opengl1o5_x11_indexbuffer_h
#define dragonpoop_opengl1o5_x11_indexbuffer_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_indexbuffer.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_indexbuffer : public render_api_indexbuffer
    {
        
    private:
        
        dpindex_buffer vb;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //return vb
        dpindex_buffer *getBuffer( void );
        
    public:
        
        //ctor
        opengl1o5_x11_indexbuffer( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpindex_buffer *vb );
        //dtor
        virtual ~opengl1o5_x11_indexbuffer( void );
        
        friend class opengl1o5_x11_indexbuffer_readlock;
    };
    
};

#endif