
#ifndef dragonpoop_model_man_gui_h
#define dragonpoop_model_man_gui_h

#include "../window_gui/window_gui.h"
#include "../menu_gui/menu_gui.h"

namespace dragonpoop
{

    class model_man_ref;
    class model_man_model_gui;

    class model_man_gui : public window_gui
    {

    private:

        menu_gui *models_menu;
        model_man_ref *m;
        model_man_model_gui *current_model;

        //repopulate menu
        void repop( void );

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        model_man_gui( gfx_writelock *g, dpid id, dpid pid );
        //dtor
        virtual ~model_man_gui( void );

    };

};

#endif
