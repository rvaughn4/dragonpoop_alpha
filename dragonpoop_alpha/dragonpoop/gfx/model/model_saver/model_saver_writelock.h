
#ifndef dragonpoop_model_saver_writelock_h
#define dragonpoop_model_saver_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class model_saver;
    class dpmutex_readlock;
    class dpthread_lock;
    class model_ref;
    
    class model_saver_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_saver *t;
        
    protected:
        
        //ctor
        model_saver_writelock( model_saver *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_saver_writelock( void );
        
    public:
        
        //returns true if running
        bool isRunning( void );
        //returns true if sucessful
        bool wasSucessful( void );
        //run model saver from task
        void run( dpthread_lock *thd );
        //get model saver
        model_saver *getSaver( void );
        //returns model
        model_ref *getModel( void );
        
        friend class model_saver;
        friend class model_saver_state_fail;
        friend class model_saver_state_sucess;
        friend class model_saver_state_begin;
        
    };
    
};

#endif