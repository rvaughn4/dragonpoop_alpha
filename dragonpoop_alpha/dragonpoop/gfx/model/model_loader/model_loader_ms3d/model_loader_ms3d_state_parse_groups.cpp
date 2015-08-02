
#include "model_loader_ms3d_state_parse_groups.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_group.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_groups::model_loader_ms3d_state_parse_groups( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_parse_groups::~model_loader_ms3d_state_parse_groups( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_groups::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        
        ms3d_model_group_section sh;
        ms3d_model_group_m h;
        ms3d_model_group_triangle ht;
        unsigned int i, j;
        model_loader_ms3d *m;
        std::vector<ms3d_model_group_m> *v;
        
        m = (model_loader_ms3d *)ml->getLoader();
        
        v = m->groups;
        if( v )
            delete v;
        v = new std::vector<ms3d_model_group_m>();
        m->groups = v;
        
        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        memset( &h.f, 0, sizeof( h.f ) );
        h.e.index = 0;
        v->clear();
        
        for( i = 0; i < sh.cnt; i++ )
        {
            if( this->b->readBytes( (uint8_t *)&h.f, sizeof( h.f ) ) < sizeof( h.f ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
            
            h.triangles.clear();
            for( j = 0; j < h.f.cntTriangles; j++ )
            {
                if( this->b->readBytes( (uint8_t *)&ht, sizeof( ht ) ) < sizeof( ht ) )
                    return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
                h.triangles.push_back( ht );
            }
            
            if( this->b->readBytes( (uint8_t *)&h.e, sizeof( h.e ) ) < sizeof( h.e ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
            
            v->push_back( h );
        }
        
        return new model_loader_ms3d_state_cleanup( this->b, this->m, 1 );
    }
    
};
