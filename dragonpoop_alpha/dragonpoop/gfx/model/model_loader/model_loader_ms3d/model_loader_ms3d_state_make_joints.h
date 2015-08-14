
#ifndef dragonpoop_model_loader_ms3d_state_make_joints_h
#define dragonpoop_model_loader_ms3d_state_make_joints_h

#include "../model_loader_state.h"
#include <string>

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    struct ms3d_model_material_m;
    class dpbitmap;
    struct ms3d_model_joint_m;
    class model_writelock;
    
    class model_loader_ms3d_state_make_joints : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //create joint
        void makejoint( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_joint_m *t, model_writelock *m );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_joints( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_joints( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif