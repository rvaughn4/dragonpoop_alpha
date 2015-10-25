
#include "renderer_writelock.h"
#include "renderer.h"

namespace dragonpoop
{

    //ctor
    renderer_writelock::renderer_writelock( renderer *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_writelock::~renderer_writelock( void )
    {

    }

    //returns true if running
    bool renderer_writelock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *renderer_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //stop renderer task
    void renderer_writelock::kill( void )
    {
        this->t->kill();
    }

    //run renderer from task
    void renderer_writelock::run( dptask_writelock *tskl, dpthread_lock *thd )
    {
        this->t->run( tskl, thd, this );
    }
    
    //returns fps
    float renderer_writelock::getFps( void )
    {
        return this->t->getFps();
    }
    
    //return ms each frame
    unsigned int renderer_writelock::getMsPerFrame( void )
    {
        return this->t->getMsPerFrame();
    }

    //get renderer name
    void renderer_writelock::getName( std::string *s )
    {
        this->t->getName( s );
    }
    
    //get texture memory used
    unsigned int renderer_writelock::getTextureMemory( void )
    {
        return this->t->getTextureMemory();
    }
    
    //get vertex memory used
    unsigned int renderer_writelock::getVertexMemory( void )
    {
        return this->t->getVertexMemory();
    }
    
    //gets camera position
    void renderer_writelock::getCameraPosition( dpposition *p )
    {
        this->t->getCameraPosition( p );
    }
    
    //sync camera position
    void renderer_writelock::syncCamera( void )
    {
        this->t->syncCamera();
    }
    
    //get position difference from camera
    void renderer_writelock::getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz )
    {
        this->t->getPositionRelativeToCamera( in_obj_pos, t_epoch_now, out_xyz );
    }
    
    //returns true if gui commandlist is set
    bool renderer_writelock::isGuiCommandListUploaded( void )
    {
        return this->t->isGuiCommandListUploaded();
    }
    
    //set gui commandlist
    void renderer_writelock::uploadGuiCommandList( render_api_commandlist_ref *cl )
    {
        this->t->uploadGuiCommandList( cl );
    }
 
};
