
#ifndef dragonpoop_model_man_load_gui_h
#define dragonpoop_model_man_load_gui_h

#include "../../window_gui/window_gui.h"

namespace dragonpoop
{

    class edit_gui;
    class button_gui;
    class model_man_ref;
    class model_loader_ref;

    class model_man_load_gui : public window_gui
    {

    private:

        model_man_ref *mm;
        model_loader_ref *loader_ref;

        edit_gui *name_edit, *file_edit;
        button_gui *load_button, *cancel_button;

        //save
        void load( void );

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        model_man_load_gui( gfx_writelock *g, dpid id, model_man_ref *mm );
        //dtor
        virtual ~model_man_load_gui( void );

    };

};

#endif
