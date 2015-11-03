
#ifndef dragonpoop_opengl1o5_x11_commandlist_generic_h
#define dragonpoop_opengl1o5_x11_commandlist_generic_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_commandlist_generic.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_commandlist_generic : public render_api_commandlist_generic
    {
        
    private:
        
    protected:
        
        //generate batch
        virtual render_api_commandlist_generic_batch *genBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );

    public:
        
        //ctor
        opengl1o5_x11_commandlist_generic( dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_commandlist_generic( void );
        
        friend class opengl1o5_x11_context;
    };
    
};

#endif