
#include "renderer_model_man_writelock.h"
#include "renderer_model_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man_writelock::renderer_model_man_writelock( renderer_model_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_man_writelock::~renderer_model_man_writelock( void )
    {
        
    }
    
    //return core
    core *renderer_model_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //delete models
    void renderer_model_man_writelock::deleteModels( void )
    {
        this->t->deleteModels();
    }
    
    //render models
    void renderer_model_man_writelock::renderModels( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        this->t->renderModels( thd, campos, m_world, ctx, clist );
    }
    
    //called by renderer to announce that commandlist was consumed
    void renderer_model_man_writelock::listConsumed( void )
    {
        this->t->listConsumed();
    }
    
};
