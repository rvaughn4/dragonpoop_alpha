
#ifndef dragonpoop_gfx_writelock_h
#define dragonpoop_gfx_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"
#include "../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class gfx;
    class core;
    class dpthread_lock;
    class model_ref;
    class model;
    class model_loader_ref;
    class model_instance_ref;
    class model_saver_ref;
    class gui;
    class gui_ref;
    class gui_factory;
    class renderer_ref;
    class dpposition_share_ref;
    class dpheight_cache_ref;
    class dpactor;
    class dpactor_ref;
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
    class renderer_factory;
    class dpland_man_ref;
    class dpland_man_readlock;
    class dpland_man_writelock;
    class dpsky_man_ref;
    class dpsky_man_readlock;
    class dpsky_man_writelock;
    class dptaskpool_ref;

    class gfx_writelock : public shared_obj_writelock
    {

    private:

        gfx *t;

    protected:

        //ctor
        gfx_writelock( gfx *t, dpmutex_writelock *l );
        //dtor
        virtual ~gfx_writelock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //stop gfx task
        void kill( void );
        //run gfx from task
        void run( dpthread_lock *thd );
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
        //add renderer factory
        void addRenderer( renderer_factory *f );
        //change renderer
        bool changeRenderer( const char *cname );
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
        //returns taskpool
        dptaskpool_ref *getTasks( void );

        friend class gfx;
    };

};

#endif
