
#ifndef dragonpoop_render_api_vertexbuffer_writelock_h
#define dragonpoop_render_api_vertexbuffer_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_vertexbuffer;
    
    class render_api_vertexbuffer_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api_vertexbuffer *t;
        
    protected:
        
        //ctor
        render_api_vertexbuffer_writelock( render_api_vertexbuffer *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_vertexbuffer_writelock( void );
        
    public:
        
        
        friend class render_api_vertexbuffer;
    };
    
};

#endif