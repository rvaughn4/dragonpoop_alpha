
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
        //find model by name
        model_ref *findModel( const char *cname );
        //find model by id
        model_ref *findModel( dpid id );
        //get models
        void getModels( std::list<model *> *l );
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

        friend class gfx;
    };
    
};

#endif