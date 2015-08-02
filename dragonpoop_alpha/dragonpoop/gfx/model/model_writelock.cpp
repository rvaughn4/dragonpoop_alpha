
#include "model_writelock.h"
#include "model.h"

namespace dragonpoop
{

    //ctor
    model_writelock::model_writelock( model *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    model_writelock::~model_writelock( void )
    {

    }

    //return core
    core *model_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //run model from task
    void model_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }

    //set name
    void model_writelock::setName( std::string *sname )
    {
        this->t->setName( sname );
    }

    //get name
    void model_writelock::getName( std::string *sname )
    {
        this->t->getName( sname );
    }

    //compare name
    bool model_writelock::compareName( std::string *sname )
    {
        return this->t->compareName( sname );
    }

    //set id
    void model_writelock::setId( dpid id )
    {
        this->t->setId( id );
    }

    //get id
    dpid model_writelock::getId( void )
    {
        return this->t->getId();
    }

    //compare id
    bool model_writelock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //set comment
    void model_writelock::setComment( std::string *s )
    {
        this->t->setComment( s );
    }

    //get comment
    void model_writelock::getComment( std::string *s )
    {
        this->t->getComment( s );
    }

    //append to comment
    void model_writelock::appendComment( std::string *s )
    {
        this->t->appendComment( s );
    }

    
    //add component to list and trees
    void model_writelock::addComponent( model_component *c )
    {
        this->t->addComponent( c );
    }
    
    //add component, 1 parent
    void model_writelock::addComponent( model_component *c, dpid p1 )
    {
        this->t->addComponent( c, p1 );
    }
    
    //add component, 2 parents
    void model_writelock::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->t->addComponent( c, p1, p2 );
    }
    
    //find component by type and id
    model_component *model_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //remove component
    void model_writelock::removeComponent( model_component *c )
    {
        this->t->removeComponent( c );
    }
    
    //add vertex
    model_vertex *model_writelock::makeVertex( dpid id )
    {
        return this->t->makeVertex( id );
    }
    
    //find vertex
    model_vertex *model_writelock::findVertex( dpid id )
    {
        return this->t->findVertex( id );
    }
    
    //get vertexes
    void model_writelock::getVertexes( std::list<model_vertex *> *l )
    {
        this->t->getVertexes( l );
    }
    
    //add triangle
    model_triangle *model_writelock::makeTriangle( dpid id )
    {
        return this->t->makeTriangle( id );
    }
    
    //find triangle
    model_triangle *model_writelock::findTriangle( dpid id )
    {
        return this->findTriangle( id );
    }
    
    //get triangles
    void model_writelock::getTriangles( std::list<model_triangle *> *l )
    {
        this->t->getTriangles( l );
    }
    
    //add group
    model_group *model_writelock::makeGroup( dpid id )
    {
        return this->t->makeGroup( id );
    }
    
    //find group
    model_group *model_writelock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_writelock::getGroups( std::list<model_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //add triangle vertex
    model_triangle_vertex *model_writelock::makeTriangleVertex( dpid id, dpid triangle_id, dpid vertex_id )
    {
        return this->t->makeTriangleVertex( id, triangle_id, vertex_id );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_writelock::findTriangleVertex( dpid id )
    {
        return this->t->findTriangleVertex( id );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_writelock::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        return this->t->findTriangleVertex( triangle_id, vertex_id );
    }
    
    //get triangle vertexes
    void model_writelock::getTriangleVertexes( std::list<model_triangle_vertex *> *l )
    {
        this->t->getTriangleVertexes( l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_writelock::getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid )
    {
        this->t->getTriangleVertexes( l, pid );
    }
    
};
