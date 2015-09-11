
#include "model_saver_ms3d_state_write_materials.h"
#include "model_saver_ms3d_state_write_animation.h"
#include "model_saver_ms3d_material.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include <vector>

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_materials::model_saver_ms3d_state_write_materials( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_materials::~model_saver_ms3d_state_write_materials( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_materials::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_material_section h;
        ms3d_model_material_m *v;
        std::vector<ms3d_model_material_m> *l;
        unsigned int i, e;
        model_saver_ms3d *t;
        
        t = (model_saver_ms3d *)ml->getSaver();
        l = t->materials;
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
        
        return new model_saver_ms3d_state_write_animation( this->b, this->m );
    }
    
};
