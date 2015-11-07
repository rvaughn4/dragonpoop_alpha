
#ifndef dragonpoop_renderer_readlock_h
#define dragonpoop_renderer_readlock_h

#include "../core/shared_obj/shared_obj_readlock.h"
#include <string>

namespace dragonpoop
{
    class renderer;
    class core;
    class dpposition;
    struct dpxyz_f;

    class renderer_readlock : public shared_obj_readlock
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_readlock( renderer *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_readlock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //returns fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsPerFrame( void );
        //get renderer name
        void getName( std::string *s );
        //get texture memory used
        unsigned int getTextureMemory( void );
        //get vertex memory used
        unsigned int getVertexMemory( void );
        //gets camera position
        void getCameraPosition( dpposition *p );
        //get position difference from camera
        void getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz );

        friend class renderer;
    };
    
};

#endif