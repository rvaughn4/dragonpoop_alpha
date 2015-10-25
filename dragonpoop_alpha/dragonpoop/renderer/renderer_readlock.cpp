
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
    
    //get position difference from camera
    void renderer_readlock::getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz )
    {
        this->t->getPositionRelativeToCamera( in_obj_pos, t_epoch_now, out_xyz );
    }

    //returns true if gui commandlist is set
    bool renderer_readlock::isGuiCommandListUploaded( void )
    {
        return this->t->isGuiCommandListUploaded();
    }
 
};
