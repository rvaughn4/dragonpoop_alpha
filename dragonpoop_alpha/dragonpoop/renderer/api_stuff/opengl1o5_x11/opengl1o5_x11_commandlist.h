
#ifndef dragonpoop_opengl1o5_x11_commandlist_h
#define dragonpoop_opengl1o5_x11_commandlist_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_commandlist_generic.h"

#define opengl1o5_x11_commandlist_USE_DISPLAY_LIST

namespace dragonpoop
{
    
#ifdef opengl1o5_x11_commandlist_USE_DISPLAY_LIST
    class opengl1o5_x11_commandlist : public render_api_commandlist
#else
    class opengl1o5_x11_commandlist : public render_api_commandlist_generic
#endif
    {
        
    private:

#ifdef opengl1o5_x11_commandlist_USE_DISPLAY_LIST
        unsigned int dlist;
#endif
        
    protected:
        
        //called at begin of compile
        virtual bool beginCompile( render_api_context_writelock *ctx );
        //called at end of compile
        virtual void endCompile( render_api_context_writelock *ctx );
        //called during compile for each draw call
        virtual bool drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        //execute command list
        virtual bool execute( render_api_context_writelock *r );
        
    public:
        
        //ctor
        opengl1o5_x11_commandlist( dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_commandlist( void );
      
        friend class opengl1o5_x11_context;
    };
    
};

#endif