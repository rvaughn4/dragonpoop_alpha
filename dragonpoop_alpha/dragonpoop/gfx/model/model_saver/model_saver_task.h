
#ifndef dragonpoop_model_saver_task_h
#define dragonpoop_model_saver_task_h

#include "../../../core/dptask/dptask_owner.h"

namespace dragonpoop
{
    
    class model_saver_ref;
    class dptask_writelock;
    class dpthread_lock;
    class model_saver;
    
    class model_saver_task : public dptask_owner
    {
        
    private:
        
        model_saver_ref *l;
        
    protected:
        
    public:
        
        //ctor
        model_saver_task( model_saver *l );
        //dtor
        virtual ~model_saver_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif