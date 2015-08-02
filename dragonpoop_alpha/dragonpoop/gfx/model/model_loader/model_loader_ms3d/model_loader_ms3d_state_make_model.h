
#ifndef dragonpoop_model_loader_ms3d_state_make_model_h
#define dragonpoop_model_loader_ms3d_state_make_model_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    
    class model_loader_ms3d_state_make_model : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_model( dpbuffer *b );
        //dtor
        virtual ~model_loader_ms3d_state_make_model( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif