
#ifndef dragonpoop_model_saver_ms3d_state_make_groups_h
#define dragonpoop_model_saver_ms3d_state_make_groups_h

#include "../model_saver_state.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class model_saver_ms3d;
    class model_readlock;
    class model_saver_ms3d;
    struct ms3d_model_group_m;
    struct ms3d_model_triangle_m;
    
    class model_saver_ms3d_state_make_groups : public model_saver_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //makes group
        void makeGroup( model_saver_ms3d *t, model_readlock *m, model_group *v );
        //get material index
        int getMaterial( model_saver_ms3d *t, dpid id );
        //get triangle index
        int getTriangle( model_saver_ms3d *t, dpid id, ms3d_model_triangle_m **p );

    public:
        
        //ctor
        model_saver_ms3d_state_make_groups( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_saver_ms3d_state_make_groups( void );
        //run state, returns next state
        virtual model_saver_state *run( dpthread_lock *thd, model_saver_writelock *ml );
        
    };
    
};

#endif