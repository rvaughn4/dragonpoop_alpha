
#ifndef dragonpoop_model_man_model_gui_h
#define dragonpoop_model_man_model_gui_h

#include "../../window_gui/window_gui.h"

namespace dragonpoop
{

    class model_ref;
    class edit_gui;
    class button_gui;
    class model_man_save_gui;
    class model_man_ref;

    class model_man_model_gui : public window_gui
    {

    private:

        model_ref *m;
        model_man_ref *mm;
        edit_gui *name_edit;

        button_gui *save_button, *animations_button, *materials_button, *groups_button, *export_button;
        model_man_save_gui *save_gui;

        //close the save window
        void closeSave( void );
        //open the save window
        void openSave( void );

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        model_man_model_gui( gfx_writelock *g, dpid id, dpid pid, model_ref *m, model_man_ref *mm );
        //dtor
        virtual ~model_man_model_gui( void );

    };

};

#endif
