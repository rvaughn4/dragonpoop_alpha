
#ifndef dragonpoop_render_api_commandlist_readlock_h
#define dragonpoop_render_api_commandlist_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api_commandlist;
    
    class render_api_commandlist_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api_commandlist *t;
        
    protected:
        
        //ctor
        render_api_commandlist_readlock( render_api_commandlist *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_commandlist_readlock( void );
        
    public:
        
        
        friend class render_api_commandlist;
    };
    
};

#endif