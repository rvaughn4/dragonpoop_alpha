
#ifndef dragonpoop_perf_stats_gui_h
#define dragonpoop_perf_stats_gui_h

#include "../window_gui/window_gui.h"
#include "../../dpposition/dpposition.h"

namespace dragonpoop
{

    class gfx_ref;
    class button_gui;
    class dpposition_share_ref;

    class perf_stats_gui : public window_gui
    {

    private:

        uint64_t t_last_update;
        gfx_ref *g;
        dpposition_share_ref *rcampos;
        dpposition campos;
        uint64_t campos_t;

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        perf_stats_gui( gfx_writelock *g, dpid id );
        //dtor
        virtual ~perf_stats_gui( void );

    };

};

#endif
