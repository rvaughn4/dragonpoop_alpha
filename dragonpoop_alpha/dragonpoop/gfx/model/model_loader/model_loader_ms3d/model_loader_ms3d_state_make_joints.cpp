
#include "model_loader_ms3d_state_make_joints.h"
#include "model_loader_ms3d_state_make_verts.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_joint/model_joint.h"
#include "../../../../core/dpthread/dpthread_lock.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_joints::model_loader_ms3d_state_make_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_joints::~model_loader_ms3d_state_make_joints( void )
    {
        
    }

    //find joint by name
    ms3d_model_joint_m *model_loader_ms3d_state_make_joints::findJoint( char *cname, unsigned int sz, model_loader_ms3d *t )
    {
        ms3d_model_joint_m *v;
        std::vector<ms3d_model_joint_m> *l;
        unsigned int i, e;
        std::string s0, s1, sdeb;
        
        s0.assign( cname, sz );
        
        l = t->joints;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            
            s1.assign( (char *)v->f.name, sizeof( v->f.name ) );
            if( s0.compare( s1 ) == 0 )
                return v;
            sdeb.append( s0 );
            sdeb.append( " != " );
            sdeb.append( s1 );
            sdeb.append( "\r\n" );
        }
        
        std::cout << sdeb;
        return 0;
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_joints::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_joint_m *v, *p;
        std::vector<ms3d_model_joint_m> *l;
        model_joint *j;
        
        m = (model_writelock *)o.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->joints;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            this->makejoint( thd, ml, v, m );
        }

        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            
            p = this->findJoint( (char *)v->f.parent_name, sizeof( v->f.parent_name ), t );
            if( !p )
                continue;
            j = m->findJoint( v->id );
            if( !j )
                continue;
            
            j->setParent( p->id );
            v->pid = p->id;
        }
        
        o.unlock();
        return new model_loader_ms3d_state_make_verts( this->b, this->m );
    }
    
    
    //make triangle
    void model_loader_ms3d_state_make_joints::makejoint( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_joint_m *t, model_writelock *m )
    {
        model_joint *mv;
        std::string s;
        dpxyz_f x;
        
        mv = m->makeJoint( thd->genId() );
        if( !mv )
            return;
        t->id = mv->getId();
        s.assign( (char *)t->f.name, sizeof( t->f.name ) );
        mv->setName( &s );
        
        x.x = t->f.pos.x;
        x.y = t->f.pos.y;
        x.z = t->f.pos.z;
        mv->setPosition( &x );

        x.x = t->f.rot.x;
        x.y = t->f.rot.y;
        x.z = t->f.rot.z;
        mv->setRotation( &x );
    }
    
};
