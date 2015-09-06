
#ifndef dragonpoop_model_saver_ms3d_state_make_frames_h
#define dragonpoop_model_saver_ms3d_state_make_frames_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    class model_readlock;
    class model_saver_ms3d;
    struct ms3d_model_animation_m;
    
    class model_saver_ms3d_state_make_frames : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //makes frames for animation
        void makeFrames( model_saver_ms3d *t, ms3d_model_animation_m *a, model_readlock *m );
        
    public:
        
        //ctor
        model_saver_ms3d_state_make_frames( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_make_frames( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif