
#ifndef dragonpoop_model_loader_ms3d_state_make_triangles_h
#define dragonpoop_model_loader_ms3d_state_make_triangles_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    struct ms3d_model_triangle_m;
    class model_writelock;
    class model_triangle;
    
    class model_loader_ms3d_state_make_triangles : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //make triangle
        void makeTriangle( dpthread_lock *thd, ms3d_model_triangle_m *t, model_writelock *m );
        //make triangle vertex
        void makeTriangleVertex( dpthread_lock *thd, ms3d_model_triangle_m *t, model_writelock *m, model_triangle *tr, int vid );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_triangles( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_triangles( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif