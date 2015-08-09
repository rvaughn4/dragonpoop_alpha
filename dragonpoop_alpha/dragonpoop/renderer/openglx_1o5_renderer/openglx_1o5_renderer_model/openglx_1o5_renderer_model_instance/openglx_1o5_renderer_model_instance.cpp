
#include "openglx_1o5_renderer_model_instance.h"
#include "openglx_1o5_renderer_model_instance_ref.h"
#include "openglx_1o5_renderer_model_instance_readlock.h"
#include "openglx_1o5_renderer_model_instance_writelock.h"

#include <iostream>
namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance::openglx_1o5_renderer_model_instance( model_instance_writelock *ml ) : renderer_model_instance( ml )
    {
        
    }
    
    //dtor
    openglx_1o5_renderer_model_instance::~openglx_1o5_renderer_model_instance( void )
    {
        
    }
    
    //generate read lock
    shared_obj_readlock *openglx_1o5_renderer_model_instance::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new openglx_1o5_renderer_model_instance_readlock( (openglx_1o5_renderer_model_instance *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *openglx_1o5_renderer_model_instance::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new openglx_1o5_renderer_model_instance_writelock( (openglx_1o5_renderer_model_instance *)p, l );
    }
    
    //generate ref
    shared_obj_ref *openglx_1o5_renderer_model_instance::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new openglx_1o5_renderer_model_instance_ref( (openglx_1o5_renderer_model_instance *)p, k );
    }
    
    //handle sync
    void openglx_1o5_renderer_model_instance::onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml )
    {
        std::cout << "gl model instance\r\n";
    }
    
    //genertae group
    renderer_model_instance_group *openglx_1o5_renderer_model_instance::genGroup( model_instance_group *g )
    {
        return 0;
    }
    
};
