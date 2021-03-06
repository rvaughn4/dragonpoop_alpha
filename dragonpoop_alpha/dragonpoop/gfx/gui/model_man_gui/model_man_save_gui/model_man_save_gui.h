
#ifndef dragonpoop_model_man_save_gui_h
#define dragonpoop_model_man_save_gui_h

#include "../../window_gui/window_gui.h"

namespace dragonpoop
{

    class model_ref;
    class edit_gui;
    class button_gui;
    class model_man_ref;
    class model_saver_ref;

    class model_man_save_gui : public window_gui
    {

    private:

        model_ref *m;
        model_man_ref *mm;
        edit_gui *file_edit;
        model_saver_ref *saver_ref;

        button_gui *save_button, *cancel_button;

        //save
        void save( void );

    protected:

        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:

        //ctor
        model_man_save_gui( gfx_writelock *g, dpid id, model_ref *m, model_man_ref *mm );
        //dtor
        virtual ~model_man_save_gui( void );

    };

};

#endif
