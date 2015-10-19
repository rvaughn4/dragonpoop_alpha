
#include "renderer_state.h"
#include "renderer.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state::renderer_state( renderer *r )
    {
        this->r = r;
    }
    
    //dtor
    renderer_state::~renderer_state( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        return 0;
    }

    //run renderer
    void renderer_state::_run( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_run( thd, rl );
    }
    
    //init api
    bool renderer_state::initApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        return this->r->state_initApi( thd, rl );
    }
    
    //init gui manager
    bool renderer_state::initGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        return this->r->state_initGui( thd, rl );
    }
    
    //init model manager
    bool renderer_state::initModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        return this->r->state_initModel( thd, rl );
    }
    
    //start api
    void renderer_state::startApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_startApi( thd, rl );
    }
    
    //start gui manager
    void renderer_state::startGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_startGui( thd, rl );
    }
    
    //start model manager
    void renderer_state::startModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_startModel( thd, rl );
    }
    
    //stop api
    void renderer_state::stopApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_stopApi( thd, rl );
    }
    
    //stop gui manager
    void renderer_state::stopGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_stopGui( thd, rl );
    }
    
    //stop model manager
    void renderer_state::stopModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_stopModel( thd, rl );
    }
    
    //deinit api
    void renderer_state::deinitApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_deinitApi( thd, rl );
    }
    
    //init gui manager
    void renderer_state::deinitGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_deinitGui( thd, rl );
    }
    
    //init model manager
    void renderer_state::deinitModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->r->state_deinitModel( thd, rl );
    }
    
    //kill renderer
    void renderer_state::kill( void )
    {
        this->r->bIsRun = 0;
    }
    
    //returns true if should run
    bool renderer_state::doRun( void )
    {
        return this->r->bDoRun;
    }
    
};
