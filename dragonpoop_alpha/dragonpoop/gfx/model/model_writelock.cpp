
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

};
