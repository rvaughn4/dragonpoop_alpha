
#ifndef dragonpoop_model_writelock_h
#define dragonpoop_model_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class model;
    class core;
    class dpthread_lock;
    class model_component;
    class model_vertex;
    class model_triangle;

    class model_writelock : public shared_obj_writelock
    {

    private:

        model *t;

    protected:

        //ctor
        model_writelock( model *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_writelock( void );

    public:

        //return core
        core *getCore( void );
        //run model from task
        void run( dpthread_lock *thd );
        //set name
        void setName( std::string *sname );
        //get name
        void getName( std::string *sname );
        //compare name
        bool compareName( std::string *sname );
        //set id
        void setId( dpid id );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //set comment
        void setComment( std::string *s );
        //get comment
        void getComment( std::string *s );
        //append to comment
        void appendComment( std::string *s );
        //add component to list and trees
        void addComponent( model_component *c );
        //add component, 1 parent
        void addComponent( model_component *c, dpid p1 );
        //add component, 2 parents
        void addComponent( model_component *c, dpid p1, dpid p2 );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //remove component
        void removeComponent( model_component *c );
        //add vertex
        model_vertex *makeVertex( dpid id );
        //find vertex
        model_vertex *findVertex( dpid id );
        //get vertexes
        void getVertexes( std::list<model_vertex *> *l );
        //add triangle
        model_triangle *makeTriangle( dpid id );
        //find triangle
        model_triangle *findTriangle( dpid id );
        //get triangles
        void getTriangles( std::list<model_triangle *> *l );

        friend class model;
    };
    
};

#endif