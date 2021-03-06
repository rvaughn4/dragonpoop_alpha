
#include "model_instance.h"
#include "model_instance_ref.h"
#include "model_instance_readlock.h"
#include "model_instance_writelock.h"
#include "../../../core/core.h"
#include "model_instance_animation/model_instance_animation.h"
#include "model_instance_group/model_instance_group.h"
#include "../model_writelock.h"
#include "../model_readlock.h"
#include "../model_ref.h"
#include "../../../core/dpbtree/dpid_btree.h"
#include "../../../core/dpbtree/dpid_multibtree.h"
#include "../../../core/bytetree/dpid_bytetree.h"
#include "../../../core/bytetree/dpid_multibytetree.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_ref.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_readlock.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../dpvertex/dpvertex.h"
#include "../../dpvertex/dpvertexindex_buffer.h"
#include "model_instance_animation/model_instance_animation.h"
#include "../model_animation/model_animation.h"
#include "model_instance_joint/model_instance_joint.h"
#include "../../../core/dpthread/dpthread_lock.h"
#include "../model_frame/model_frame.h"
#include "../model_frame_joint/model_frame_joint.h"
#include "../model_vertex_joint/model_vertex_joint.h"
#include "../model_group_triangle/model_group_triangle.h"
#include "../model_triangle_vertex/model_triangle_vertex.h"
#include "../model_vertex/model_vertex.h"
#include "../../dpmatrix/dpmatrix.h"

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    model_instance::model_instance( dpthread_lock *thd, dpid id, model_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        model_instance_writelock *l;
        shared_obj_guard g;
        
        this->r = 0;
        this->id = id;
        this->c = ml->getCore();
        this->m = (model_ref *)ml->getRef();
        this->t_frame_time = 50;
        this->bIsSynced = 1;
        this->j_ctr = 0;
        this->t_start = this->t_end = 0;
        this->t_play = 0;
        this->bIsGui = 0;
        this->bIsAlive = 1;
        
        l = (model_instance_writelock *)g.tryWriteLock( this, 400, "model_instance::model_instance" );
        if( !l )
            return;
        this->bIsSynced = 1;
        this->dosync( l, ml );
        this->makeAnimations( thd, ml );
    }
    
    //dtor
    model_instance::~model_instance( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "model_instance::~model_instance" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_instance::~model_instance" );
        this->deleteComponents();
        delete this->m;
        delete this->r;
        o.unlock();
    }
    
    //return core
    core *model_instance::getCore( void )
    {
        return this->c;
    }
    
    //delete all components
    void model_instance::deleteComponents( void )
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
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            delete c;
        }
    }

    //generate read lock
    shared_obj_readlock *model_instance::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_instance_readlock( (model_instance *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *model_instance::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_instance_writelock( (model_instance *)p, l );
    }
    
    //generate ref
    shared_obj_ref *model_instance::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_instance_ref( (model_instance *)p, k );
    }
    
    //run model from task
    void model_instance::run( dpthread_lock *thd, model_instance_writelock *g, model_writelock *m, unsigned int ms_each_frame )
    {
        uint64_t t;
        
        if( !this->bIsSynced )
        {
            this->dosync( g, m );
            this->bIsSynced = 1;
        }
        
        t = thd->getTicks();
        if( t - this->t_last_animate > this->t_frame_time )
        {
            ms_each_frame *= 2;
            if( ms_each_frame < 10 )
                ms_each_frame = 10;
            this->animate( g, m, thd, ms_each_frame );
            this->t_last_animate = t;
        }
    }
    
    //get id
    dpid model_instance::getId( void )
    {
        return this->id;
    }
    
    //add component to list and trees
    void model_instance::addComponent( model_component *c )
    {
        uint16_t k;
        
        this->comps.lst.push_back( c );
        this->comps.byid.addLeaf( c->getId(), c );
        k = c->getType();
        this->comps.bytype.addLeaf( (char *)&k, sizeof( k ), c );
    }
    
    //add component, 1 parent
    void model_instance::addComponent( model_component *c, dpid p1 )
    {
        this->addComponent( c );
        this->comps.byowner.addLeaf( p1, c );
    }
    
    //add component, 2 parents
    void model_instance::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->addComponent( c, p1 );
        this->comps.byowner.addLeaf( p2, c );
    }
    
    //find component by type and id
    model_component *model_instance::findComponent( uint16_t mtype, dpid id )
    {
        model_component *c;
        
        c = (model_component *)this->comps.byid.findLeaf( id );
        
        if( !c || c->getType() != mtype )
            return 0;
        
        return c;
    }
    
    //find components by type
    void model_instance::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( (char *)&mtype, sizeof( mtype ), (std::list<void *> *)l );
    }
    
    //find components by type and 1 parent
    void model_instance::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
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
    void model_instance::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
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
    void model_instance::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
        this->comps.byid.removeLeaf( c );
        this->comps.byowner.removeLeaf( c );
        this->comps.bytype.removeLeaf( c );
    }

    //add group
    model_instance_group *model_instance::makeGroup( model_group *g )
    {
        model_instance_group *c;
        c = new model_instance_group( g );
        this->addComponent( c );
        return c;
    }
    
    //find group
    model_instance_group *model_instance::findGroup( dpid id )
    {
        return (model_instance_group *)this->findComponent( model_component_type_group, id );
    }
    
    //get groups
    void model_instance::getGroups( std::list<model_instance_group *> *l )
    {
        this->getComponents( model_component_type_group, (std::list<model_component *> *)l );
    }
    
    //make groups
    void model_instance::makeGroups( model_writelock *ml )
    {
        std::list<model_group *> l;
        std::list<model_group *>::iterator i;
        model_group *p;
        std::list<model_instance_group *> li;
        std::list<model_instance_group *>::iterator ii;
        model_instance_group *pi;
        dpid_btree t;
        
        this->getGroups( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getGroups( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( !t.findLeaf( p->getId() ) )
                this->makeGroup( p );
        }
    }
    
    //sync
    void model_instance::sync( void )
    {
        this->bIsSynced = 0;
    }

    //sync
    void model_instance::dosync( model_instance_writelock *mi, model_writelock *ml )
    {
        shared_obj_guard o;
        renderer_model_instance_writelock *rl;

        this->makeJoints( ml );
        this->makeGroups( ml );
        this->redoMesh( mi, ml );
        
        if( !this->r )
            return;
        rl = (renderer_model_instance_writelock *)o.tryWriteLock( this->r, 400, "model_instance::dosync" );
        if( !rl )
            return;
        rl->sync();
    }
    
    //do animation
    void model_instance::animate( model_instance_writelock *mi, model_writelock *ml, dpthread_lock *thd, unsigned int ms_each_frame )
    {
        shared_obj_guard o;
        renderer_model_instance_writelock *rl;
        unsigned int i;
        
        this->t_frame_time = 10;
        
        i = ( ms_each_frame + 9 ) / 10;
        i *= 10;
        if( i > 100 )
            i = 100;
        if( i > this->t_frame_time )
            this->t_frame_time = i;
        
        this->runAnimations( mi, ml, thd );
        this->runJoints( mi, ml, thd );
        
        if( !this->r )
            return;
        rl = (renderer_model_instance_writelock *)o.tryWriteLock( this->r, 30, "model_instance::animate" );
        if( !rl )
            return;
        rl->animate();
    }
 
    //run animations
    void model_instance::runAnimations( model_instance_writelock *mi, model_writelock *ml, dpthread_lock *thd )
    {
        std::list<model_instance_animation *> l, d;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *p;
        
        this->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->run( mi, ml, thd, (unsigned int)this->t_frame_time );
            if( !p->isPlaying() )
                d.push_back( p );
        }

        for( i = d.begin(); i != d.end(); ++i )
        {
            p = *i;
            this->removeComponent( p );
            delete p;
        }
    }
    
    //run joints
    void model_instance::runJoints( model_instance_writelock *mi, model_writelock *ml, dpthread_lock *thd )
    {
        std::list<model_instance_joint *> l;
        std::list<model_instance_joint *>::iterator i;
        model_instance_joint *p;
        
        this->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->run( mi, ml, thd );
        }
    }
    
    //set renderer model
    void model_instance::setRenderer( renderer_model_instance *r )
    {
        shared_obj_guard o;
        renderer_model_instance_writelock *rl;
        
        if( this->r )
            delete this->r;
        this->r = 0;
        
        rl = (renderer_model_instance_writelock *)o.tryWriteLock( r, 1000, "model_instance::setRenderer" );
        if( !rl )
            return;
        this->r = (renderer_model_instance_ref *)rl->getRef();
        rl->sync();
        rl->animate();
    }
    
    //add animation
    model_instance_animation *model_instance::makeAnimation( dpid id, model_animation *g )
    {
        model_instance_animation *c;
        c = new model_instance_animation( id, g );
        this->addComponent( c, g->getId() );
        return c;
    }
    
    //find animation
    model_instance_animation *model_instance::findAnimation( dpid id )
    {
        return (model_instance_animation *)this->findComponent( model_component_type_animation, id );
    }
    
    //find animation
    model_instance_animation *model_instance::findAnimation( const char *cname )
    {
        std::list<model_instance_animation *> l;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *p;
        std::string s;
        
        s.assign( cname );
        this->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( p->compareName( &s ) )
                return p;
        }
        
        return 0;
    }
    
    //stop all animations
    void model_instance::stopAllAnimations( void )
    {
        std::list<model_instance_animation *> l;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *p;
        
        this->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->stop();
        }
    }
    
    //play animation (returns animation instance id)
    dpid model_instance::playAnimation( dpid id, model_readlock *ml, const char *cname, bool bDoRepeat, float speed )
    {
        model_animation *a;
        a = ml->findAnimation( cname );
        return this->playAnimation( id, a, bDoRepeat, speed );
    }
    
    //play animation (returns animation instance id)
    dpid model_instance::playAnimation( dpid id, model_writelock *ml, const char *cname, bool bDoRepeat, float speed )
    {
        model_animation *a;
        a = ml->findAnimation( cname );
        return this->playAnimation( id, a, bDoRepeat, speed );
    }
    
    //play animation (returns animation instance id)
    dpid model_instance::playAnimation( dpid id, model_animation *a, bool bDoRepeat, float speed )
    {
        model_instance_animation *r;
        dpid rid;
        
        if( !a )
        {
            dpid_zero( &rid );
            return rid;
        }
        
        r = this->makeAnimation( id, a );
        r->setRepeated( bDoRepeat );
        
        if( speed <= 0 )
            speed = 1;
        r->setSpeed( speed );
        r->start();
        
        return r->getId();
    }
    
    //stop animation
    void model_instance::stopAnimation( const char *cname )
    {
        model_instance_animation *a;
        a = this->findAnimation( cname );
        this->stopAnimation( a );
    }
    
    //stop animation
    void model_instance::stopAnimation( dpid id )
    {
        model_instance_animation *a;
        a = this->findAnimation( id );
        this->stopAnimation( a );
    }
    
    //stop animation
    void model_instance::stopAnimation( model_instance_animation *a )
    {
        if( a )
            a->stop();
    }
    
    //returns true if animation is playing
    bool model_instance::isAnimationPlaying( const char *cname )
    {
        model_instance_animation *a;
        a = this->findAnimation( cname );
        return this->isAnimationPlaying( a );
    }
    
    //returns true if animation is playing
    bool model_instance::isAnimationPlaying( dpid id )
    {
        model_instance_animation *a;
        a = this->findAnimation( id );
        return this->isAnimationPlaying( a );
    }
    
    //returns true if animation is playing
    bool model_instance::isAnimationPlaying( model_instance_animation *a )
    {
        if( !a )
            return 0;
        return a->isPlaying();
    }
    
    //change animation repeat
    void model_instance::setAnimationRepeat( const char *cname, bool bDoRepeat )
    {
        model_instance_animation *a;
        a = this->findAnimation( cname );
        this->setAnimationRepeat( a, bDoRepeat );
    }
    
    //change animation repeat
    void model_instance::setAnimationRepeat( dpid id, bool bDoRepeat )
    {
        model_instance_animation *a;
        a = this->findAnimation( id );
        this->setAnimationRepeat( a, bDoRepeat );
    }
    
    //change animation repeat
    void model_instance::setAnimationRepeat( model_instance_animation *a, bool bDoRepeat )
    {
        if( a )
            a->setRepeated( bDoRepeat );
    }
    
    //change animation speed
    void model_instance::setAnimationSpeed( const char *cname, float s )
    {
        model_instance_animation *a;
        a = this->findAnimation( cname );
        this->setAnimationSpeed( a, s );
    }
    
    //change animation speed
    void model_instance::setAnimationSpeed( dpid id, float s )
    {
        model_instance_animation *a;
        a = this->findAnimation( id );
        this->setAnimationSpeed( a, s );
    }
    
    //change animation speed
    void model_instance::setAnimationSpeed( model_instance_animation *a, float s )
    {
        if( s <= 0 && a )
            return a->stop();
        if( a )
            a->setSpeed( s );
    }
    
    //get animations
    void model_instance::getAnimations( std::list<model_instance_animation *> *l )
    {
        this->getComponents( model_component_type_animation, (std::list<model_component *> *)l );
    }
    
    //make animations
    void model_instance::makeAnimations( dpthread_lock *thd, model_writelock *ml )
    {
        std::list<model_animation *> l;
        std::list<model_animation *>::iterator i;
        model_animation *p;
        
        ml->getAnimations( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( p->isAutoPlay() )
                this->makeAnimation( thd->genId(), p );
        }
    }
    
    //add joint
    model_instance_joint *model_instance::makeJoint( model_joint *g )
    {
        model_instance_joint *c;
        this->j_ctr++;
        c = new model_instance_joint( g, this->j_ctr );
        this->addComponent( c );
        return c;
    }
    
    //find joint
    model_instance_joint *model_instance::findJoint( dpid id )
    {
        return (model_instance_joint *)this->findComponent( model_component_type_joint, id );
    }
    
    //get joints
    void model_instance::getJoints( std::list<model_instance_joint *> *l )
    {
        this->getComponents( model_component_type_joint, (std::list<model_component *> *)l );
    }
    
    //make joints
    void model_instance::makeJoints( model_writelock *ml )
    {
        std::list<model_joint *> l;
        std::list<model_joint *>::iterator i;
        model_joint *p;
        std::list<model_instance_joint *> li;
        std::list<model_instance_joint *>::iterator ii;
        model_instance_joint *pi;
        dpid_btree t;
        
        this->getJoints( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getJoints( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                continue;
            this->makeJoint( p );
        }
    }
    
    //populate vertex buffer for rendering
    void model_instance::fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb )
    {
        model_instance_group *g = this->findGroup( group_id );
        if( !g )
            return;
        vb->clear();
        vb->copy( g->getVertexBuffer() );
    }
    
    //redo mesh
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m )
    {
        std::list<model_instance_group *> l;
        std::list<model_instance_group *>::iterator i;
        model_instance_group *p;
        
        this->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->redoMesh( mi, m, p );
        }
    }
    
    //redo group mesh
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m, model_instance_group *g )
    {
        dpvertexindex_buffer *vb;
        std::list<void *> l;
        std::list<void *>::iterator i;
        void *p;
        unsigned int tcnt;
        dpvertexindex_buffer b;
        
        vb = g->getVertexBuffer();
        vb->clear();
        
        tcnt = 0;
        m->getGroupTriangles( (std::list<model_group_triangle *> *)&l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->redoMesh( mi, m, &b, (model_group_triangle *)p );
            tcnt++;
            if( tcnt > 15 )
            {
                tcnt = 0;
                vb->append( &b );
                b.clear();
            }
        }
        
        vb->append( &b );
    }
    
    //redo triangle mesh
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m, dpvertexindex_buffer *vb, model_group_triangle *t )
    {
        std::list<model_triangle_vertex *> l;
        std::list<model_triangle_vertex *>::iterator i;
        model_triangle_vertex *p;
        
        m->getTriangleVertexes( &l, t->getTriangleId() );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->redoMesh( mi, m, vb, p );
        }
    }
    
    //redo vertex mesh
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m, dpvertexindex_buffer *vb, model_triangle_vertex *tv )
    {
        model_vertex *p;
        dpvertex vt;
        
        p = m->findVertex( tv->getVertexId() );
        if( !p )
            return;
        
        tv->getNormal( &vt.normal );
        tv->getTexCoord0( &vt.texcoords[ 0 ] );
        tv->getTexCoord1( &vt.texcoords[ 1 ] );
        p->getPosition( &vt.pos );

        this->redoMesh( mi, m, tv, &vt );
        
        vb->addVertexUnique( &vt );
    }
    
    //redo vertex mesh, transform using joints
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m, model_triangle_vertex *tv, dpvertex *v )
    {
        std::list<model_vertex_joint *> l;
        std::list<model_vertex_joint *>::iterator i;
        model_vertex_joint *vj;
        float w, aw;
        unsigned int vi, vs, jcnt;
        
        vs = dpvertex_bones_size;
        for( vi = 0; vi < vs; vi++ )
        {
            v->bones[ vi ].index = -1;
        }
        
        m->getVertexJoints( &l, tv->getVertexId() );
        
        jcnt = 0;
        aw = 0;
        for( i = l.begin(); i != l.end(); ++i, jcnt++ )
        {
            vj = *i;
            
            w = vj->getWeight();
            if( w < 0.001f )
                continue;
            
            this->redoMesh( mi, m, tv, vj, v, jcnt );
            if( jcnt < vs )
                aw += v->bones[ jcnt ].w;
        }

        if( aw <= 0 )
            aw = 1.0f;
        for( vi = 0; vi < jcnt; vi++ )
            v->bones[ vi ].w /= aw;
    }
    
    //redo vertex mesh, transform using joints
    void model_instance::redoMesh( model_instance_writelock *mi, model_writelock *m, model_triangle_vertex *tv, model_vertex_joint *vj, dpvertex *v, unsigned int jcnt )
    {
        model_instance_joint *j;
        
        j = this->findJoint( vj->getJointId() );
        if( !j )
            return;
        
        if( jcnt >= dpvertex_bones_size )
            return;
        v->bones[ jcnt ].index = j->getIndex();
        v->bones[ jcnt ].w = vj->getWeight();
    }
    
    //get start time
    uint64_t model_instance::getStartTime( void )
    {
        return this->t_start;
    }
    
    //get end time
    uint64_t model_instance::getEndTime( void )
    {
        return this->t_end;
    }
    
    //set position
    void model_instance::setGuiPosition( float x, float y )
    {
        this->gui_pos.x = x;
        this->gui_pos.y = y;
    }
    
    //set width and height
    void model_instance::setGuiSize( float w, float h )
    {
        this->gui_pos.w = w;
        this->gui_pos.h = h;
    }
    
    //get dimensions
    model_gui_pos *model_instance::getGuiDimensions( void )
    {
        return &this->gui_pos;
    }
    
    //set gui mode
    void model_instance::setGuiMode( bool b )
    {
        this->bIsGui = b;
    }
    
    //returns true if gui
    bool model_instance::isGui( void )
    {
        return this->bIsGui;
    }
    
    //get position
    void model_instance::getPosition( dpposition *p )
    {
        p->copy( &this->pos );
    }
    
    //set position
    void model_instance::setPosition( dpposition *p )
    {
        renderer_model_instance_writelock *rl;
        shared_obj_guard o;
        
        this->pos.copy( p );
        
        if( !this->r )
            return;
        rl = (renderer_model_instance_writelock *)o.tryWriteLock( this->r, 30, "model_instance::setPosition" );
        if( !rl )
            return;
        rl->syncPosition();
    }
    
    //returns true if alive
    bool model_instance::isAlive( void )
    {
        return this->bIsAlive;
    }
    
    //kill instance
    void model_instance::kill( void )
    {
        this->bIsAlive = 0;
    }
    
    //clone animations
    void model_instance::cloneAnimations( model_instance_ref *mi )
    {
        model_readlock *ml;
        model_instance_readlock *mil;
        shared_obj_guard om, omi;
        std::list<model_instance_animation *> l;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *a;
        std::list<model_instance_joint *> lj;
        std::list<model_instance_joint *>::iterator ij;
        model_instance_joint *j, *nj;
        
        mil = (model_instance_readlock *)omi.tryReadLock( mi, 1000, "model_instance::cloneAnimations" );
        if( !mil )
            return;
        ml = (model_readlock *)om.tryReadLock( this->m, 1000, "model_instance::cloneAnimations" );
        if( !ml )
            return;
        
        mil->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            a = *i;
            this->cloneAnimation( ml, a );
        }
        
        mil->getJoints( &lj );
        for( ij = lj.begin(); ij != lj.end(); ++ij )
        {
            j = *ij;
            nj = this->findClosestJoint( j );
            if( !nj )
                continue;
            nj->clone( j );
        }
    }

    //clone animation
    bool model_instance::cloneAnimation( model_readlock *ml, model_instance_animation *a )
    {
        model_animation *ma;
        std::string sname;
        model_instance_animation *na;
        
        a->getName( &sname );
        ma = ml->findAnimation( sname.c_str() );
        if( !ma )
            return 0;
        
        na = this->makeAnimation( a->getId(), ma );
        if( !na )
            return 0;
        
        na->clone( a );
        return 1;
    }
    
    //find closest joint
    model_instance_joint *model_instance::findClosestJoint( model_instance_joint *j )
    {
        model_instance_joint *r, *p;
        std::list<model_instance_joint *> l;
        std::list<model_instance_joint *>::iterator i;
        dpxyz_f jp, pp;
        float last_d, d;
        
        j->getPosition( &jp );
        r = 0;
        last_d = 0;
        
        this->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            p->getPosition( &pp );
            
            d = ( jp.x - pp.x ) * ( jp.x - pp.x );
            d += ( jp.y - pp.y ) * ( jp.y - pp.y );
            d += ( jp.z - pp.z ) * ( jp.z - pp.z );
            if( d > 0 )
                d = sqrtf( d );
            
            if( !r )
            {
                r = p;
                last_d = d;
                continue;
            }
            if( d > last_d )
                continue;
            r = p;
            last_d = d;
        }
        
        return r;
    }
    
};
