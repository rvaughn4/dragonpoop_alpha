
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
    class model_loader;
    class model_instance_ref;

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
        //create model using name (if not exists, reuses if does), returns ref in pointer arg
        bool createModel( dpthread_lock *thd, const char *mname, model_ref **r );
        //create model and load model file into it
        bool loadModel( dpthread_lock *thd, const char *mname, const char *file_name, model_ref **r, model_loader **mldr );
        //find model by name
        model_ref *findModel( const char *cname );
        //find model by id
        model_ref *findModel( dpid id );
        //get a model instance by name
        model_instance_ref *makeModelInstance( const char *cname );
        //get a model instance by id
        model_instance_ref *makeModelInstance( dpid id );

        friend class gfx;
    };
    
};

#endif