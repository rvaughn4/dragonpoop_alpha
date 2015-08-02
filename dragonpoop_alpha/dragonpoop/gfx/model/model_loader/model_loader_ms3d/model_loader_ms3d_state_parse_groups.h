
#ifndef dragonpoop_model_loader_ms3d_state_parse_groups_h
#define dragonpoop_model_loader_ms3d_state_parse_groups_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    
    class model_loader_ms3d_state_parse_groups : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_loader_ms3d_state_parse_groups( dpbuffer *b );
        //dtor
        virtual ~model_loader_ms3d_state_parse_groups( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif