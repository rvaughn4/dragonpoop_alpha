
#ifndef dragonpoop_gui_man_h
#define dragonpoop_gui_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class gui_man_ref;
    class gui_man_readlock;
    class gui_man_writelock;
    class gfx_ref;
    class dptask;
    class gui_man_task;
    class dptaskpool_writelock;
    class gfx;
    class gui;
    class gui_ref;
    class dptaskpool_ref;
    class gui_factory;
    class gui_factory_ref;
    class root_gui;
    
    class gui_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        gui_man_task *gtsk;
        std::list<gui_ref *> guis;
        int gui_cnt;
        dptaskpool_ref *tpr;
        gui_factory_ref *root_factory;
        gui *root_g;
        
        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //delete all guis
        void deleteGuis( void );
        //run all guis
        void runGuis( dpthread_lock *thd );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, gui_man_writelock *g );
        //add gui
        void addGui( gui *g );
        //add gui
        void addGui( gui_ref *g );
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //return gui count
        unsigned int getGuiCount( void );
        //set root gui factory
        void setRootGui( gui_factory *g );

    public:
        
        //ctor
        gui_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~gui_man( void );
        //return core
        core *getCore( void );
        
        friend class gui_man_readlock;
        friend class gui_man_writelock;
        
    };
    
};

#endif