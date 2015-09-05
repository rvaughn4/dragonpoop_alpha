
#ifndef dragonpoop_model_saver_state_h
#define dragonpoop_model_saver_state_h

#include "model_saver.h"

namespace dragonpoop
{
    
    class model_saver_state
    {
        
    private:
        
    protected:
        
        //ctor
        model_saver_state( void );
        
    public:
        
        //dtor
        virtual ~model_saver_state( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif