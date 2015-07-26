
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
    
};
