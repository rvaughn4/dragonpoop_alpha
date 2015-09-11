
#include "model_saver_ms3d_state_write_groups.h"
#include "model_saver_ms3d_state_write_materials.h"
#include "model_saver_ms3d_group.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include <vector>

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_groups::model_saver_ms3d_state_write_groups( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_groups::~model_saver_ms3d_state_write_groups( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_groups::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_group_section h;
        ms3d_model_group_m *v;
        std::vector<ms3d_model_group_m> *l;
        unsigned int i, e, j, k;
        model_saver_ms3d *t;
        
        t = (model_saver_ms3d *)ml->getSaver();
        l = t->groups;
        if( !l )
            e = 0;
        else
            e = (unsigned int)l->size();
        
        h.cnt = e;
        this->b->writeBytes( (uint8_t *)&h, sizeof( h ) );
        
        for( i = 0; i < e; i++ )
        {
            v = &( *l )[ i ];

            k = (unsigned int)v->triangles.size();
            v->f.cntTriangles = k;

            this->b->writeBytes( (uint8_t *)&v->f, sizeof( v->f ) );
            
            for( j = 0; j < k; j++ )
                this->b->writeBytes( (uint8_t *)&v->triangles[ j ], sizeof( v->triangles[ 0 ] ) );

            this->b->writeBytes( (uint8_t *)&v->e, sizeof( v->e ) );
        }
        
        return new model_saver_ms3d_state_write_materials( this->b, this->m );
    }
    
};
