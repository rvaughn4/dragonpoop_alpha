
#ifndef dragonpoop_model_saver_ms3d_state_write_header_h
#define dragonpoop_model_saver_ms3d_state_write_header_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    
    class model_saver_ms3d_state_write_header : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
    public:
        
        //ctor
        model_saver_ms3d_state_write_header( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_write_header( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif