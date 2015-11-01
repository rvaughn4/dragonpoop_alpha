
#ifndef dragonpoop_render_api_commandlist_writelock_h
#define dragonpoop_render_api_commandlist_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_commandlist;
    class render_api_context_ref;
    class render_api_context_writelock;
    class render_api_shader_ref;
    class render_api_texture_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;
    class dpmatrix;
    
    class render_api_commandlist_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api_commandlist *t;
        
    protected:
        
        //ctor
        render_api_commandlist_writelock( render_api_commandlist *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_commandlist_writelock( void );
        
    public:
        
        //compile command list
        bool compile( render_api_context_writelock *ctx );
        //execute command list
        bool execute( render_api_context_writelock *r );
        //delete commands
        void deleteCommands( void );
        //set shader command
        void setShader( render_api_shader_ref *p );
        //set texture command
        void setTexture( render_api_texture_ref *p, int level );
        //set vertex buffer command
        void setVertexBuffer( render_api_vertexbuffer_ref *p );
        //set index buffer command
        void setIndexBuffer( render_api_indexbuffer_ref *p );
        //set matrix command
        void setMatrix( dpmatrix *m );
        //set alpha command
        void setAlpha( float a );
        //draw command
        void draw( void );
        
        friend class render_api_commandlist;
    };
    
};

#endif