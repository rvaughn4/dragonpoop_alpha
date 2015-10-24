
#ifndef dragonpoop_opengl1o5_x11_context_h
#define dragonpoop_opengl1o5_x11_context_h

#include "opengl1o5_x11.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_context : public render_api_context
    {
        
    private:
        
        GLXContext ctx;
        Window win;
        Display *dpy;
        
    protected:
        
        //clear screen
        virtual void clearColor( float r, float g, float b );
        //clear depth
        virtual void clearDepth( float f );
        //present backbuffer
        virtual void flipBackBuffer( void );
        //make context active in thread
        virtual void makeActive( void );
        //set viewport
        virtual void setViewport( float w, float h );
        
    public:
        
        //ctor
        opengl1o5_x11_context( render_api_writelock *r, dpmutex_master *mm, GLXContext ctx, Window win, Display *dpy );
        //dtor
        virtual ~opengl1o5_x11_context( void );
        
    };
    
};

#endif