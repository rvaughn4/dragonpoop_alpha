
#ifndef dragonpoop_render_api_shader_readlock_h
#define dragonpoop_render_api_shader_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class render_api_shader;
    class render_api_context_ref;
    class render_api_texture_ref;
    class render_api_indexbuffer_ref;
    class render_api_vertexbuffer_ref;
    class dpmatrix;
    class render_api_context_writelock;

    class render_api_shader_readlock : public shared_obj_readlock
    {
        
    private:
        
        render_api_shader *t;
        
    protected:
        
        //ctor
        render_api_shader_readlock( render_api_shader *t, dpmutex_readlock *l );
        //dtor
        virtual ~render_api_shader_readlock( void );
        
    public:
        
        //render vb
        bool render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha, dpmatrix *m_world );
        
        friend class render_api_shader;
    };
    
};

#endif