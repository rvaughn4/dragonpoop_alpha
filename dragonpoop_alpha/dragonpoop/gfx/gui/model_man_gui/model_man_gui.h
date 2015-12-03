
#ifndef dragonpoop_model_man_gui_h
#define dragonpoop_model_man_gui_h

#include "../menu_gui/menu_gui.h"

namespace dragonpoop
{

    class model_man_gui : public menu_gui
    {

    private:

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
