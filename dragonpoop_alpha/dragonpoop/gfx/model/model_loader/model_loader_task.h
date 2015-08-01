
#ifndef dragonpoop_model_loader_task_h
#define dragonpoop_model_loader_task_h

#include "../../../core/dptask/dptask_owner.h"

namespace dragonpoop
{
    
    class model_loader_ref;
    class dptask_writelock;
    class dpthread_lock;
    class model_loader;
    
    class model_loader_task : public dptask_owner
    {
        
    private:
        
        model_loader_ref *l;
        
    protected:
        
    public:
        
        //ctor
        model_loader_task( model_loader *l );
        //dtor
        virtual ~model_loader_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif