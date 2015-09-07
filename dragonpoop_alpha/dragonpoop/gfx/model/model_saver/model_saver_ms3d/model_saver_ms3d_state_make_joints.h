
#ifndef dragonpoop_model_saver_ms3d_state_make_joints_h
#define dragonpoop_model_saver_ms3d_state_make_joints_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    class model_readlock;
    class model_saver_ms3d;
    class model_joint;
    struct ms3d_model_joint_m;
    struct ms3d_model_frame;
    
    class model_saver_ms3d_state_make_joints : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //makes joint and keyframes
        void makeJoint( model_saver_ms3d *t, model_joint *j, model_readlock *m );
        //make joint keyframes
        void makeKeyframes( model_saver_ms3d *t, ms3d_model_joint_m *j, ms3d_model_frame *f, model_readlock *m );
        
    public:
        
        //ctor
        model_saver_ms3d_state_make_joints( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_make_joints( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif