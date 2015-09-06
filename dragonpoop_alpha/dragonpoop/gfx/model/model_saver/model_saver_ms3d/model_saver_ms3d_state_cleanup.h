
#ifndef dragonpoop_model_saver_ms3d_state_cleanup_h
#define dragonpoop_model_saver_ms3d_state_cleanup_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    
    class model_saver_ms3d_state_cleanup : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        bool bWasSucess;
        
    protected:
        
    public:
        
        //ctor
        model_saver_ms3d_state_cleanup( dpbuffer *b, model_ref *m, bool bWasSucess );
        //dtor
        virtual ~model_saver_ms3d_state_cleanup( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif