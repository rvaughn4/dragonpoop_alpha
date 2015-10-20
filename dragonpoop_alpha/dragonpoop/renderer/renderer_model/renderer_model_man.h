
#ifndef dragonpoop_renderer_model_man_h
#define dragonpoop_renderer_model_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

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
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        
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
        void renderModels( dpthread_lock *thd, renderer_writelock *rl, renderer_model_man_readlock *ml, dpmatrix *m_world );
        //generate renderer model
        virtual renderer_model *genModel( model_writelock *ml );
        //sync models
        static void sync( dpthread_lock *thd, renderer_model_man_ref *g );
        
    public:
        
        //ctor
        renderer_model_man( core *c, renderer *r, dptaskpool_writelock *tp );
        //dtor
        virtual ~renderer_model_man( void );
        //return core
        core *getCore( void );
        //run from manager thread
        static void runFromTask( dpthread_lock *thd, renderer_model_man_ref *g );
        //run from renderer thread
        static void runFromRenderer( dpthread_lock *thd, renderer_model_man *g );
        
        friend class renderer_model_man_readlock;
        friend class renderer_model_man_writelock;
        
    };
    
};

#endif