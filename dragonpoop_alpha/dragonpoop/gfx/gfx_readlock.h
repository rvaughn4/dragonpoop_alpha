
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
    class dpposition;
    class model_man_writelock;
    class model_man_readlock;
    class model_man_ref;
    class shared_obj_guard;

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
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //return fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsEachFrame( void );
        //return renderer
        renderer_ref *getRenderer( void );
        //return model count
        unsigned int getModelCount( void );
        //return gui count
        unsigned int getGuiCount( void );
        //get camera position
        void getCameraPosition( dpposition *p );
        //return actor count
        unsigned int getActorCount( void );
        
        //get models
        bool getModels( model_man_ref **r );
        //get models
        bool getModels( model_man_readlock **r, shared_obj_guard *o );
        //get models
        bool getModels( model_man_writelock **r, shared_obj_guard *o );

        friend class gfx;
    };
    
};

#endif