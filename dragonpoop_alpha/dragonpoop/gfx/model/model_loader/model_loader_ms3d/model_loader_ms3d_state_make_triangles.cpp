
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
#include "../../model_triangle_vertex/model_triangle_vertex.h"

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
            this->makeTriangle( thd, ml, v, m );
        }
        o.unlock();
        
        return new model_loader_ms3d_state_parse_groups( this->b, this->m );
    }
    
    //make triangle
    void model_loader_ms3d_state_make_triangles::makeTriangle( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_triangle_m *t, model_writelock *m )
    {
        model_triangle *mv;
        
        mv = m->makeTriangle( thd->genId() );
        if( !mv )
            return;
        t->id = mv->getId();
        
        this->makeTriangleVertex( thd, ml, t, m, mv, 0 );
        this->makeTriangleVertex( thd, ml, t, m, mv, 1 );
        this->makeTriangleVertex( thd, ml, t, m, mv, 2 );
    }
    
    //make triangle vertex
    void model_loader_ms3d_state_make_triangles::makeTriangleVertex( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_triangle_m *t, model_writelock *m, model_triangle *tr, int vid )
    {
        model_triangle_vertex *tv;
        ms3d_model_vertex_m *v;
        model_loader_ms3d *ldr;
        std::vector<ms3d_model_vertex_m> *vz;
        dpxyzw x;
        dpst s;
        
        ldr = (model_loader_ms3d *)ml->getLoader();
        vz = ldr->verts;
        
        if( t->f.verticies[ vid ] < 0 || t->f.verticies[ vid ] >= vz->size() )
            return;
        v = &( ( *vz )[ t->f.verticies[ vid ] ] );
        
        tv = m->makeTriangleVertex( thd->genId(), t->id, v->id );
        
        x.x = t->f.normals[ vid ].x;
        x.y = t->f.normals[ vid ].y;
        x.z = t->f.normals[ vid ].z;
        x.w = 1;
        tv->setNormal( &x );
        
        s.s = t->f.s[ vid ];
        s.t = t->f.t[ vid ];
        tv->setTexCoord0( &s );
        
        s.s = t->f.s[ vid ];
        s.t = t->f.t[ vid ];
        tv->setTexCoord1( &s );
        
        if( vid == 0 )
            tr->setVertex0( tv->getId() );
        if( vid == 1 )
            tr->setVertex1( tv->getId() );
        if( vid == 2 )
            tr->setVertex2( tv->getId() );
    }
    
};
