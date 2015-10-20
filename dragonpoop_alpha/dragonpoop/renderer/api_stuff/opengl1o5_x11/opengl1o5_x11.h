
#ifndef dragonpoop_opengl1o5_x11_h
#define dragonpoop_opengl1o5_x11_h

#include "../x11_window/x11_window.h"
#include "../render_api/render_api.h"

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace dragonpoop
{
    
    class opengl1o5_x11 : public render_api
    {
        
    private:
        
        x11_window *w;
        Display *dpy;
        int screen;
        Window win;
        GLXContext ctx;
        int x, y;
        unsigned int width, height;
        unsigned int depth;
        bool bIsOpen;

    protected:
        
    public:
        
        
        //ctor
        opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        
    };
    
};

#endif