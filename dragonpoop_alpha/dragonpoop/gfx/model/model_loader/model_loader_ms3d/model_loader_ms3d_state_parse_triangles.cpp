
#include "model_loader_ms3d_state_parse_triangles.h"
#include "model_loader_ms3d_state_parse_groups.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_triangle.h"
#include "../model_loader_state_fail.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_triangles::model_loader_ms3d_state_parse_triangles( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_ms3d_state_parse_triangles::~model_loader_ms3d_state_parse_triangles( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_triangles::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        
        ms3d_model_triangle_section sh;
        ms3d_model_triangle_m h;
        unsigned int i;
        model_loader_ms3d *m;
        std::vector<ms3d_model_triangle_m> *v;
        
        m = (model_loader_ms3d *)ml->getLoader();
        
        v = m->tris;
        if( v )
            delete v;
        v = new std::vector<ms3d_model_triangle_m>();
        m->tris = v;
        
        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_state_fail( this->b );
        
        memset( &h, 0, sizeof( h ) );
        v->clear();
        
        for( i = 0; i < sh.cnt; i++ )
        {
            if( this->b->readBytes( (uint8_t *)&h.f, sizeof( h.f ) ) < sizeof( h.f ) )
                return new model_loader_state_fail( this->b );
            v->push_back( h );
        }
        
        return new model_loader_ms3d_state_parse_groups( this->b );
    }
    
};
