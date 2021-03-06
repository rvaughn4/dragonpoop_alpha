
#ifndef dragonpoop_root_gui_h
#define dragonpoop_root_gui_h

#include "../gui.h"
#include "../../dpposition/dpposition.h"

namespace dragonpoop
{

    class menu_gui;
    class window_gui;
    class menu_gui_writelock;
    class menu_gui_readlock;
    class gfx_ref;
    class perf_stats_gui;
    class dpactor;
    class dpposition_share_ref;

    class root_gui : public gui
    {

    private:

        gfx_ref *g;
        menu_gui *esc_menu;
        perf_stats_gui *perf_stats;
        window_gui *modelgui, *taskgui;
        bool esc_menu_is_show, esc_menu_do_show, perf_open, model_open, task_open;
        uint64_t last_esc_menu_process, t;
        dpactor *a;
        dpposition campos;
        dpposition_share_ref *rcampos;

    protected:

        //override to handle keyboard button
        virtual bool handleKbEvent( std::string *skey, bool isDown );
        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );
        //populate escape menu
        virtual void populateEscapeMenu( menu_gui_writelock *m );
        //process escape menu clicks
        virtual void processEscapeMenu( menu_gui_readlock *m );
        //hide escape menu
        void hideEscapeMenu( void );
        //show escape menu
        void showEscapeMenu( void );
        //show perf stats
        void showPerfStats( void );

    public:

        //ctor
        root_gui( gfx_writelock *g, dpid id );
        //dtor
        virtual ~root_gui( void );

    };

};

#endif
