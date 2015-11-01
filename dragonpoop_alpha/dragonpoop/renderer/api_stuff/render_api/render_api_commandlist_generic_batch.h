
#ifndef dragonpoop_render_api_commandlist_generic_batch_h
#define dragonpoop_render_api_commandlist_generic_batch_h

#include "render_api_commandlist_generic.h"

namespace dragonpoop
{
    
    class render_api_commandlist_generic;
    
    class render_api_commandlist_generic_batch
    {
        
    private:
        
        render_api_commandlist_generic *l;
        render_api_shader_ref *sdr;
        render_api_texture_ref *t0;
        render_api_texture_ref *t1;
        render_api_vertexbuffer_ref *vb;
        render_api_indexbuffer_ref *ib;
        dpmatrix m;
        float alpha;
        int tex_index;
        
    protected:

        //execute batch
        virtual bool execute( render_api_context_writelock *ctx );
        
    public:
        
        //ctor
        render_api_commandlist_generic_batch( render_api_commandlist_generic *l, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        //dtor
        virtual ~render_api_commandlist_generic_batch( void );
        
        friend class render_api_commandlist_generic;
        
    };
    
};

#endif