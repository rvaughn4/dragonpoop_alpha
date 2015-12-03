
#include "model_man_gui.h"

namespace dragonpoop
{

        //ctor
        model_man_gui::model_man_gui( gfx_writelock *g, dpid id, dpid pid ) : menu_gui( g, id, pid, 20, 20, 500, 1000, 50, "Models" )
        {

        }

        //dtor
        model_man_gui::~model_man_gui( void )
        {

        }

        //override to do processing
        void model_man_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
        {
            this->menu_gui::doProcessing( thd, g );
        }

};
