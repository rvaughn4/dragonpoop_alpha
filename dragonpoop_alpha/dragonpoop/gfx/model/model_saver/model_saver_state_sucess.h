
#ifndef dragonpoop_model_saver_state_sucess_h
#define dragonpoop_model_saver_state_sucess_h

#include "model_saver_state.h"

namespace dragonpoop
{
    
    class model_saver_state_sucess : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_saver_state_sucess( dpbuffer *b );
        //dtor
        virtual ~model_saver_state_sucess( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif