
#ifndef dragonpoop_renderer_gui_man_h
#define dragonpoop_renderer_gui_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class renderer_gui_man_ref;
    class renderer_gui_man_readlock;
    class renderer_gui_man_writelock;
    class gfx_ref;
    class dptask;
    class renderer_gui_man_task;
    class dptaskpool_writelock;
    class gfx;
    class renderer_gui;
    class renderer_gui_ref;
    class dptaskpool_ref;
    class renderer;
    class renderer_ref;
    class gui_man_ref;
    class renderer_writelock;
    class dpmatrix;
    class gui_writelock;
    
    class renderer_gui_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        renderer_gui_man_task *gtsk;
        std::list<renderer_gui *> guis;
        dptaskpool_ref *tpr;
        renderer_ref *r;
        gui_man_ref *g_guis;
        dpid hover_gui, focus_gui;
        uint64_t t_last_gui_synced;

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
        //run from manager thread
        void runFromTask( dpthread_lock *thd, renderer_gui_man_writelock *g );
        //run from renderer thread
        void runFromRenderer( dpthread_lock *thd, renderer_gui_man_writelock *g, renderer_writelock *rl );
        //delete guis
        void deleteGuis( void );
        //render guis
        void renderGuis( dpthread_lock *thd, renderer_writelock *rl, renderer_gui_man_readlock *ml, dpmatrix *m_world );
        //return guis
        void getChildrenGuis( std::list<renderer_gui *> *l, dpid pid );
        //process mouse input
        bool processGuiMouseInput( renderer_gui_man_writelock *r, float x, float y, bool lb, bool rb );
        //get hovering gui id
        dpid getHoverId( void );
        //process gui keyboard input
        void processGuiKbInput( std::string *skey_name, bool isDown );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //generate renderer gui
        virtual renderer_gui *genGui( gui_writelock *ml );

    public:
        
        //ctor
        renderer_gui_man( core *c, renderer *r, dptaskpool_writelock *tp );
        //dtor
        virtual ~renderer_gui_man( void );
        //return core
        core *getCore( void );
        
        friend class renderer_gui_man_readlock;
        friend class renderer_gui_man_writelock;
        
    };
    
};

#endif