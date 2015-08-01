
#ifndef dragonpoop_model_loader_ms3d_state_parse_header_h
#define dragonpoop_model_loader_ms3d_state_parse_header_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    
    class model_loader_ms3d_state_parse_header : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_loader_ms3d_state_parse_header( dpbuffer *b );
        //dtor
        virtual ~model_loader_ms3d_state_parse_header( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif