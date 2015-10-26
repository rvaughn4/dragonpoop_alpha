
#ifndef dragonpoop_renderer_writelock_h
#define dragonpoop_renderer_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"
#include <list>
#include <string>
#include "../core/dpid/dpid.h"

namespace dragonpoop
{
    class renderer;
    class core;
    class dpthread_lock;
    class dptask_writelock;
    class gfx_writelock;
    class renderer_writelock;
    class renderer_model_readlock;
    class renderer_model_instance_readlock;
    class renderer_model_instance_group;
    class renderer_model_material;
    class dpmatrix;
    class renderer_gui_readlock;
    class renderer_gui;
    class dpposition;
    struct dpxyz_f;
    class render_api_commandlist_ref;

    class renderer_writelock : public shared_obj_writelock
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_writelock( renderer *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_writelock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //stop renderer task
        void kill( void );
        //run renderer from task
        void run( dptask_writelock *tskl, dpthread_lock *thd );
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
        //sync camera position
        void syncCamera( void );
        //get position difference from camera
        void getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz );
        //returns true if gui commandlist is set
        bool isGuiCommandListUploaded( void );
        //set gui commandlist
        void uploadGuiCommandList( render_api_commandlist_ref *cl );
        //returns true if model commandlist is set
        bool isModelCommandListUploaded( void );
        //set model commandlist
        void uploadModelCommandList( render_api_commandlist_ref *cl );

        friend class renderer;
    };
    
};

#endif