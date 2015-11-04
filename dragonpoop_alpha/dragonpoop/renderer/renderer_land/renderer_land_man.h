
#ifndef dragonpoop_renderer_land_man_h
#define dragonpoop_renderer_land_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../../gfx/dpmatrix/dpmatrix.h"

#include <atomic>

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class renderer_land_man_ref;
    class renderer_land_man_readlock;
    class renderer_land_man_writelock;
    class gfx_ref;
    class dptask;
    class renderer_land_man_task;
    class dptaskpool_writelock;
    class gfx;
    class renderer_land;
    class renderer_land_ref;
    class dptaskpool_ref;
    class renderer;
    class renderer_ref;
    class dpland_man_ref;
    class renderer_writelock;
    class dpmatrix;
    class dpland_writelock;
    class render_api_context_ref;
    class render_api_commandlist_ref;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class render_api_shader_ref;
    class dpthread_singletask;
    class renderer_commandlist_passer;
    class renderer_commandlist_passer_ref;
    
    class renderer_land_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        renderer_land_man_task *gtsk;
        std::list<renderer_land *> lands;
        dptaskpool_ref *tpr;
        renderer_ref *r;
        dpland_man_ref *g_lands;
        uint64_t t_last_land_synced;
        render_api_context_ref *ctx;
        render_api_commandlist_ref *clist;
        dpmatrix m, m_undo;
        float log_screen_width, log_screen_height;
        dpthread_singletask *thd;
        renderer_commandlist_passer_ref *clpasser;
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay, renderer *r );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //sync lands
        void sync( dpthread_lock *thd );
        //run lands
        void runLands( dpthread_lock *thd );
        //render into command list
        void render( dpthread_lock *thd );
        //compute matrix
        void computeMatrix( void );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //delete lands
        void deletelands( void );
        //render lands
        void renderLands( dpthread_lock *thd, renderer_land_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl );
        //generate renderer land
        virtual renderer_land *genLand( dpland *ml );
        
    public:
        
        //ctor
        renderer_land_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height );
        //dtor
        virtual ~renderer_land_man( void );
        //return core
        core *getCore( void );
        //run from manager thread
        void run( dpthread_lock *thd );
        
        friend class renderer_land_man_readlock;
        friend class renderer_land_man_writelock;
        
    };
    
};

#endif