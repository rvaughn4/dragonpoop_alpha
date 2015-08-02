
#ifndef dragonpoop_model_loader_writelock_h
#define dragonpoop_model_loader_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class model_loader;
    class dpmutex_readlock;
    class dpthread_lock;
    class model_ref;
    
    class model_loader_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_loader *t;
        
    protected:
        
        //ctor
        model_loader_writelock( model_loader *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_loader_writelock( void );
        
    public:
        
        //returns true if running
        bool isRunning( void );
        //returns true if sucessful
        bool wasSucessful( void );
        //run model loader from task
        void run( dpthread_lock *thd );
        //get model loader
        model_loader *getLoader( void );
        //returns model
        model_ref *getModel( void );
        
        friend class model_loader;
        friend class model_loader_state_close;
        friend class model_loader_state_sucess;
        friend class model_loader_state_fail;
        friend class model_loader_state_openfile;

    };
    
};

#endif