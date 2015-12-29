
#ifndef dragonpoop_edit_text_gui_h
#define dragonpoop_edit_text_gui_h

#include "../gui.h"
#include <string>

namespace dragonpoop
{

    class edit_text_gui : public gui
    {

    private:

        bool bWasClicked;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //returns true if clicked
        bool wasClicked( void );
        //override to handle keyboard button
        virtual bool handleKbEvent( std::string *skey, bool isDown );

    public:

        //ctor
        edit_text_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txtValue );
        //dtor
        virtual ~edit_text_gui( void );

        friend class edit_text_gui_writelock;
        friend class edit_text_gui_readlock;
    };

};

#endif
