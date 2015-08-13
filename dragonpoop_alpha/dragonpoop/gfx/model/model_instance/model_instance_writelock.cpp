
#include "model_instance_writelock.h"
#include "model_instance.h"

namespace dragonpoop
{

    //ctor
    model_instance_writelock::model_instance_writelock( model_instance *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_instance_writelock::~model_instance_writelock( void )
    {
        
    }
    
    //return core
    core *model_instance_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get id
    dpid model_instance_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *model_instance_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_instance_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_instance_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_instance_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find vertex
    model_instance_vertex *model_instance_writelock::findVertex( dpid id )
    {
        return this->t->findVertex( id );
    }
    
    //get vertexes
    void model_instance_writelock::getVertexes( std::list<model_instance_vertex *> *l )
    {
        this->t->getVertexes( l );
    }
    
    //find group
    model_instance_group *model_instance_writelock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_instance_writelock::getGroups( std::list<model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance_writelock::findTriangleVertex( dpid id )
    {
        return this->t->findTriangleVertex( id );
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance_writelock::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        return this->t->findTriangleVertex( triangle_id, vertex_id );
    }
    
    //get triangle vertexes
    void model_instance_writelock::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l )
    {
        this->t->getTriangleVertexes( l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_instance_writelock::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid )
    {
        this->t->getTriangleVertexes( l, pid );
    }

    //run model from task
    void model_instance_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //sync
    void model_instance_writelock::sync( model_writelock *ml )
    {
        this->t->sync( ml );
    }

    //set renderer model
    void model_instance_writelock::setRenderer( renderer_model_instance *r )
    {
        this->t->setRenderer( r );
    }
    
    //populate vertex buffer for rendering
    void model_instance_writelock::fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb )
    {
        this->t->fillVertexBuffer( group_id, vb );
    }

    //find animation
    model_instance_animation *model_instance_writelock::findAnimation( dpid id )
    {
        return this->t->findAnimation( id );
    }
    
    //get animations
    void model_instance_writelock::getAnimations( std::list<model_instance_animation *> *l )
    {
        this->t->getAnimations( l );
    }
    
};
