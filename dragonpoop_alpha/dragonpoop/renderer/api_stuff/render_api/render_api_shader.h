
#ifndef dragonpoop_render_api_shader_h
#define dragonpoop_render_api_shader_h

#include "render_api.h"

namespace dragonpoop
{
    
    class render_api_ref;
    class render_api_writelock;
    class render_api_context_writelock;
    class render_api_context_ref;
    
    class render_api_shader : public shared_obj
    {
        
    private:
        
        render_api_ref *r;
        render_api_context_ref *c;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //ctor
        render_api_shader( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm );
        //dtor
        virtual ~render_api_shader( void );
        
        friend class render_api_shader_ref;
        friend class render_api_shader_readlock;
        friend class render_api_shader_writelock;
    };
    
};

#endif