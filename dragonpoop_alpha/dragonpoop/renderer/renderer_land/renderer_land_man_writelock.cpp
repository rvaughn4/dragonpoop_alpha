
#include "renderer_land_man_writelock.h"
#include "renderer_land_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_land_man_writelock::renderer_land_man_writelock( renderer_land_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_land_man_writelock::~renderer_land_man_writelock( void )
    {
        
    }
    
    //return core
    core *renderer_land_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //render lands
    void renderer_land_man_writelock::renderLands( dpthread_lock *thd, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl )
    {
        this->t->renderLands( thd, this, m_world, ctx, cl );
    }
    
    //run from manager thread
    void renderer_land_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
