
#include "model_saver_ms3d_state_make_verts.h"
#include "model_saver_ms3d_state_make_triangles.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_joint.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_vertex/model_vertex.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../model_vertex_joint/model_vertex_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_verts::model_saver_ms3d_state_make_verts( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_verts::~model_saver_ms3d_state_make_verts( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_verts::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_saver_ms3d *t;
        std::list<model_vertex *> l;
        std::list<model_vertex *>::iterator i;
        model_vertex *p;
        std::vector<ms3d_model_vertex_m> *lf;
        
        m = (model_readlock *)o.readLock( this->m, "model_saver_ms3d_state_make_verts::run" );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        lf = t->verts;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_vertex_m>();
        t->verts = lf;
        
        m->getVertexes( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->makeVertex( t, m, p );
        }
        
        return new model_saver_ms3d_state_make_triangles( this->b, this->m );
    }
    
    //makes joints for animation
    void model_saver_ms3d_state_make_verts::makeVertex( model_saver_ms3d *t, model_readlock *m, model_vertex *p )
    {
        std::vector<ms3d_model_vertex_m> *lf;
        ms3d_model_vertex_m v;
        dpxyz_f w;
        
        lf = t->verts;
        
        v.id = p->getId();
        v.f.flags = 0;
        
        p->getPosition( &w );
        v.f.position.x = w.x;
        v.f.position.y = w.y;
        v.f.position.z = w.z;
        
        v.f.referenceCount = this->getRefCount( m, p );
        v.f.boneId = -1;
        v.bones[ 0 ].id = -1;
        v.bones[ 1 ].id = -1;
        v.bones[ 2 ].id = -1;
        this->addJointsSorted( t, m, &v );
        
        lf->push_back( v );
    }
   
    //get ref count
    unsigned int model_saver_ms3d_state_make_verts::getRefCount( model_readlock *m, model_vertex *v )
    {
        std::list<model_triangle_vertex *> l;
        
        m->getTriangleVertexes( &l, v->getId() );
        
        return (unsigned int)l.size();
    }
    
    //get vertex joints and sort them by weight and add them
    void model_saver_ms3d_state_make_verts::addJointsSorted( model_saver_ms3d *t, model_readlock *m, ms3d_model_vertex_m *v )
    {
        std::vector<float> wv;
        std::list<model_vertex_joint *> l;
        std::list<model_vertex_joint *>::iterator i;
        model_vertex_joint *p;
        unsigned int c, a, b;
        
        m->getVertexJoints( &l, v->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            wv.push_back( p->getWeight() );
        }

        std::sort( wv.begin(), wv.end() );

        c = 0;
        b = (unsigned int)wv.size();
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            for( a = 0; a < b; a++ )
            {
                if( wv[ a ] != p->getWeight() )
                    continue;
                if( c > 2 )
                    continue;
                if( c == 0 )
                {
                    v->f.boneId = this->findJoint( t, p->getJointId() );
                    a = b;
                    continue;
                }
                v->bones[ c ].id = this->findJoint( t, p->getJointId() );
                v->bones[ c ].weight = p->getWeight();
            }
        }
    }
    
    //find joint offset
    int model_saver_ms3d_state_make_verts::findJoint( model_saver_ms3d *t, dpid id )
    {
        unsigned int i, e;
        std::vector<ms3d_model_joint_m> *l;
        ms3d_model_joint_m *j;
        
        l = t->joints;
        if( !l )
            return -1;
        
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            j = &( *l )[ i ];
            if( dpid_compare( &j->id, &id ) )
                return i;
        }
        
        return -1;
    }
    
};
