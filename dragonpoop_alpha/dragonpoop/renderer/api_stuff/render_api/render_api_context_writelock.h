
#ifndef dragonpoop_render_api_context_writelock_h
#define dragonpoop_render_api_context_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_context;
    class render_api_commandlist_ref;
    
    class render_api_context_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api_context *t;
        
    protected:
        
        //ctor
        render_api_context_writelock( render_api_context *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_context_writelock( void );
        
    public:
        
        //make commandlist
        render_api_commandlist_ref *makeCmdList( void );
        //delete commandlists
        void deleteCmdLists( void );
        //clear screen
        void clearColor( float r, float g, float b );
        //clear depth
        void clearDepth( float f );
        //present backbuffer
        void flipBackBuffer( void );
        //make context active in thread
        void makeActive( void );
        //set viewport
        void setViewport( float w, float h );

        friend class render_api_context;
    };
    
};

#endif