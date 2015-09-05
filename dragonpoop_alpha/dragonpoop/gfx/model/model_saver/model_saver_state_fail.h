
#ifndef dragonpoop_model_saver_state_fail_h
#define dragonpoop_model_saver_state_fail_h

#include "model_saver_state.h"

namespace dragonpoop
{
    
    class model_saver_state_fail : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_saver_state_fail( dpbuffer *b );
        //dtor
        virtual ~model_saver_state_fail( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif