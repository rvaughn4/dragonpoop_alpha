
#ifndef dragonpoop_openglx_1o5_renderer_model_h
#define dragonpoop_openglx_1o5_renderer_model_h

#include "../../renderer_model/renderer_model.h"

namespace dragonpoop
{
    class openglx_1o5_renderer;
   
    class openglx_1o5_renderer_model : public renderer_model
    {
        
    private:
        
        openglx_1o5_renderer *r;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //handle sync
        virtual void onSync( dpthread_lock *thd, renderer_model_writelock *g, model_writelock *ml );
        //generate instance
        virtual renderer_model_instance *genInstance( model_instance_writelock *ml );
        //generate material
        virtual renderer_model_material *genMaterial( model_writelock *ml, model_material *m );
        
    public:
        
        //ctor
        openglx_1o5_renderer_model( model_writelock *ml, openglx_1o5_renderer *r );
        //dtor
        virtual ~openglx_1o5_renderer_model( void );
        
        friend class openglx_1o5_renderer_model_readlock;
        friend class openglx_1o5_renderer_model_writelock;
        
    };
    
};

#endif