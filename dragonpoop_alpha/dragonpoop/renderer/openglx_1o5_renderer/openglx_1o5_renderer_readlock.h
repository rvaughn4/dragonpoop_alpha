
#ifndef dragonpoop_openglx_1o5_renderer_readlock_h
#define dragonpoop_openglx_1o5_renderer_readlock_h

#include "../renderer_readlock.h"

namespace dragonpoop
{
    class openglx_1o5_renderer;

    class openglx_1o5_renderer_readlock : public renderer_readlock
    {

    private:

        openglx_1o5_renderer *t;

    protected:

        //ctor
        openglx_1o5_renderer_readlock( openglx_1o5_renderer *t, dpmutex_readlock *l );

    public:

        //dtor
        virtual ~openglx_1o5_renderer_readlock( void );

        friend class openglx_1o5_renderer;
    };
    
};

#endif