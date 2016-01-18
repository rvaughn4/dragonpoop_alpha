
#ifndef dragonpoop_gui_readlock_h
#define dragonpoop_gui_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class gui;
    class core;
    struct gui_dims;
    class dpbitmap;
    class gui_ref;

    class gui_readlock : public shared_obj_readlock
    {

    private:

        gui *t;

    protected:

        //ctor
        gui_readlock( gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~gui_readlock( void );

    public:

        //return core
        core *getCore( void );
        //returns id
        dpid getId( void );
        //compares id
        bool compareId( dpid id );
        //get dimensions
        void getDimensions( gui_dims *p );
        //returns true if has background texture
        bool hasBg( void );
        //returns true if has forground texture
        bool hasFg( void );
        //returns pointer to bg texture
        dpbitmap *getBg( void );
        //returns pointer to fg texture
        dpbitmap *getFg( void );
        //add gui
        void addGui( gui *g );
        //returns true if editable
        bool isEditable( void );
        //returns true if hoverable
        bool isHoverable( void );
        //returns true if gui should swoosh in and out
        bool isFade( void );
        //get text
        void getText( std::string *s );
        //get children
        void getChildren( std::list<gui_ref *> *l );
        //returns true if follows cursor
        bool followsCursor( float *px, float *py );

        friend class gui;
    };

};

#endif
