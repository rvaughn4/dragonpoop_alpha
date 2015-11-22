
#ifndef dragonpoop_gfx_readlock_h
#define dragonpoop_gfx_readlock_h

#include "../core/shared_obj/shared_obj_readlock.h"
#include <list>
#include "../core/dpid/dpid.h"

namespace dragonpoop
{
    class gfx;
    class core;
    class model_ref;
    class model;
    class gui_ref;
    class renderer_ref;
    class dpposition_share_ref;
    class dpheight_cache_ref;
    class model_man_writelock;
    class model_man_readlock;
    class model_man_ref;
    class shared_obj_guard;
    class gui_man_ref;
    class gui_man_readlock;
    class gui_man_writelock;
    class dpactor_man_ref;
    class dpactor_man_readlock;
    class dpactor_man_writelock;
    class dpland_man_ref;
    class dpland_man_readlock;
    class dpland_man_writelock;
    class dpsky_man_ref;
    class dpsky_man_readlock;
    class dpsky_man_writelock;

    class gfx_readlock : public shared_obj_readlock
    {

    private:

        gfx *t;

    protected:

        //ctor
        gfx_readlock( gfx *t, dpmutex_readlock *l );
        //dtor
        virtual ~gfx_readlock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //return fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsEachFrame( void );
        //return renderer
        renderer_ref *getRenderer( void );
        //get camera position
        dpposition_share_ref *getCameraPosition( void );
        //get height cache
        dpheight_cache_ref *getHeights( void );
        //get models
        bool getModels( model_man_ref **r );
        //get models
        bool getModels( model_man_readlock **r, shared_obj_guard *o );
        //get models
        bool getModels( model_man_writelock **r, shared_obj_guard *o );
        //get guis
        bool getGuis( gui_man_ref **r );
        //get guis
        bool getGuis( gui_man_readlock **r, shared_obj_guard *o );
        //get guis
        bool getGuis( gui_man_writelock **r, shared_obj_guard *o );
        //get actors
        bool getActors( dpactor_man_ref **r );
        //get actors
        bool getActors( dpactor_man_readlock **r, shared_obj_guard *o );
        //get actors
        bool getActors( dpactor_man_writelock **r, shared_obj_guard *o );
        //get land
        bool getLand( dpland_man_ref **r );
        //get land
        bool getLand( dpland_man_readlock **r, shared_obj_guard *o );
        //get land
        bool getLand( dpland_man_writelock **r, shared_obj_guard *o );
        //get sky
        bool getSky( dpsky_man_ref **r );
        //get sky
        bool getSky( dpsky_man_readlock **r, shared_obj_guard *o );
        //get sky
        bool getSky( dpsky_man_writelock **r, shared_obj_guard *o );

        friend class gfx;
    };

};

#endif
