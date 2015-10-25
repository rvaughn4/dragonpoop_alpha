
#ifndef dragonpoop_render_api_writelock_h
#define dragonpoop_render_api_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
#include "../window/window.h"

namespace dragonpoop
{
    class render_api;
    class render_api_context_ref;
    class render_api_context_writelock;
    class dpbitmap;
    class dpindex_buffer;
    class dpvertex_buffer;
    class render_api_texture_ref;
    class render_api_shader_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;

    class render_api_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api *t;
        
    protected:
        
        //ctor
        render_api_writelock( render_api *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_writelock( void );
        
    public:
        
        //run api
        void run( void );
        //returns true if window is open
        bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        //make context
        render_api_context_ref *getContext( void );
        //delete contexts
        void deleteContexts( void );
        //make shader
        render_api_shader_ref *makeShader( render_api_context_writelock *cl, const char *cname );
        //make texture
        render_api_texture_ref *makeTexture( render_api_context_writelock *cl, dpbitmap *bm );
        //make vertex buffer
        render_api_vertexbuffer_ref *makeVertexBuffer( render_api_context_writelock *cl, dpvertex_buffer *vb );
        //make index buffer
        render_api_indexbuffer_ref *makeIndexBuffer( render_api_context_writelock *cl, dpindex_buffer *vb );
        //delete shaders
        void deleteShaders( void );
        //delete textures
        void deleteTextures( void );
        //delete vertex buffers
        void deleteVertexBuffers( void );
        //delete index buffers
        void deleteIndexBuffers( void );
        //returns true if mouse input is waiting
        bool hasMouseInput( void );
        //fetches mouse input
        bool getMouseInput( window_mouse_input *m );
        //adds mouse input
        void addMouseInput( window_mouse_input *m );

        friend class render_api;
    };
    
};

#endif