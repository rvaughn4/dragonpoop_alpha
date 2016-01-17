
#ifndef dragonpoop_renderer_gui_man_readlock_h
#define dragonpoop_renderer_gui_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{

    class renderer_gui_man;
    class core;
    class dpthread_lock;
    class renderer_gui_ref;
    class renderer_gui;
    class renderer_writelock;
    class dpmatrix;

    class renderer_gui_man_readlock : public shared_obj_readlock
    {

    private:

        renderer_gui_man *t;

    protected:

        //ctor
        renderer_gui_man_readlock( renderer_gui_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_gui_man_readlock( void );

    public:

        //return core
        core *getCore( void );
        //get hovering gui id
        dpid getHoverId( void );

        friend class renderer_gui_man;
    };

};

#endif
