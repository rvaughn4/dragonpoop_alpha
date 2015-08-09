
#ifndef dragonpoop_openglx_1o5_renderer_model_instance_h
#define dragonpoop_openglx_1o5_renderer_model_instance_h

#include "../../../renderer_model/renderer_model_instance/renderer_model_instance.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model_instance : public renderer_model_instance
    {
        
    private:
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //handle sync
        virtual void onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml );
        //genertae group
        virtual renderer_model_instance_group *genGroup( model_instance_writelock *ml, model_instance_group *g );
        
    public:
        
        //ctor
        openglx_1o5_renderer_model_instance( model_instance_writelock *ml );
        //dtor
        virtual ~openglx_1o5_renderer_model_instance( void );
        
        friend class openglx_1o5_renderer_model_instance_readlock;
        friend class openglx_1o5_renderer_model_instance_writelock;
        
    };
    
};

#endif