
#ifndef dragonpoop_render_api_commandlist_generic_h
#define dragonpoop_render_api_commandlist_generic_h

#include "render_api_commandlist.h"

namespace dragonpoop
{
    
    class render_api_commandlist_generic_batch;
    
    class render_api_commandlist_generic : public render_api_commandlist
    {
        
    private:
        
        std::list<render_api_commandlist_generic_batch *> cmds;
        
    protected:
        
        //called at begin of compile
        virtual bool beginCompile( render_api_context_writelock *ctx );
        //called at end of compile
        virtual void endCompile( render_api_context_writelock *ctx );
        //called during compile for each draw call
        virtual bool drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        //execute command list
        virtual bool execute( render_api_context_writelock *r );
        //execute batch
        virtual bool executeBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        //delete batches
        void deleteBatches( void );
        //generate batch
        virtual render_api_commandlist_generic_batch *genBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha );
        
    public:
        
        //ctor
        render_api_commandlist_generic( dpmutex_master *mm );
        //dtor
        virtual ~render_api_commandlist_generic( void );
        
        friend class render_api_commandlist_generic_batch;
    };
    
};

#endif