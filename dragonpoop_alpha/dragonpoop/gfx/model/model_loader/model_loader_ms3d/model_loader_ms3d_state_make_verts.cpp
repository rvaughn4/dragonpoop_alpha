
#include "model_loader_ms3d_state_make_verts.h"
#include "model_loader_ms3d_state_make_vertex_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_vertex/model_vertex.h"
#include "../../../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_verts::model_loader_ms3d_state_make_verts( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_verts::~model_loader_ms3d_state_make_verts( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_verts::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        model_vertex *mv;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_vertex_m *v;
        std::vector<ms3d_model_vertex_m> *l;
        dpxyz_f x;
        
        m = (model_writelock *)o.writeLock( this->m, "model_loader_ms3d_state_make_verts::run" );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->verts;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            
            mv = m->makeVertex( thd->genId() );
            if( !mv )
                continue;
            v->id = mv->getId();
            
            x.x = v->f.position.x;
            x.y = v->f.position.y;
            x.z = v->f.position.z;
            
            mv->setPosition( &x );
        }
        o.unlock();
        
        return new model_loader_ms3d_state_make_vertex_joints( this->b, this->m );
    }
    
};
