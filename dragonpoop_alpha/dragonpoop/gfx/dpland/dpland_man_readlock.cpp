
#include "dpland_man_readlock.h"
#include "dpland_man.h"

namespace dragonpoop
{
    
    //ctor
    dpland_man_readlock::dpland_man_readlock( dpland_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpland_man_readlock::~dpland_man_readlock( void )
    {
        
    }
    
    //return core
    core *dpland_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get tiles
    void dpland_man_readlock::getTiles( std::list<dpland *> *l )
    {
        this->t->getTiles( l );
    }
    
};
