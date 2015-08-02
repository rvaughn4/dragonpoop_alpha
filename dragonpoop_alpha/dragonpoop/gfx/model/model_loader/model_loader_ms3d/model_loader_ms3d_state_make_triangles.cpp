
#include "model_loader_ms3d_state_make_triangles.h"
#include "model_loader_ms3d_state_parse_groups.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_triangle/model_triangle.h"
#include "../../../../core/dpthread/dpthread_lock.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_triangles::model_loader_ms3d_state_make_triangles( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_triangles::~model_loader_ms3d_state_make_triangles( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_triangles::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        model_triangle *mv;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_triangle_m *v;
        std::vector<ms3d_model_triangle_m> *l;
        
        m = (model_writelock *)o.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->tris;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            
            mv = m->makeTriangle( thd->genId() );
            if( !mv )
                continue;
            
            v->id = mv->getId();
        }
        o.unlock();
        
        return new model_loader_ms3d_state_parse_groups( this->b, this->m );
    }
    
};
