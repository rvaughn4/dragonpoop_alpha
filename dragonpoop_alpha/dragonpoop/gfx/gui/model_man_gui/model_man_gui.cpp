
#include "model_man_gui.h"

namespace dragonpoop
{

        //ctor
        model_man_gui::model_man_gui( gfx_writelock *g, dpid id, dpid pid ) : window_gui( g, id, pid, 100, 100, 1000, 1000, "Models" )
        {

        }

        //dtor
        model_man_gui::~model_man_gui( void )
        {

        }

        //override to do processing
        void model_man_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
        {
            this->window_gui::doProcessing( thd, g );
        }

};
