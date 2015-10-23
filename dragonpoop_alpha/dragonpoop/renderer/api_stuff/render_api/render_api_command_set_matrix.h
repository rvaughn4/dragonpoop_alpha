
#ifndef dragonpoop_render_api_command_set_matrix_h
#define dragonpoop_render_api_command_set_matrix_h

#include "render_api_command.h"
#include "../../../gfx/dpmatrix/dpmatrix.h"

namespace dragonpoop
{
    
    class render_api_command_set_matrix : public render_api_command
    {
        
    private:
        
        dpmatrix m;
        
    protected:
        
    public:
        
        //ctor
        render_api_command_set_matrix( render_api_commandlist *l, dpmatrix *m );
        //dtor
        virtual ~render_api_command_set_matrix( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );
        
    };
    
};

#endif