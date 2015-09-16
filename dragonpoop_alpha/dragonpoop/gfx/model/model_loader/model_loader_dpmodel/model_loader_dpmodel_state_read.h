
#ifndef dragonpoop_model_loader_dpmodel_state_read_h
#define dragonpoop_model_loader_dpmodel_state_read_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    
    class model_loader_dpmodel_state_read : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
    public:
        
        //ctor
        model_loader_dpmodel_state_read( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_dpmodel_state_read( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif