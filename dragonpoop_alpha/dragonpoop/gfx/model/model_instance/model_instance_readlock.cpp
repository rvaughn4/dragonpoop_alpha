
#include "model_instance_readlock.h"
#include "model_instance.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_readlock::model_instance_readlock( model_instance *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_instance_readlock::~model_instance_readlock( void )
    {
        
    }
    
    //return core
    core *model_instance_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get id
    dpid model_instance_readlock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *model_instance_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_instance_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_instance_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_instance_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find vertex
    model_instance_vertex *model_instance_readlock::findVertex( dpid id )
    {
        return this->t->findVertex( id );
    }
    
    //get vertexes
    void model_instance_readlock::getVertexes( std::list<model_instance_vertex *> *l )
    {
        this->t->getVertexes( l );
    }
    
    //find group
    model_instance_group *model_instance_readlock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_instance_readlock::getGroups( std::list<model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance_readlock::findTriangleVertex( dpid id )
    {
        return this->t->findTriangleVertex( id );
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance_readlock::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        return this->t->findTriangleVertex( triangle_id, vertex_id );
    }
    
    //get triangle vertexes
    void model_instance_readlock::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l )
    {
        this->t->getTriangleVertexes( l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_instance_readlock::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid )
    {
        this->t->getTriangleVertexes( l, pid );
    }
    
};
