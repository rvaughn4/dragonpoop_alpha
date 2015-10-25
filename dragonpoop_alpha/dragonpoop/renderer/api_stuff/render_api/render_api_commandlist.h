
#ifndef dragonpoop_render_api_commandlist_h
#define dragonpoop_render_api_commandlist_h

#include "render_api.h"
#include "../../../gfx/dpmatrix/dpmatrix.h"

#include <atomic>

namespace dragonpoop
{
    
    class render_api_ref;
    class render_api_context_ref;
    class render_api_context_writelock;
    class render_api_shader_ref;
    class render_api_texture_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;
    class render_api_command;

    class render_api_commandlist : public shared_obj
    {
        
    private:
        
        std::atomic<bool> bIsCompiled;
        std::list<render_api_command *> cmds;
        render_api_shader_ref *sdr;
        render_api_texture_ref *t0, *t1;
        render_api_vertexbuffer_ref *vb;
        render_api_indexbuffer_ref *ib;
        dpmatrix m;
        float alpha;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //returns true if ready to be executed
        bool isReadyForExecute( void );
        //compile command list
        void compile( render_api_context_writelock *ctx );
        //called at begin of compile
        virtual bool beginCompile( render_api_context_writelock *ctx );
        //called at end of compile
        virtual void endCompile( render_api_context_writelock *ctx );
        //called during compile for each draw call
        virtual void drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        //execute command list
        virtual void execute( render_api_context_writelock *r );
        //set current matrix
        void cmd_setMatrix( dpmatrix *m );
        //set current shader
        void cmd_setShader( render_api_shader_ref *r );
        //set current texture 0
        void cmd_setTexture0( render_api_texture_ref *r );
        //set current texture 1
        void cmd_setTexture1( render_api_texture_ref *r );
        //set current vertex buffer
        void cmd_setVertexBuffer( render_api_vertexbuffer_ref *r );
        //set current index buffer
        void cmd_setIndexBuffer( render_api_indexbuffer_ref *r );
        //set current alpha
        void cmd_setAlpha( float a );
        //draw
        void cmd_draw( render_api_context_writelock *ctx );
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
        
    public:
        
        //ctor
        render_api_commandlist( dpmutex_master *mm );
        //dtor
        virtual ~render_api_commandlist( void );
      
        friend class render_api_commandlist_ref;
        friend class render_api_commandlist_readlock;
        friend class render_api_commandlist_writelock;
        friend class render_api_command;
    };
    
};

#endif