
#include "renderer_model_instance.h"
#include "renderer_model_instance_ref.h"
#include "renderer_model_instance_readlock.h"
#include "renderer_model_instance_writelock.h"
#include "../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../gfx/model/model_instance/model_instance_ref.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/bytetree/dpid_bytetree.h"
#include "../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"
#include "../../../gfx/model/model_instance/model_instance_joint/model_instance_joint.h"
#include "renderer_model_group_instance/renderer_model_group_instance.h"
#include "renderer_model_joint_instance/renderer_model_joint_instance.h"
#include "../../renderer_writelock.h"
#include "../renderer_model_material/renderer_model_material.h"
#include "../renderer_model_readlock.h"
#include "../../../core/dpthread/dpthread_lock.h"

#include "../../api_stuff/render_api/render_api_commandlist_writelock.h"
#include "../../api_stuff/render_api/render_api_context_writelock.h"
#include "../../api_stuff/render_api/render_api_vertexbuffer_ref.h"
#include "../../api_stuff/render_api/render_api_indexbuffer_ref.h"

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance::renderer_model_instance( model_instance_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        this->m = (model_instance_ref *)ml->getRef();
        this->id = ml->getId();
        this->bIsSynced = 0;
        this->bIsPosSynced = 0;
        this->fade_val = 0;
        this->bIsAlive = 1;
        this->smooth_rot.x = this->smooth_rot.y = this->smooth_rot.z = 0;
        ml->setRenderer( this );
    }
    
    //dtor
    renderer_model_instance::~renderer_model_instance( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_model_instance::~renderer_model_instance" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "renderer_model_instance::~renderer_model_instance" );
        this->deleteComponents();
        delete this->m;
        o.unlock();
    }
    
    //generate read lock
    shared_obj_readlock *renderer_model_instance::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_model_instance_readlock( (renderer_model_instance *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_model_instance::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_model_instance_writelock( (renderer_model_instance *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_model_instance::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_model_instance_ref( (renderer_model_instance *)p, k );
    }
    
    //delete all components
    void renderer_model_instance::deleteComponents( void )
    {
        std::list<model_component *> *l, d;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            d.push_back( c );
        }
        l->clear();
        this->comps.byid.clear();
        this->comps.byowner.clear();
        this->comps.bytype.clear();
        //this->jnts.clear(); ///HEAP PROBLEMS!!!
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            delete c;
        }
    }
    
    //get id
    dpid renderer_model_instance::getId( void )
    {
        return this->id;
    }
    
    //add component to list and trees
    void renderer_model_instance::addComponent( model_component *c )
    {
        uint16_t k;
        
        if( !c )
            return;
        
        this->comps.lst.push_back( c );
        this->comps.byid.addLeaf( c->getId(), c );
        k = c->getType();
        this->comps.bytype.addLeaf( (char *)&k, sizeof( k ), c );
        
        if( c->getType() == model_component_type_joint )
            this->jnts.addJoint( c, ((renderer_model_instance_joint *)c )->getIndex() );
    }
    
    //find component by type and id
    model_component *renderer_model_instance::findComponent( uint16_t mtype, dpid id )
    {
        model_component *c;
        
        c = (model_component *)this->comps.byid.findLeaf( id );
        if( !c || c->getType() != mtype )
            return 0;
        
        return c;
    }
    
    //find components by type
    void renderer_model_instance::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( (char *)&mtype, sizeof( mtype ), (std::list<void *> *)l );
    }
    
    //find components by type and 1 parent
    void renderer_model_instance::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
    {
        std::list<model_component *> l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        this->comps.byowner.findLeaves( p1, (std::list<void *> *)&l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            ll->push_back( c );
        }
    }
    
    //find components by type and 2 parents
    void renderer_model_instance::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
    {
        std::list<model_component *> l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        this->comps.byowner.findLeaves( p1, (std::list<void *> *)&l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            if( !c->hasParent( p2 ) )
                continue;
            ll->push_back( c );
        }
    }
    
    //remove component
    void renderer_model_instance::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
        this->comps.byid.removeLeaf( c );
        this->comps.byowner.removeLeaf( c );
        this->comps.bytype.removeLeaf( c );
        if( c->getType() == model_component_type_joint )
            this->jnts.remove( c );
    }
    
    //add group
    renderer_model_instance_group *renderer_model_instance::makeGroup( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
        renderer_model_instance_group *c;
        
        c = this->genGroup( ml, g, thd );
        this->addComponent( c );
        c->sync( ml, g, thd );
        
        return c;
    }
    
    //find group
    renderer_model_instance_group *renderer_model_instance::findGroup( dpid id )
    {
        return (renderer_model_instance_group *)this->findComponent( model_component_type_group, id );
    }
    
    //add joint
    renderer_model_instance_joint *renderer_model_instance::makeJoint( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd )
    {
        renderer_model_instance_joint *c;
        
        c = this->genJoint( ml, g, thd );
        this->addComponent( c );
        c->sync( ml, g, thd );
        
        return c;
    }
    
    //find joint
    renderer_model_instance_joint *renderer_model_instance::findJoint( dpid id )
    {
        return (renderer_model_instance_joint *)this->findComponent( model_component_type_joint, id );
    }
    
    //get groups
    void renderer_model_instance::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->getComponents( model_component_type_group, (std::list<model_component *> *)l );
    }
    
    //make groups
    void renderer_model_instance::makeGroups( model_instance_writelock *ml, dpthread_lock *thd )
    {
        std::list<renderer_model_instance_group *> li, d;
        std::list<renderer_model_instance_group *>::iterator ii;
        std::list<model_instance_group *> l;
        std::list<model_instance_group *>::iterator i;
        renderer_model_instance_group *pi;
        model_instance_group *p;
        dpid_btree t;
        
        //build index
        this->getGroups( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        //pair intances and sync them (or make them)
        ml->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pi = (renderer_model_instance_group *)t.findLeaf( p->getId() );
            if( pi )
            {
                pi->sync( ml, p, thd );
                t.removeLeaf( pi );
                continue;
            }
            this->makeGroup( ml, p, thd );
        }
        
        //find leaves in index not paired with a model_instance
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            if( t.findLeaf( pi->getId() ) )
                d.push_back( pi );
        }
        
        //delete them
        for( ii = d.begin(); ii != d.end(); ++ii )
        {
            pi = *ii;
            this->removeComponent( pi );
            delete pi;
        }
    }
    
    //animate groups
    void renderer_model_instance::syncGroups( model_instance_writelock *ml, dpthread_lock *thd )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *p;
        std::list<model_instance_group *> lg;
        std::list<model_instance_group *>::iterator ig;
        model_instance_group *pg;
        dpid_btree t;

        ml->getGroups( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            t.addLeaf( pg->getId(), pg );
        }
        
        this->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pg = (model_instance_group *)t.findLeaf( p->getId() );
            if( pg )
                p->sync( ml, pg, thd );
        }
    }
    
    //animate groups
    void renderer_model_instance::animateGroups( model_instance_writelock *ml, dpthread_lock *thd )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *p;
        std::list<model_instance_group *> lg;
        std::list<model_instance_group *>::iterator ig;
        model_instance_group *pg;
        dpid_btree t;
        
        ml->getGroups( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            t.addLeaf( pg->getId(), pg );
        }
        
        this->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pg = (model_instance_group *)t.findLeaf( p->getId() );
            if( pg )
                p->animate( ml, pg, thd );
        }
    }
    
    //run model from task
    void renderer_model_instance::run( dpthread_lock *thd, renderer_model_instance_writelock *g )
    {
        model_instance_writelock *ml;
        shared_obj_guard o;
        
        if( this->bIsAlive )
        {
            if( this->fade_val < 0.95f )
                this->fade_val += ( 1.0f - this->fade_val ) * 0.1f;
            else
                this->fade_val = 1;
        }
        else
        {
            if( this->fade_val > 0.05f )
            {
                if( this->fade_val > 0.85f )
                    this->fade_val *= 0.95f;
                else
                    this->fade_val *= 0.9f;
            }
            else
                this->fade_val = 0;
        }
        
        if( !this->bIsSynced )
        {
            ml = (model_instance_writelock *)o.tryWriteLock( this->m, 3, "renderer_model_instance::run" );
            if( ml )
            {
                this->gui_pos = *ml->getGuiDimensions();
                this->bIsGui = ml->isGui();
                this->makeJoints( ml, thd );
                this->makeGroups( ml, thd );
                this->bIsSynced = 1;
                this->onSync( thd, g, ml );
                this->syncJoints( ml, thd );
                this->syncGroups( ml, thd );
                this->bIsAlive = ml->isAlive();
            }
        }

        if( !this->bIsAnimated )
        {
            ml = (model_instance_writelock *)o.tryWriteLock( this->m, 3, "renderer_model_instance::run" );
            if( ml )
            {
                this->syncJoints( ml, thd );
                this->animateGroups( ml, thd );
                this->bIsAnimated = 1;
            }
        }
        
        if( !this->bIsPosSynced )
        {
            ml = (model_instance_writelock *)o.tryWriteLock( this->m, 3, "renderer_model_instance::run" );
            if( ml )
            {
                ml->getPosition( &this->pos );
                this->bIsPosSynced = 1;
            }
        }
    }
    
    //sync
    void renderer_model_instance::sync( void )
    {
        this->bIsSynced = 0;
    }
    
    //animate
    void renderer_model_instance::animate( void )
    {
        this->bIsAnimated = 0;
    }
    
    //handle sync
    void renderer_model_instance::onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml )
    {
        
    }
    
    //genertae group
    renderer_model_instance_group *renderer_model_instance::genGroup( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
        return new renderer_model_instance_group( ml, g, thd );
    }
    
    //genertae joint
    renderer_model_instance_joint *renderer_model_instance::genJoint( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd )
    {
        return new renderer_model_instance_joint( ml, g, thd );
    }

    //render model
    void renderer_model_instance::render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *g;
        renderer_model_material *mat;
        dpmatrix mlocal;
        
        this->getModelViewMatrix( thd, r, m, m_world, &mlocal );
        this->getGroups( &l );
        
        clist->setMatrix( &mlocal );
        clist->setAlpha( this->getAlpha() );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            g = *i;
            mat = m->findMaterial( g->getMaterialId() );
            this->renderGroup( thd, mi, mat, g, ctx, clist );
        }
    }
    
    //render group
    void renderer_model_instance::renderGroup( dpthread_lock *thd, renderer_model_instance_readlock *mi, renderer_model_material *m, renderer_model_instance_group *g, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        dpvertex_buffer *vb;
        dpindex_buffer *ib;
        render_api_vertexbuffer_ref *rvb;
        render_api_indexbuffer_ref *rib;
        
        vb = g->getTransformedBuffer( mi, &ib );
        rvb = ctx->makeVertexBuffer( vb );
        rib = ctx->makeIndexBuffer( ib );
        
        clist->setTexture( m->getDiffuseTex(), 0 );
        clist->setIndexBuffer( rib );
        clist->setVertexBuffer( rvb );
        clist->draw();
        
        delete rvb;
        delete rib;
    }
    
    //returns joint cache
    model_instance_joint_cache *renderer_model_instance::getJointCache( void )
    {
        return &this->jnts;
    }
    
    //get joints
    void renderer_model_instance::getJoints( std::list<renderer_model_instance_joint *> *l )
    {
        this->getComponents( model_component_type_joint, (std::list<model_component *> *)l );
    }
    
    //make joints
    void renderer_model_instance::makeJoints( model_instance_writelock *ml, dpthread_lock *thd )
    {
        std::list<renderer_model_instance_joint *> li, d;
        std::list<renderer_model_instance_joint *>::iterator ii;
        std::list<model_instance_joint *> l;
        std::list<model_instance_joint *>::iterator i;
        renderer_model_instance_joint *pi;
        model_instance_joint *p;
        dpid_btree t;
        
        //build index
        this->getJoints( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        //pair intances and sync them (or make them)
        ml->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pi = (renderer_model_instance_joint *)t.findLeaf( p->getId() );
            if( pi )
            {
                pi->sync( ml, p, thd );
                t.removeLeaf( pi );
                continue;
            }
            this->makeJoint( ml, p, thd );
        }
        
        //find leaves in index not paired with a model_instance
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            if( t.findLeaf( pi->getId() ) )
                d.push_back( pi );
        }
        
        //delete them
        for( ii = d.begin(); ii != d.end(); ++ii )
        {
            pi = *ii;
            this->removeComponent( pi );
            delete pi;
        }
    }
    
    //sync joints
    void renderer_model_instance::syncJoints( model_instance_writelock *ml, dpthread_lock *thd )
    {
        std::list<renderer_model_instance_joint *> l;
        std::list<renderer_model_instance_joint *>::iterator i;
        renderer_model_instance_joint *p;
        std::list<model_instance_joint *> lg;
        std::list<model_instance_joint *>::iterator ig;
        model_instance_joint *pg;
        dpid_btree t;
        
        ml->getJoints( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            t.addLeaf( pg->getId(), pg );
        }
        
        this->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pg = (model_instance_joint *)t.findLeaf( p->getId() );
            if( pg )
                p->sync( ml, pg, thd );
        }
    }
    
    //transform vertex using joints
    void renderer_model_instance::transform( dpvertex *v )
    {
        renderer_model_instance_joint *j;
        model_instance_joint_cache_element *je;
        dpvertex o, vc;
        unsigned int i, e, c;
        float w, cw;
        
        o.normal.x = o.normal.y = o.normal.z = 0;
        o.pos.x = o.pos.y = o.pos.z = 0;
        cw = 0.0f;

        e = dpvertex_bones_size;
        for( c = i = 0; i < e; i++ )
        {
            w = v->bones[ i ].w;
            if( w <= 0 || v->bones[ 0 ].index < 0 )
                continue;
            je = this->jnts.getElement( v->bones[ i ].index );
            if( !je )
                continue;
            j = (renderer_model_instance_joint *)je->p;
            if( !j )
                continue;
            
            vc.normal = v->normal;
            vc.pos = v->pos;
            j->transform( &vc );
            
            o.pos.x += vc.pos.x * w;
            o.pos.y += vc.pos.y * w;
            o.pos.z += vc.pos.z * w;
            
            o.normal.x += vc.normal.x * w;
            o.normal.y += vc.normal.y * w;
            o.normal.z += vc.normal.z * w;
            
            cw += w;
            c++;
        }
        if( !c || cw <= 0.0f )
            return;

        v->pos.x = o.pos.x / cw;
        v->pos.y = o.pos.y / cw;
        v->pos.z = o.pos.z / cw;
        
        v->normal.x = o.normal.x / cw;
        v->normal.y = o.normal.y / cw;
        v->normal.z = o.normal.z / cw;
    }

    //recompute animation joint matrixes
    void renderer_model_instance::redoMatrixes( renderer_model_instance_readlock *m, uint64_t t )
    {
        std::list<renderer_model_instance_joint *> l;
        std::list<renderer_model_instance_joint *>::iterator i;
        renderer_model_instance_joint *p;
        
        this->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->redoMatrix( m, t );
        }
    }
    
    //recompute animation joint matrixes
    void renderer_model_instance::redoMatrixes( renderer_model_instance_writelock *m, uint64_t t )
    {
        std::list<renderer_model_instance_joint *> l;
        std::list<renderer_model_instance_joint *>::iterator i;
        renderer_model_instance_joint *p;
        
        this->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->redoMatrix( m, t );
        }
    }
    
    //get model view matrix
    void renderer_model_instance::getModelViewMatrix( dpthread_lock *thd, dpposition *campos, renderer_model_readlock *m, dpmatrix *in_world_matrix, dpmatrix *out_model_matrix )
    {
        dpxyz_f pp, sz, ctr, rot;
        float fsz, rsz;
        dpquaternion qa, qb, qc;

        campos->getDifference( &this->pos, thd->getTicks(), &pp );
        m->getCenter( &ctr );
        m->getSize( &sz );
        fsz = sz.x * sz.x + sz.y * sz.y + sz.z * sz.z;
        if( fsz > 0.0f )
            fsz = sqrtf( fsz );
        else
            fsz = 1;
        rsz = 1.0f / fsz;
        
        this->pos.getDirection( &rot );
        qa.setAngle( &this->smooth_rot );
        qb.setAngle( &rot );
        qc.slerp( &qa, &qb, 0.125f );
        qc.getAngle( &rot );
        this->smooth_rot = rot;

        out_model_matrix->copy( in_world_matrix );
        
        if( !this->isGui() )
        {
            out_model_matrix->translate( pp.x, pp.y, pp.z - 3 );
            out_model_matrix->scale( rsz, rsz, rsz );
            out_model_matrix->translate( -ctr.x, -ctr.y, -ctr.z );
            
            out_model_matrix->rotateXrad( rot.x );
            out_model_matrix->rotateZrad( rot.z );
            out_model_matrix->rotateYrad( rot.y );
            
            return;
        }
        
    }
    
    //get dimensions
    model_gui_pos *renderer_model_instance::getGuiDimensions( void )
    {
        return &this->gui_pos;
    }
    
    //returns true if gui
    bool renderer_model_instance::isGui( void )
    {
        return this->bIsGui;
    }
    
    //get position
    void renderer_model_instance::getPosition( dpposition *p )
    {
        p->copy( &this->pos );
    }
    
    //get position
    dpposition *renderer_model_instance::_getPosition( void )
    {
        return &this->pos;
    }
    
    //sync position
    void renderer_model_instance::syncPosition( void )
    {
        this->bIsPosSynced = 0;
    }
    
    //returns true if alive
    bool renderer_model_instance::isAlive( void )
    {
        if( this->bIsAlive )
            return 1;
        return this->fade_val > 0.01f;
    }
    
    //kills instance
    void renderer_model_instance::kill( void )
    {
        this->bIsAlive = 0;
    }
    
    //retuns opacity
    float renderer_model_instance::getAlpha( void )
    {
        return this->fade_val;
    }
    
};
