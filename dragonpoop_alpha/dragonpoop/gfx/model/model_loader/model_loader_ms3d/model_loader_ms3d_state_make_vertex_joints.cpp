
#include "model_loader_ms3d_state_make_vertex_joints.h"
#include "model_loader_ms3d_state_make_animation.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_vertex_joint/model_vertex_joint.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include <math.h>

#include <iostream>

namespace dragonpoop
{

    //ctor
    model_loader_ms3d_state_make_vertex_joints::model_loader_ms3d_state_make_vertex_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }

    //dtor
    model_loader_ms3d_state_make_vertex_joints::~model_loader_ms3d_state_make_vertex_joints( void )
    {

    }

    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_vertex_joints::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e, je;
        model_loader_ms3d *t;
        ms3d_model_vertex_m *v;
        std::vector<ms3d_model_vertex_m> *lv;
        ms3d_model_joint_m *j;
        std::vector<ms3d_model_joint_m> *lj;
        float n;

        m = (model_writelock *)o.writeLock( this->m, "model_loader_ms3d_state_make_vertex_joints::run" );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );

        t = (model_loader_ms3d *)ml->getLoader();

        lv = t->verts;
        e = (unsigned int)lv->size();
        lj = t->joints;
        je = (unsigned int)lj->size();

        for( i = 0; i < e; i++ )
        {
            v = &( ( *lv )[ i ] );

            n = 0;
            if( v->f.boneId >= 0 )
                n = 1;
            if( v->bones[ 0 ].id >= 0 && v->bones[ 0 ].weight > 0 )
                n += v->bones[ 0 ].weight * v->bones[ 0 ].weight;
            if( v->bones[ 1 ].id >= 0 && v->bones[ 1 ].weight > 0 )
                n += v->bones[ 1 ].weight * v->bones[ 1 ].weight;
            if( v->bones[ 2 ].id >= 0 && v->bones[ 2 ].weight > 0 )
                n += v->bones[ 2 ].weight * v->bones[ 2 ].weight;
            if( n > 0 )
                n = sqrt( n );

            if( v->f.boneId >= 0 && v->f.boneId < (int)je )
            {
                j = &( *lj )[ v->f.boneId ];
                this->makeVertexJoint( thd, ml, v, j, 1.0f / n, m );
            }
            if( v->bones[ 0 ].id >= 0 && v->bones[ 0 ].id < (int)je && v->bones[ 0 ].weight > 0 )
            {
                j = &( *lj )[ v->bones[ 0 ].id ];
                this->makeVertexJoint( thd, ml, v, j, v->bones[ 0 ].weight / n, m );
            }
            if( v->bones[ 1 ].id >= 0 && v->bones[ 1 ].id < (int)je && v->bones[ 1 ].weight > 0 )
            {
                j = &( *lj )[ v->bones[ 1 ].id ];
                this->makeVertexJoint( thd, ml, v, j, v->bones[ 1 ].weight / n, m );
            }
            if( v->bones[ 2 ].id >= 0 && v->bones[ 2 ].id < (int)je && v->bones[ 2 ].weight > 0 )
            {
                j = &( *lj )[ v->bones[ 2 ].id ];
                this->makeVertexJoint( thd, ml, v, j, v->bones[ 2 ].weight / n, m );
            }

        }
        o.unlock();

        return new model_loader_ms3d_state_make_animation( this->b, this->m );
    }

    //create vertex joint
    void model_loader_ms3d_state_make_vertex_joints::makeVertexJoint( dpthread_lock *thd, model_loader_writelock *ml,  ms3d_model_vertex_m *v, ms3d_model_joint_m *t, float w, model_writelock *m )
    {
        m->makeVertexJoint( thd->genId(), v->id, t->id, w );
    }

};
