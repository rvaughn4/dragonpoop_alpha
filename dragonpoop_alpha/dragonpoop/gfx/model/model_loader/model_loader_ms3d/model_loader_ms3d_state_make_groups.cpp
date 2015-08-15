
#include "model_loader_ms3d_state_make_groups.h"
#include "model_loader_ms3d_state_make_animation.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_group/model_group.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../model_group_triangle/model_group_triangle.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_groups::model_loader_ms3d_state_make_groups( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_groups::~model_loader_ms3d_state_make_groups( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_groups::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_group_m *v;
        std::vector<ms3d_model_group_m> *l;
        
        m = (model_writelock *)o.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->groups;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            this->makeGroup( thd, ml, v, m );
        }
        o.unlock();
        
        return new model_loader_ms3d_state_make_animation( this->b, this->m );
    }
    
    
    //make triangle
    void model_loader_ms3d_state_make_groups::makeGroup( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_group_m *t, model_writelock *m )
    {
        model_group *mv;
        std::string s;
        unsigned int i, e, mi, ms;
        model_loader_ms3d *mld;
        std::vector<ms3d_model_material_m> *l;

        
        mv = m->makeGroup( thd->genId() );
        if( !mv )
            return;
        t->id = mv->getId();
        s.assign( (char *)t->f.name, sizeof( t->f.name ) );
        mv->setName( &s );
        
        if( t->e.index >= 0 )
        {
            mld = (model_loader_ms3d *)ml->getLoader();
            l = mld->materials;
            ms = (unsigned int)l->size();
            mi = t->e.index;
            if( mi < ms )
                mv->setMaterialId( ( *l )[ mi ].id );
        }
        
        e = t->f.cntTriangles;
        for( i = 0; i < e; i++ )
            this->makeGroupTriangle( thd, ml, t, m, mv, t->triangles[ i ].index );
    }
    
    //make triangle vertex
    void model_loader_ms3d_state_make_groups::makeGroupTriangle( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_group_m *t, model_writelock *m, model_group *tr, int vid )
    {
        model_group_triangle *tv;
        ms3d_model_triangle_m *v;
        model_loader_ms3d *ldr;
        std::vector<ms3d_model_triangle_m> *vz;
 
        ldr = (model_loader_ms3d *)ml->getLoader();
        vz = ldr->tris;
        
        if( vid < 0 || vid >= vz->size() )
            return;
        v = &( ( *vz )[ vid ] );
        
        tv = m->makeGroupTriangle( thd->genId(), t->id, v->id );
    }
    
};
