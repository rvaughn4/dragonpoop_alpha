
#include "openglx_1o5_renderer_model_group_instance.h"
#include "../../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_group::openglx_1o5_renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd ) : renderer_model_instance_group( ml, g, thd )
    {
        this->last_frame_time = 0;
        this->glist = 0;
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_group::~openglx_1o5_renderer_model_instance_group( void )
    {
    }
    
    //sync with group
    void openglx_1o5_renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
        this->renderer_model_instance_group::sync( ml, g, thd );
    }
    
    //get last frame compute time
    uint64_t openglx_1o5_renderer_model_instance_group::getLastFrameTime( void )
    {
        return this->last_frame_time;
    }
    
    //set last frame time
    void openglx_1o5_renderer_model_instance_group::setLastFrameTime( uint64_t t )
    {
        this->last_frame_time = t;
    }
    
    //get dipslay list
    unsigned int openglx_1o5_renderer_model_instance_group::getList( void )
    {
        return this->glist;
    }
    
    //set display list
    void openglx_1o5_renderer_model_instance_group::setList( unsigned int l )
    {
        this->glist = l;
    }
    
};
