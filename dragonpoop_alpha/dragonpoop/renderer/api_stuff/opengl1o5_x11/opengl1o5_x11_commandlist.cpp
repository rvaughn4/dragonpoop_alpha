
#include "opengl1o5_x11_commandlist.h"
#include "../render_api/render_api_context_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_shader_readlock.h"
#include "../render_api/render_api_shader_ref.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_commandlist::opengl1o5_x11_commandlist( dpmutex_master *mm ) : render_api_commandlist_generic( mm )
    {
    }
    
    //dtor
    opengl1o5_x11_commandlist::~opengl1o5_x11_commandlist( void )
    {
    }
        
};
