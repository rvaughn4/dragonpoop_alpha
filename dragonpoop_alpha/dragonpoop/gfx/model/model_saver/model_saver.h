
#ifndef dragonpoop_model_saver_h
#define dragonpoop_model_saver_h

#include "../model.h"
#include "../../../core/shared_obj/shared_obj.h"
#include <atomic>

namespace dragonpoop
{
    
    class model_saver_state;
    class dpbuffer;
    class model_ref;
    class model_saver_task;
    class model_saver_writelock;
    
    class model_saver : public shared_obj
    {
        
    private:
        
        model_ref *m;
        model_saver_state *cs;
        std::atomic<bool> bIsRun, bWasOpen;
        
        std::string fname, pname;
        char *buffer;
        unsigned int size;
        
    protected:
        
        //ctor
        model_saver( core *c, model_ref *m, std::string *pname, std::string *fname );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run model saver from task
        void run( dpthread_lock *thd, model_saver_writelock *g );
        //generate second state
        virtual model_saver_state *genState( dpbuffer *b );
        //returns true if sucessful
        bool wasSucessful( void );
        //returns model
        model_ref *getModel( void );
        
    public:
        
        
        //dtor
        virtual ~model_saver( void );
        //returns true if running
        bool isRunning( void );
        //save model to file
        static model_saver *saveFile( core *c, model_ref *m, const char *path_name, const char *fname );
        
        friend class model_saver_readlock;
        friend class model_saver_writelock;
        friend class model_saver_state_begin;
        friend class model_saver_state_sucess;
        friend class model_saver_state_fail;
        
    };
    
};

#endif