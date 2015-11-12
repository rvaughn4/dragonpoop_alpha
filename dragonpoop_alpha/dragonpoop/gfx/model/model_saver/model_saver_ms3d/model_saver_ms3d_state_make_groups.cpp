
#include "model_saver_ms3d_state_make_groups.h"
#include "model_saver_ms3d_state_write_header.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_group.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_group/model_group.h"
#include "../../model_group_triangle/model_group_triangle.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"

#include <string.h>

namespace dragonpoop
{

    //ctor
    model_saver_ms3d_state_make_groups::model_saver_ms3d_state_make_groups( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }

    //dtor
    model_saver_ms3d_state_make_groups::~model_saver_ms3d_state_make_groups( void )
    {

    }

    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_groups::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_saver_ms3d *t;
        std::list<model_group *> l;
        std::list<model_group *>::iterator i;
        model_group *p;
        std::vector<ms3d_model_group_m> *lf;

        m = (model_readlock *)o.readLock( this->m, "model_saver_ms3d_state_make_groups::run" );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();

        lf = t->groups;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_group_m>();
        t->groups = lf;

        m->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->makeGroup( t, m, p );
        }

        return new model_saver_ms3d_state_write_header( this->b, this->m );
    }

    //makes joints for animation
    void model_saver_ms3d_state_make_groups::makeGroup( model_saver_ms3d *t, model_readlock *m, model_group *p )
    {
        std::vector<ms3d_model_group_m> *lf;
        ms3d_model_group_m v, *pv;
        std::string s;
        std::list<model_group_triangle *> l;
        std::list<model_group_triangle *>::iterator i;
        model_group_triangle *pt;
        ms3d_model_triangle_m *tp;
        ms3d_model_group_triangle gt;
        int ti;

        lf = t->groups;

        v.id = p->getId();
        v.f.flags = 0;
        v.e.index = this->getMaterial( t, p->getMaterialId() );
        v.f.cntTriangles = 0;

        memset( v.f.name, 0, sizeof( v.f.name ) );
        p->getName( &s );
        s.copy( (char *)v.f.name, sizeof( v.f.name ) );

        lf->push_back( v );
        pv = &( *lf )[ lf->size() - 1 ];

        m->getGroupTriangles( &l, p->getId() );
        for( i = l.begin(); i != l.end(); ++i )
        {
            pt = *i;

            ti = this->getTriangle( t, pt->getTriangleId(), &tp );
            if( ti < 0 )
                continue;

            tp->group_id = p->getId();
            tp->f.groupIndex = (int)lf->size();

            gt.index = ti;

            pv->triangles.push_back( gt );
            pv->f.cntTriangles++;
        }
    }

    //get material index
    int model_saver_ms3d_state_make_groups::getMaterial( model_saver_ms3d *t, dpid id )
    {
        std::vector<ms3d_model_material_m> *lf;
        ms3d_model_material_m *p;
        unsigned int i, e;

        lf = t->materials;
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

    //get triangle index
    int model_saver_ms3d_state_make_groups::getTriangle( model_saver_ms3d *t, dpid id, ms3d_model_triangle_m **pp )
    {
        std::vector<ms3d_model_triangle_m> *lf;
        ms3d_model_triangle_m *p;
        unsigned int i, e;

        lf = t->tris;
        if( !lf )
            return -1;

        e = (unsigned int)lf->size();
        for( i = 0; i < e; i++ )
        {
            p = &( *lf )[ i ];
            if( !dpid_compare( &id, &p->id ) )
                continue;
            *pp = p;
            return i;
        }

        return -1;
    }

};
