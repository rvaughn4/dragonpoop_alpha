
#include "window_gui.h"
#include "../../../core/dpthread/dpthread_lock.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../gfx_readlock.h"
#include "../../gfx_writelock.h"
#include "../../gfx_ref.h"
#include "../button_gui/button_gui.h"
#include "../../../renderer/renderer_ref.h"
#include "../../../renderer/renderer_readlock.h"
#include "../../dpposition/dpposition.h"
#include "../../dpposition/dpposition_share_ref.h"
#include "../../dpposition/dpposition_share_readlock.h"

#include "../gui_man_readlock.h"
#include "../../model/model_man_readlock.h"
#include "../gui_man_ref.h"
#include "../../model/model_man_ref.h"

#include <sstream>

namespace dragonpoop
{

    //ctor
    window_gui::window_gui( gfx_writelock *g, dpid id, dpid pid, int x, int y, int w, int h, const char *ctitle ) : gui( g, id )
    {
        this->setParentId( pid );
        this->setPosition( x, y );
        this->setWidthHeight( w, h + 50 );
        this->setEditMode( 0 );
        this->enableBg( 1 );
        this->enableFg( 1 );
        this->setFontSize( 20 );
        this->setMargin( 20 );
        this->setFade( 1 );
        this->setHoverMode( 1 );
        this->bDoClose = 0;

        this->bclose = new button_gui( g, this->genId(), id, w - 52, 2, 50, 50, "X", 1 );
        this->bhide = new button_gui( g, this->genId(), id, w - 104, 2, 50, 50, "-", 1 );
        this->btitle = new button_gui( g, this->genId(), id, 0, 0, w - 104, 50, ctitle, 0 );
        this->addGui( this->bclose );
        this->addGui( this->bhide );
        this->addGui( this->btitle );
    }

    //dtor
    window_gui::~window_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "window_gui::~window_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "window_gui::~window_gui" );
        delete this->bclose;
        delete this->bhide;
        delete this->btitle;
        o.unlock();
    }

    //override to paint background texture
    void window_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }

    //override to do processing
    void window_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        this->gui::doProcessing( thd, g );
        if( this->bclose->wasClicked() )
            this->bDoClose = 1;
    }

    //returns true if closed
    bool window_gui::wasClosed( void )
    {
        return this->bDoClose;
    }

};
