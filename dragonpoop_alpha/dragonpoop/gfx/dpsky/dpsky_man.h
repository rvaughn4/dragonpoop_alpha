
#ifndef dragonpoop_dpsky_man_h
#define dragonpoop_dpsky_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "dpsky.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpsky_man_ref;
    class dpsky_man_readlock;
    class dpsky_man_writelock;
    class gfx_ref;
    class dptask;
    class dpsky_man_task;
    class dptaskpool_writelock;
    class gfx;
    
    class dpsky_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        dpsky_man_task *gtsk;
        dpsky_stuff stuff;
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //build skydome
        void buildSky( void );
        //load sky textures
        void loadSky( void );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, dpsky_man_writelock *g );
        //return sky stuff
        dpsky_stuff *getSky( void );
        
    public:
        
        //ctor
        dpsky_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~dpsky_man( void );
        //return core
        core *getCore( void );
        
        friend class dpsky_man_readlock;
        friend class dpsky_man_writelock;
        
    };
    
};

#endif