
#ifndef dragonpoop_render_api_indexbuffer_readlock_h
#define dragonpoop_render_api_indexbuffer_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api_indexbuffer;
    
    class render_api_indexbuffer_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api_indexbuffer *t;
        
    protected:
        
        //ctor
        render_api_indexbuffer_readlock( render_api_indexbuffer *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_indexbuffer_readlock( void );
        
    public:
        
        
        friend class render_api_indexbuffer;
    };
    
};

#endif