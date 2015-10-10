
#include "openglx_1o5_renderer_model.h"
#include "openglx_1o5_renderer_model_ref.h"
#include "openglx_1o5_renderer_model_readlock.h"
#include "openglx_1o5_renderer_model_writelock.h"
#include "openglx_1o5_renderer_model_instance/openglx_1o5_renderer_model_instance.h"
#include "openglx_1o5_renderer_model_material/openglx_1o5_renderer_model_material.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model::openglx_1o5_renderer_model( model_writelock *ml, openglx_1o5_renderer *r ) : renderer_model( ml )
    {
        this->r = r;
    }
    
    //dtor
    openglx_1o5_renderer_model::~openglx_1o5_renderer_model( void )
    {
        
    }
    
    //generate read lock
    shared_obj_readlock *openglx_1o5_renderer_model::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new openglx_1o5_renderer_model_readlock( (openglx_1o5_renderer_model *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *openglx_1o5_renderer_model::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new openglx_1o5_renderer_model_writelock( (openglx_1o5_renderer_model *)p, l );
    }
    
    //generate ref
    shared_obj_ref *openglx_1o5_renderer_model::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new openglx_1o5_renderer_model_ref( (openglx_1o5_renderer_model*)p, k );
    }
    
    //handle sync
    void openglx_1o5_renderer_model::onSync( dpthread_lock *thd, renderer_model_writelock *g, model_writelock *ml )
    {
    }
    
    //generate instance
    renderer_model_instance *openglx_1o5_renderer_model::genInstance( model_instance_writelock *ml )
    {
        return new openglx_1o5_renderer_model_instance( ml );
    }

    //generate material
    renderer_model_material *openglx_1o5_renderer_model::genMaterial( model_writelock *ml, model_material *m )
    {
        return new openglx_1o5_renderer_model_material( ml, m, this->r );
    }
    
};
