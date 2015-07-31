
#include "model_readlock.h"
#include "model.h"

namespace dragonpoop
{

    //ctor
    model_readlock::model_readlock( model *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    model_readlock::~model_readlock( void )
    {

    }

    //return core
    core *model_readlock::getCore( void )
    {
        return this->t->getCore();
    }

    //get name
    void model_readlock::getName( std::string *sname )
    {
        this->t->getName( sname );
    }

    //compare name
    bool model_readlock::compareName( std::string *sname )
    {
        return this->t->compareName( sname );
    }

    //get id
    dpid model_readlock::getId( void )
    {
        return this->t->getId();
    }

    //compare id
    bool model_readlock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //get comment
    void model_readlock::getComment( std::string *s )
    {
        this->t->getComment( s );
    }
    
    //find component by type and id
    model_component *model_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
};
