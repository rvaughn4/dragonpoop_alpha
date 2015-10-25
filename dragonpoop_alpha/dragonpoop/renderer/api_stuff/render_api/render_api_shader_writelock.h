
#ifndef dragonpoop_render_api_shader_writelock_h
#define dragonpoop_render_api_shader_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_shader;
    class render_api_context_ref;
    class render_api_texture_ref;
    class render_api_indexbuffer_ref;
    class render_api_vertexbuffer_ref;
    class dpmatrix;

    class render_api_shader_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api_shader *t;
        
    protected:
        
        //ctor
        render_api_shader_writelock( render_api_shader *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_shader_writelock( void );
        
    public:
        
        //render vb
        void render( render_api_context_ref *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m );
        
        
        friend class render_api_shader;
    };
    
};

#endif