
#ifndef dragonpoop_render_api_context_readlock_h
#define dragonpoop_render_api_context_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api_context;
    
    class render_api_context_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api_context *t;
        
    protected:
        
        //ctor
        render_api_context_readlock( render_api_context *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_context_readlock( void );
        
    public:
        
        //return width and height
        void getDimensions( float *w, float *h );        
        
        friend class render_api_context;
    };
    
};

#endif