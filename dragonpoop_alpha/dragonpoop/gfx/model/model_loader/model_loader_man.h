
#ifndef dragonpoop_model_loader_man_h
#define dragonpoop_model_loader_man_h

#include "../../../core/shared_obj/shared_obj.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class model_loader_man_ref;
    class model_loader_man_readlock;
    class model_loader_man_writelock;
    class gfx_ref;
    class dptask;
    class model_loader_man_task;
    class dptaskpool_writelock;
    class gfx;
    class model_loader;
    class model_loader_ref;
    class model_saver;
    class model_saver_ref;
    class model_ref;
    
    class model_loader_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        model_loader_man_task *gtsk;
        std::list<model_loader *> loaders;
        std::list<model_saver *> savers;
        int loader_cnt, saver_cnt;
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //delete all loaders
        void deleteLoaders( void );
        //run all loaders
        void runLoaders( dpthread_lock *thd );
        //delete all savers
        void deleteSavers( void );
        //run all savers
        void runSavers( dpthread_lock *thd );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, model_loader_man_writelock *g );
        //load file into model
        bool load( model_ref *m, const char *path_name, const char *file_name, model_loader_ref **mldr );
        //save model into file
        bool save( model_ref *m, const char *path_name, const char *file_name, model_saver_ref **mldr );
        
    public:
        
        //ctor
        model_loader_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~model_loader_man( void );
        //return core
        core *getCore( void );
        
        friend class model_loader_man_readlock;
        friend class model_loader_man_writelock;
        
    };
    
};

#endif