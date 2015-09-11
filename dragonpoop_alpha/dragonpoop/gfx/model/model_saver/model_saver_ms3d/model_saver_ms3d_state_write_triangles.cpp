
#include "model_saver_ms3d_state_write_triangles.h"
#include "model_saver_ms3d_state_write_groups.h"
#include "model_saver_ms3d_triangle.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include <vector>

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_triangles::model_saver_ms3d_state_write_triangles( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_triangles::~model_saver_ms3d_state_write_triangles( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_triangles::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_triangle_section h;
        ms3d_model_triangle_m *v;
        std::vector<ms3d_model_triangle_m> *l;
        unsigned int i, e;
        model_saver_ms3d *t;
        
        t = (model_saver_ms3d *)ml->getSaver();
        l = t->tris;
        if( !l )
            e = 0;
        else
            e = (unsigned int)l->size();
        
        h.cnt = e;
        this->b->writeBytes( (uint8_t *)&h, sizeof( h ) );
        
        for( i = 0; i < e; i++ )
        {
            v = &( *l )[ i ];
            this->b->writeBytes( (uint8_t *)&v->f, sizeof( v->f ) );
        }
        
        return new model_saver_ms3d_state_write_groups( this->b, this->m );
    }
    
};
