
#ifndef dragonpoop_model_loader_ms3d_state_make_animation_h
#define dragonpoop_model_loader_ms3d_state_make_animation_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    
    class model_loader_ms3d_state_make_animation : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //read text file and create animations
        void readAnimations( dpthread_lock *thd, model_loader_writelock *ml );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_animation( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_animation( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif