
#ifndef dragonpoop_edit_gui_h
#define dragonpoop_edit_gui_h

#include "../gui.h"
#include <string>

namespace dragonpoop
{

    class edit_text_gui;

    class edit_gui : public gui
    {

    private:

        bool bWasClicked;
        edit_text_gui *txt_gui;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //returns true if clicked
        bool wasClicked( void );
        //set value
        void setValue( std::string *s );
        //get value
        void getValue( std::string *s );

    public:

        //ctor
        edit_gui( gfx_writelock *g, dpid id, float x, float y, float w, float h, const char *txtLabel, const char *txtValue, float labelWidth, bool bHasBg );
        //dtor
        virtual ~edit_gui( void );

        friend class edit_gui_writelock;
        friend class edit_gui_readlock;
    };

};

#endif
