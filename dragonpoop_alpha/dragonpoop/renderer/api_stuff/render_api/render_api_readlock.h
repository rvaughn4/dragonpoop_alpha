
#ifndef dragonpoop_render_api_readlock_h
#define dragonpoop_render_api_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api;
    
    class render_api_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api *t;
        
    protected:
        
        //ctor
        render_api_readlock( render_api *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_readlock( void );
        
    public:
        
        //returns true if window is open
        bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        //returns true if mouse input is waiting
        bool hasMouseInput( void );

        friend class render_api;
    };
    
};

#endif