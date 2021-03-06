
#ifndef dragonpoop_renderer_gui_man_writelock_h
#define dragonpoop_renderer_gui_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>
#include <string>

namespace dragonpoop
{

    class renderer_gui_man;
    class core;
    class dpthread_lock;
    class renderer_gui_ref;
    class renderer_gui;
    class renderer_writelock;
    class dpmatrix;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class gui_writelock;

    class renderer_gui_man_writelock : public shared_obj_writelock
    {

    private:

        renderer_gui_man *t;

    protected:

        //ctor
        renderer_gui_man_writelock( renderer_gui_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_gui_man_writelock( void );

    public:

        //return core
        core *getCore( void );
        //delete guis
        void deleteGuis( void );
        //process mouse input
        bool processGuiMouseInput( float w, float h, float x, float y, bool lb, bool rb );
        //get hovering gui id
        dpid getHoverId( void );
        //process gui keyboard input
        void processGuiKbInput( std::string *skey_name, bool isDown );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //run
        void run( dpthread_lock *thd );
        //generate gui
        renderer_gui *genGui( gui_writelock *g );

        friend class renderer_gui_man;
    };

};

#endif
