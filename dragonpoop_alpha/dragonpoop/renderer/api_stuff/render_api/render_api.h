
#ifndef dragonpoop_render_api_h
#define dragonpoop_render_api_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpmutex/dpmutex_master.h"
#include "../window/window.h"
#include <list>

namespace dragonpoop
{
    
    class render_api_context;
    class render_api_context_ref;
    class render_api_writelock;
    class render_api_shader;
    class render_api_texture;
    class render_api_vertexbuffer;
    class render_api_indexbuffer;
    class render_api_shader_ref;
    class render_api_texture_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;
    class dpbitmap;
    class render_api_context_writelock;
    class dpvertex_buffer;
    class dpindex_buffer;
    
    class render_api : public shared_obj
    {
        
    private:
        
        window *w;
        dpmutex_master *mm;
        std::list<render_api_context *> contexts;
        std::list<render_api_shader *> shaders;
        std::list<render_api_texture *> textures;
        std::list<render_api_vertexbuffer *> vertexbuffers;
        std::list<render_api_indexbuffer *> indexbuffers;
        int crun;
        
        //run contexts
        void runContexts( void );
        //run shaders
        void runShaders( void );
        //run textures
        void runTextures( void );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //returns window
        window *getWindow( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        //generate context
        virtual render_api_context *genContext( render_api_writelock *al, dpmutex_master *mm );
        //make context
        render_api_context_ref *getContext( render_api_writelock *al );
        //delete contexts
        void deleteContexts( void );
        //gen shader
        virtual render_api_shader *genShader( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, const char *cname );
        //gen texture
        virtual render_api_texture *genTexture( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm );
        //gen vertex buffer
        virtual render_api_vertexbuffer *genVertexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb );
        //gen index buffer
        virtual render_api_indexbuffer *genIndexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *vb );
        //make shader
        render_api_shader_ref *makeShader( render_api_writelock *al, render_api_context_writelock *cl, const char *cname );
        //make texture
        render_api_texture_ref *makeTexture( render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm );
        //make vertex buffer
        render_api_vertexbuffer_ref *makeVertexBuffer( render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb );
        //make index buffer
        render_api_indexbuffer_ref *makeIndexBuffer( render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *vb );
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
        //returns true if kb input is waiting
        bool hasKBInput( void );
        //fetches kb input
        bool getKBInput( window_kb_input *m );
        //adds kb input
        void addKBInput( window_kb_input *m );

    public:
        
        //ctor
        render_api( window *w, dpmutex_master *mm );
        //dtor
        virtual ~render_api( void );

        friend class render_api_ref;
        friend class render_api_readlock;
        friend class render_api_writelock;
    };
    
};

#endif