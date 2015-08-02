
#ifndef dragonpoop_model_loader_h
#define dragonpoop_model_loader_h

#include "../model.h"
#include "../../../core/shared_obj/shared_obj.h"
#include <atomic>

namespace dragonpoop
{
    
    class model_loader_state;
    class dpbuffer;
    class model_ref;
    class model_loader_task;
    class model_loader_writelock;
    
    class model_loader : public shared_obj
    {
        
    private:
        
        dptask *tsk;
        model_loader_task *gtsk;
        model_ref *m;
        model_loader_state *cs;
        std::atomic<bool> bIsRun, bWasOpen;
        
        std::string fname;
        char *buffer;
        unsigned int size;
        
    protected:
        
        //ctor
        model_loader( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run model loader from task
        void run( dpthread_lock *thd, model_loader_writelock *g );
        //generate second state
        virtual model_loader_state *genState( dpbuffer *b );
        //returns true if sucessful
        bool wasSucessful( void );
        
    public:
        
        
        //dtor
        virtual ~model_loader( void );
        //returns true if running
        bool isRunning( void );
        //load model from file
        static model_loader *loadFile( core *c, dptaskpool_writelock *tp, model_ref *m, const char *fname );
        
        friend class model_loader_readlock;
        friend class model_loader_writelock;
        friend class model_loader_state_openfile;
        friend class model_loader_state_close;
        friend class model_loader_state_sucess;
        friend class model_loader_state_fail;
        
    };
    
};

#endif