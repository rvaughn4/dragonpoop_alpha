
#ifndef dragonpoop_model_loader_ms3d_state_make_vertex_joints_h
#define dragonpoop_model_loader_ms3d_state_make_vertex_joints_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    struct ms3d_model_vertex_m;
    class dpbitmap;
    struct ms3d_model_joint_m;
    class model_writelock;
    
    class model_loader_ms3d_state_make_vertex_joints : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //create vertex joint
        void makeVertexJoint( dpthread_lock *thd, model_loader_writelock *ml,  ms3d_model_vertex_m *v, ms3d_model_joint_m *t, float w, model_writelock *m );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_vertex_joints( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_vertex_joints( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif