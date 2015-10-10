
#include "renderer_readlock.h"
#include "renderer.h"

namespace dragonpoop
{

    //ctor
    renderer_readlock::renderer_readlock( renderer *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_readlock::~renderer_readlock( void )
    {

    }

    //returns true if running
    bool renderer_readlock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *renderer_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //returns fps
    float renderer_readlock::getFps( void )
    {
        return this->t->getFps();
    }
    
    //return ms each frame
    unsigned int renderer_readlock::getMsPerFrame( void )
    {
        return this->t->getMsPerFrame();
    }
    
    //get renderer name
    void renderer_readlock::getName( std::string *s )
    {
        this->t->getName( s );
    }
    
    //get texture memory used
    unsigned int renderer_readlock::getTextureMemory( void )
    {
        return this->t->getTextureMemory();
    }
    
    //get vertex memory used
    unsigned int renderer_readlock::getVertexMemory( void )
    {
        return this->t->getVertexMemory();
    }
    
    //gets camera position
    void renderer_readlock::getCameraPosition( dpposition *p )
    {
        this->t->getCameraPosition( p );
    }

};
