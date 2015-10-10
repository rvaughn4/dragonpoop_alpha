
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

    //render model instance group
    void renderer_writelock::renderGroup( dpthread_lock *thd, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world )
    {
        this->t->renderGroup( thd, this, m, mi, g, mat, m_world );
    }
    
    //render gui
    void renderer_writelock::renderGui( dpthread_lock *thd, renderer_gui_readlock *m, dpmatrix *m_world )
    {
        this->t->renderGui( thd, this, m, m_world );
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
    
    //return guis
    void renderer_writelock::getChildrenGuis( std::list<renderer_gui *> *l, dpid pid )
    {
        this->t->getChildrenGuis( l, pid );
    }
    
    //get hovering gui id
    dpid renderer_writelock::getHoverId( void )
    {
        return this->t->getHoverId();
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
    
};
