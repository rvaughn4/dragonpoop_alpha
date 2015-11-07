
#include "dpland_man_writelock.h"
#include "dpland_man.h"

namespace dragonpoop
{

    //ctor
    dpland_man_writelock::dpland_man_writelock( dpland_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpland_man_writelock::~dpland_man_writelock( void )
    {
        
    }
    
    //return core
    core *dpland_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void dpland_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //get tiles
    void dpland_man_writelock::getTiles( std::list<dpland *> *l )
    {
        this->t->getTiles( l );
    }

};
