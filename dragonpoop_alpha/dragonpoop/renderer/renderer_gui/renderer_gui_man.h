
#ifndef dragonpoop_renderer_gui_man_h
#define dragonpoop_renderer_gui_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../../gfx/dpmatrix/dpmatrix.h"

#include <atomic>

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
    class render_api_context_ref;
    class render_api_commandlist_ref;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class render_api_shader_ref;
    class renderer_commandlist_passer;
    class renderer_commandlist_passer_ref;
    class input_passer_writelock;
    class input_passer;
    class input_passer_ref;

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
        render_api_context_ref *ctx;
        render_api_commandlist_ref *clist;
        dpmatrix m, m_undo;
        float log_screen_width, log_screen_height;
        std::atomic<bool> listReady;
        renderer_commandlist_passer_ref *clpasser;
        input_passer *ip;
        input_passer_ref *ipr;
        bool lb, rb;

        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //sync guis
        void sync( dpthread_lock *thd, renderer_gui_man_writelock *g );
        //delete old guis
        void deleteOldGuis( dpthread_lock *thd, renderer_gui_man_writelock *g );
        //run guis
        void runGuis( dpthread_lock *thd, renderer_gui_man_writelock *g );
        //render into command list
        void render( dpthread_lock *thd, renderer_gui_man_writelock *g );
        //compute matrix
        void computeMatrix( void );
        //render guis
        void renderGuis( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //delete guis
        void deleteGuis( void );
        //process mouse input
        bool processGuiMouseInput( renderer_gui_man_writelock *r, float w, float h, float x, float y, bool lb, bool rb );
        //get hovering gui id
        dpid getHoverId( void );
        //process gui keyboard input
        void processGuiKbInput( renderer_gui_man_writelock *r, std::string *skey_name, bool isDown );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //generate renderer gui
        virtual renderer_gui *genGui( gui_writelock *ml );
        //run from manager thread
        void run( dpthread_lock *thd, renderer_gui_man_writelock *g );

    public:

        //ctor
        renderer_gui_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height, input_passer_writelock *ipl );
        //dtor
        virtual ~renderer_gui_man( void );
        //return core
        core *getCore( void );

        friend class renderer_gui_man_readlock;
        friend class renderer_gui_man_writelock;

    };

};

#endif
