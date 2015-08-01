
#ifndef dragonpoop_model_loader_state_openfile_h
#define dragonpoop_model_loader_state_openfile_h

#include "model_loader_state.h"

namespace dragonpoop
{
    
    class model_loader_state_openfile : public model_loader_state
    {
        
    private:
        
    protected:
        
        //ctor
        model_loader_state_openfile( void );
        
    public:
        
        //dtor
        virtual ~model_loader_state_openfile( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif