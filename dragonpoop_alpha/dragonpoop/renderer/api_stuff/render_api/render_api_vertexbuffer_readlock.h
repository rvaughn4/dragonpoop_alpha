
#ifndef dragonpoop_render_api_vertexbuffer_readlock_h
#define dragonpoop_render_api_vertexbuffer_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api_vertexbuffer;
    
    class render_api_vertexbuffer_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api_vertexbuffer *t;
        
    protected:
        
        //ctor
        render_api_vertexbuffer_readlock( render_api_vertexbuffer *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_vertexbuffer_readlock( void );
        
    public:
        
        
        friend class render_api_vertexbuffer;
    };
    
};

#endif