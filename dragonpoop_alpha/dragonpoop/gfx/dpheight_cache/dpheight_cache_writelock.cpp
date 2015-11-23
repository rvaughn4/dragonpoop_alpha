
#include "dpheight_cache_writelock.h"
#include "dpheight_cache.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache_writelock::dpheight_cache_writelock( dpheight_cache *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    dpheight_cache_writelock::~dpheight_cache_writelock( void )
    {

    }

    //set camera position
    void dpheight_cache_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }

    //get camera position
    dpposition *dpheight_cache_writelock::getPosition( void )
    {
        return this->t->getPosition();
    }

    //get position time
    uint64_t dpheight_cache_writelock::getTime( void )
    {
        return this->t->getTime();
    }

    //set time
    void dpheight_cache_writelock::setTime( uint64_t t )
    {
        this->t->setTime( t );
    }

    //resize and set center
    void dpheight_cache_writelock::setDimensions( double w, double h, double x, double z, double tile_size )
    {
        this->t->setDimensions( w, h, x, z, tile_size );
    }

    //set height at coord
    void dpheight_cache_writelock::setHeight( double x, double z, float h )
    {
        this->t->setHeight( x, z, h );
    }

    //get height at coord
    float dpheight_cache_writelock::getHeight( double x, double z )
    {
        return this->t->getHeight( x, z );
    }

    //clear
    void dpheight_cache_writelock::clear( void )
    {
        this->t->clear();
    }

    //sync
    void dpheight_cache_writelock::sync( dpheight_cache_ref *r )
    {
        this->t->sync( r );
    }

};
