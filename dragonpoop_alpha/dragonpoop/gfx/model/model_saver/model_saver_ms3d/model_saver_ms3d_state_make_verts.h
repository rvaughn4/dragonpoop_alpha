
#ifndef dragonpoop_model_saver_ms3d_state_make_verts_h
#define dragonpoop_model_saver_ms3d_state_make_verts_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    class model_readlock;
    class model_saver_ms3d;
    struct ms3d_model_vertex_m;
    
    class model_saver_ms3d_state_make_verts : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //makes vert
        void makeVertex( model_saver_ms3d *t, model_readlock *m, model_vertex *v );
        //get ref count
        unsigned int getRefCount( model_readlock *m, model_vertex *v );
        //get vertex joints and sort them by weight and add them
        void addJointsSorted( model_saver_ms3d *t, model_readlock *m, ms3d_model_vertex_m *v );
        //find joint offset
        int findJoint( model_saver_ms3d *t, dpid id );
        
    public:
        
        //ctor
        model_saver_ms3d_state_make_verts( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_make_verts( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif