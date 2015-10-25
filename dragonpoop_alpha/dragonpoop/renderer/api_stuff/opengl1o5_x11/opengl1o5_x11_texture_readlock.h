
#ifndef dragonpoop_opengl1o5_x11_texture_readlock_h
#define dragonpoop_opengl1o5_x11_texture_readlock_h

#include "../render_api/render_api_texture_readlock.h"

namespace dragonpoop
{

    class opengl1o5_x11_texture;
    
    class opengl1o5_x11_texture_readlock : public render_api_texture_readlock
    {
        
    private:
        
        opengl1o5_x11_texture *t;
        
    protected:
        
        //ctor
        opengl1o5_x11_texture_readlock( opengl1o5_x11_texture *t, dpmutex_readlock *l );
        //dtor
        virtual ~opengl1o5_x11_texture_readlock( void );
        
    public:
        
        //return tex
        unsigned int getTex( void );
        
        friend class opengl1o5_x11_texture;
    };
    
};

#endif