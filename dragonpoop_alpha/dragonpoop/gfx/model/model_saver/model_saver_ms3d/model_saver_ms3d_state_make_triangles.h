
#ifndef dragonpoop_model_saver_ms3d_state_make_triangles_h
#define dragonpoop_model_saver_ms3d_state_make_triangles_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    class model_readlock;
    class model_saver_ms3d;
    struct ms3d_model_triangle_m;
    
    class model_saver_ms3d_state_make_triangles : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //makes triangle
        void makeTriangle( model_saver_ms3d *t, model_readlock *m, model_triangle *v );
        //get vertex index
        int getVertex( model_saver_ms3d *t, dpid id );
        
    public:
        
        //ctor
        model_saver_ms3d_state_make_triangles( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_make_triangles( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif