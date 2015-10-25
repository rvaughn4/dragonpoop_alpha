
#ifndef dragonpoop_opengl1o5_x11_texture_h
#define dragonpoop_opengl1o5_x11_texture_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_texture.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_texture : public render_api_texture
    {
        
    private:
        
        unsigned int glTex;
        
        //make texture
        void makeTex( dpbitmap *bm );
        //delete texture
        void deleteTex( void );
        
    protected:
        
    public:
        
        //ctor
        opengl1o5_x11_texture( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpbitmap *bm );
        //dtor
        virtual ~opengl1o5_x11_texture( void );

    };
    
};

#endif