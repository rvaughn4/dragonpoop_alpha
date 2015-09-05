
#ifndef dragonpoop_model_saver_state_begin_h
#define dragonpoop_model_saver_state_begin_h

#include "model_saver_state.h"

namespace dragonpoop
{
    
    class model_saver_state_begin : public model_saver_state
    {
        
    private:
        
    protected:
        
        //ctor
        model_saver_state_begin( void );
        
    public:
        
        //dtor
        virtual ~model_saver_state_begin( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
        friend class model_saver;
    };
    
};

#endif