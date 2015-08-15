
#include "model_loader_ms3d_state_parse_vertex_extras.h"
#include "model_loader_ms3d_state_make_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_vertex.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_vertex_extras::model_loader_ms3d_state_parse_vertex_extras( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_parse_vertex_extras::~model_loader_ms3d_state_parse_vertex_extras( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_vertex_extras::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        
        ms3d_model_vertex_extra_section sh;
        ms3d_model_vertex_extra_v2 h;
        unsigned int i;
        model_loader_ms3d *m;
        std::vector<ms3d_model_vertex_m> *v;
        ms3d_model_vertex_m *p;
        
        m = (model_loader_ms3d *)ml->getLoader();
        v = m->verts;
        
        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        
        if( sh.version != 1 && sh.version != 2 )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        
        for( i = 0; i < v->size(); i++ )
        {
            p = &( *v )[ i ];
            switch( sh.version )
            {
                case 1:
                    if( this->b->readBytes( (uint8_t *)&h.v, sizeof( h.v ) ) < sizeof( h.v ) )
                        return new model_loader_ms3d_state_make_joints( this->b, this->m );
                    break;
                case 2:
                    if( this->b->readBytes( (uint8_t *)&h, sizeof( h ) ) < sizeof( h ) )
                        return new model_loader_ms3d_state_make_joints( this->b, this->m );
                    break;
            }
            p->bones[ 0 ].id = h.v.bones[ 0 ];
            p->bones[ 0 ].weight = h.v.weights[ 0 ];
            p->bones[ 1 ].id = h.v.bones[ 1 ];
            p->bones[ 1 ].weight = h.v.weights[ 1 ];
            p->bones[ 2 ].id = h.v.bones[ 2 ];
            p->bones[ 2 ].weight = h.v.weights[ 2 ];
        }
        
        return new model_loader_ms3d_state_make_joints( this->b, this->m );
    }
    
};
