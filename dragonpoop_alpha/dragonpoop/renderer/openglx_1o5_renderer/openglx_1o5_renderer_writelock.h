
#ifndef dragonpoop_openglx_1o5_renderer_writelock_h
#define dragonpoop_openglx_1o5_renderer_writelock_h

#include "../renderer_writelock.h"

namespace dragonpoop
{
    class openglx_1o5_renderer;

    class openglx_1o5_renderer_writelock : public renderer_writelock
    {

    private:

        openglx_1o5_renderer *t;

    protected:

        //ctor
        openglx_1o5_renderer_writelock( openglx_1o5_renderer *t, dpmutex_writelock *l );

    public:

        //dtor
        virtual ~openglx_1o5_renderer_writelock( void );

        friend class openglx_1o5_renderer;
    };
    
};

#endif