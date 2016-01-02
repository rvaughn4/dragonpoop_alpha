
#ifndef dragonpoop_model_man_save_gui_h
#define dragonpoop_model_man_save_gui_h

#include "../../window_gui/window_gui.h"

namespace dragonpoop
{

    class model_ref;
    class edit_gui;
    class button_gui;

    class model_man_save_gui : public window_gui
    {

    private:

        model_ref *m;
        edit_gui *file_edit;

        button_gui *save_button, *cencel_button;

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        model_man_save_gui( gfx_writelock *g, dpid id, dpid pid, model_ref *m );
        //dtor
        virtual ~model_man_save_gui( void );

    };

};

#endif
