
#include "opengl1o5_x11_commandlist.h"
#include "../render_api/render_api_context_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_shader_readlock.h"
#include "../render_api/render_api_shader_ref.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_commandlist::opengl1o5_x11_commandlist( dpmutex_master *mm ) : render_api_commandlist( mm )
    {
        this->dlist = 0;
    }
    
    //dtor
    opengl1o5_x11_commandlist::~opengl1o5_x11_commandlist( void )
    {
        if( this->dlist )
            glDeleteLists( this->dlist, 1 );
    }
    
    //called at begin of compile
    bool opengl1o5_x11_commandlist::beginCompile( render_api_context_writelock *ctx )
    {
        ctx->makeActive();
        
        if( this->dlist )
            glDeleteLists( this->dlist, 1 );
        
        this->dlist = glGenLists( 1 );
        if( !this->dlist )
            return 0;
        
        glNewList( this->dlist, GL_COMPILE );
        return 1;
    }
    
    //called at end of compile
    void opengl1o5_x11_commandlist::endCompile( render_api_context_writelock *ctx )
    {
        glEndList();
        glFinish();
    }
    
    //execute command list
    void opengl1o5_x11_commandlist::execute( render_api_context_writelock *r )
    {
        if( !this->dlist )
            return;
        glCallList( this->dlist );
    }
    
};
