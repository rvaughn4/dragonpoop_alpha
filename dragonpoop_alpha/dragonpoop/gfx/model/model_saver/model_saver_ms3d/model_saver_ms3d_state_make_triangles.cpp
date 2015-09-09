
#include "model_saver_ms3d_state_make_triangles.h"
#include "model_saver_ms3d_state_make_materials.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_triangle.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_triangle/model_triangle.h"
#include "../../model_triangle_vertex/model_triangle_vertex.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_triangles::model_saver_ms3d_state_make_triangles( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_triangles::~model_saver_ms3d_state_make_triangles( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_triangles::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_saver_ms3d *t;
        std::list<model_triangle *> l;
        std::list<model_triangle *>::iterator i;
        model_triangle *p;
        std::vector<ms3d_model_triangle_m> *lf;
        
        m = (model_readlock *)o.readLock( this->m );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        lf = t->tris;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_triangle_m>();
        t->tris = lf;
        
        m->getTriangles( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->makeTriangle( t, m, p );
        }
        
        return new model_saver_ms3d_state_make_materials( this->b, this->m );
    }
    
    //makes joints for animation
    void model_saver_ms3d_state_make_triangles::makeTriangle( model_saver_ms3d *t, model_readlock *m, model_triangle *p )
    {
        std::vector<ms3d_model_triangle_m> *lf;
        ms3d_model_triangle_m v;
        dpxyz_f w;
        dpst_f s;
        unsigned int c;
        std::list<model_triangle_vertex *> l;
        std::list<model_triangle_vertex *>::iterator i;
        model_triangle_vertex *j;

        lf = t->tris;
        
        v.id = p->getId();
        v.f.flags = 0;
        v.f.groupIndex = -1;
        v.f.smoothingGroup = 0;
        
        m->getTriangleVertexes( &l, p->getId() );
        c = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            j = *i;
            
            if( c > 2 )
                continue;
            v.f.verticies[ c ] = this->getVertex( t, j->getVertexId() );
            
            j->getTexCoord0( &s );
            v.f.s[ c ] = s.s;
            v.f.t[ c ] = s.t;
            
            j->getNormal( &w );
            v.f.normals[ c ].x = w.x;
            v.f.normals[ c ].y = w.y;
            v.f.normals[ c ].z = w.z;
        }
        
        lf->push_back( v );
    }
    
    //get vertex index
    int model_saver_ms3d_state_make_triangles::getVertex( model_saver_ms3d *t, dpid id )
    {
        std::vector<ms3d_model_vertex_m> *lf;
        ms3d_model_vertex_m *p;
        unsigned int i, e;
        
        lf = t->verts;
        if( !lf )
            return -1;
        
        e = (unsigned int)lf->size();
        for( i = 0; i < e; i++ )
        {
            p = &( *lf )[ i ];
            if( dpid_compare( &id, &p->id ) )
                return i;
        }
        
        return -1;
    }
    
};
