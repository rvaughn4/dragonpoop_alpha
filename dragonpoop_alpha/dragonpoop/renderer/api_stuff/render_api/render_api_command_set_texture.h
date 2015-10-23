
#ifndef dragonpoop_render_api_command_set_texture_h
#define dragonpoop_render_api_command_set_texture_h

#include "render_api_command.h"

namespace dragonpoop
{
    
    class render_api_texture_writelock;
    class render_api_texture_ref;
    class render_api_texture;
    
    class render_api_command_set_texture : public render_api_command
    {
        
    private:
        
        render_api_texture_ref *r;
        int tex_index;
        
    protected:
        
    public:
        
        //ctor
        render_api_command_set_texture( render_api_commandlist *l, render_api_texture *b, int tex_index );
        //ctor
        render_api_command_set_texture( render_api_commandlist *l, render_api_texture_ref *b, int tex_index );
        //ctor
        render_api_command_set_texture( render_api_commandlist *l, render_api_texture_writelock *b, int tex_index );
        //dtor
        virtual ~render_api_command_set_texture( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );
        
    };
    
};

#endif