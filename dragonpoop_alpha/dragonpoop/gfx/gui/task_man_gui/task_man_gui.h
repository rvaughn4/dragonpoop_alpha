
#ifndef dragonpoop_task_man_gui_h
#define dragonpoop_task_man_gui_h

#include "../window_gui/window_gui.h"
#include "../menu_gui/menu_gui.h"

namespace dragonpoop
{

    class dptaskpool_logger_ref;
    class dptaskpool_logger;

    class task_man_gui : public window_gui
    {

    private:

        dptaskpool_logger_ref *rlggr;
        dptaskpool_logger *lggr;
        uint64_t lastSync;

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        task_man_gui( gfx_writelock *g, dpid id, dpid pid );
        //dtor
        virtual ~task_man_gui( void );

    };

};

#endif
