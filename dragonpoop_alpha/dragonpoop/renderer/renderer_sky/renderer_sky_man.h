
#ifndef dragonpoop_renderer_sky_man_h
#define dragonpoop_renderer_sky_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../../gfx/dpmatrix/dpmatrix.h"
#include "../../gfx/dpposition/dpposition.h"

#include <atomic>

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class renderer_sky_man_ref;
    class renderer_sky_man_readlock;
    class renderer_sky_man_writelock;
    class gfx_ref;
    class dptask;
    class renderer_sky_man_task;
    class dptaskpool_writelock;
    class gfx;
    class dptaskpool_ref;
    class renderer;
    class renderer_ref;
    class dpsky_man_ref;
    class renderer_writelock;
    class dpmatrix;
    class dpsky_writelock;
    class render_api_context_ref;
    class render_api_commandlist_ref;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class render_api_shader_ref;
    class dpthread_singletask;
    class renderer_commandlist_passer;
    class renderer_commandlist_passer_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;
    class render_api_texture_ref;
    
    class renderer_sky_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        renderer_sky_man_task *gtsk;
        dptaskpool_ref *tpr;
        renderer_ref *r;
        dpsky_man_ref *g_skys;
        uint64_t t_last_sky_synced;
        render_api_context_ref *ctx;
        render_api_commandlist_ref *clist;
        dpmatrix m, m_undo;
        float log_screen_width, log_screen_height;
        dpthread_singletask *thd;
        renderer_commandlist_passer_ref *clpasser;
        
        struct
        {
            struct
            {
                struct
                {
                    render_api_vertexbuffer_ref *vb;
                    render_api_indexbuffer_ref *ib;
                } front, back, left, right, top, bottom;
            } skybox;
            
            struct
            {
                struct
                {
                    render_api_texture_ref *front, *back, *top, *bottom, *left, *right;
                } stars, mask;
            } skyboxtex;
            
        } stuff;
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //sync skys
        void sync( dpthread_lock *thd );
        //render into command list
        void render( dpthread_lock *thd );
        //compute matrix
        void computeMatrix( void );
        //run sky
        void runSky( dpthread_lock *thd );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //delete skys
        void deleteSky( void );
        //run from manager thread
        void run( dpthread_lock *thd, renderer_sky_man_writelock *l );
        
    public:
        
        //ctor
        renderer_sky_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height );
        //dtor
        virtual ~renderer_sky_man( void );
        //return core
        core *getCore( void );
        
        friend class renderer_sky_man_readlock;
        friend class renderer_sky_man_writelock;
        
    };
    
};

#endif