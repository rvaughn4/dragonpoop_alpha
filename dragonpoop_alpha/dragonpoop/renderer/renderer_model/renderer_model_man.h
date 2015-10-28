
#ifndef dragonpoop_renderer_model_man_h
#define dragonpoop_renderer_model_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../../gfx/dpmatrix/dpmatrix.h"
#include "../../gfx/dpposition/dpposition.h"

#include <atomic>

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class renderer_model_man_ref;
    class renderer_model_man_readlock;
    class renderer_model_man_writelock;
    class gfx_ref;
    class dptask;
    class renderer_model_man_task;
    class dptaskpool_writelock;
    class gfx;
    class renderer_model;
    class renderer_model_ref;
    class dptaskpool_ref;
    class renderer;
    class renderer_ref;
    class model_man_ref;
    class renderer_writelock;
    class dpmatrix;
    class model_writelock;
    class render_api_context_ref;
    class render_api_commandlist_ref;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class render_api_shader_ref;
    class dpposition;
    class dpthread_singletask;
    
    class renderer_model_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        renderer_model_man_task *gtsk;
        std::list<renderer_model *> models;
        dptaskpool_ref *tpr;
        renderer_ref *r;
        model_man_ref *g_models;
        uint64_t t_last_model_synced;
        render_api_context_ref *ctx;
        render_api_shader_ref *sdr;
        render_api_commandlist_ref *clist;
        dpmatrix m, m_undo;
        float log_screen_width, log_screen_height;
        dpposition campos;
        std::atomic<bool> listReady;
        dpthread_singletask *thd;

        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay, renderer *r );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //sync models
        static void sync( dpthread_lock *thd, renderer_model_man_ref *g );
        //delete old models
        static void deleteOldModels( dpthread_lock *thd, renderer_model_man_ref *g );
        //run models
        static void runModels( dpthread_lock *thd, renderer_model_man_ref *g );
        //render into command list
        static void render( dpthread_lock *thd, renderer_model_man_ref *g );
        //wait for renderer to finish with commandlist
        static bool waitForRenderer( renderer_model_man_ref *g );
        //swap command list with renderer
        static void swapList( renderer_model_man_ref *g, renderer_ref *r );
        //compute matrix
        void computeMatrix( void );

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //delete models
        void deleteModels( void );
        //render models
        void renderModels( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //generate renderer model
        virtual renderer_model *genModel( model_writelock *ml );
        //called by renderer to announce that commandlist was consumed
        void listConsumed( void );

    public:
        
        //ctor
        renderer_model_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, float log_screen_width, float log_screen_height );
        //dtor
        virtual ~renderer_model_man( void );
        //return core
        core *getCore( void );
        //run from manager thread
        static void run( dpthread_lock *thd, renderer_model_man_ref *g, renderer_ref *r );

        friend class renderer_model_man_readlock;
        friend class renderer_model_man_writelock;
        
    };
    
};

#endif