
#ifndef dragonpoop_model_loader_ms3d_state_make_groups_h
#define dragonpoop_model_loader_ms3d_state_make_groups_h

#include "../model_loader_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    struct ms3d_model_group_m;
    
    class model_loader_ms3d_state_make_groups : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //make group
        void makeGroup( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_group_m *t, model_writelock *m );
        //make triangle vertex
        void makeGroupTriangle( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_group_m *t, model_writelock *m, model_group *tr, int vid );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_groups( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_groups( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif