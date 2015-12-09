
#include "model_man_gui.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../model/model_man_ref.h"
#include "../../model/model_man_readlock.h"
#include "../../model/model_readlock.h"
#include "../../model/model.h"
#include "../menu_gui/menu_gui_writelock.h"
#include "../../gfx_writelock.h"

namespace dragonpoop
{

    //ctor
    model_man_gui::model_man_gui( gfx_writelock *g, dpid id, dpid pid ) : window_gui( g, id, pid, 100, 100, 1000, 1000, "Models" )
    {
        g->getModels( &this->m );

        this->models_menu = new menu_gui( g, this->genId(), id, 10, 100, 800, 700, 50, 0 );
        this->addGui( this->models_menu );

        this->repop();
    }

    //dtor
    model_man_gui::~model_man_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "model_man_gui::~model_man_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_man_gui::~model_man_gui" );
        delete this->models_menu;
        delete this->m;
    }

    //override to do processing
    void model_man_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        this->window_gui::doProcessing( thd, g );
    }

    //repopulate menu
    void model_man_gui::repop( void )
    {
        std::list<model *> l;
        std::list<model *>::iterator i;
        model *p;
        model_readlock *pl;
        model_man_readlock *ml;
        menu_gui_writelock *gl;
        shared_obj_guard o, o1, o2;
        std::string s;

        ml = (model_man_readlock *)o.tryReadLock( this->m, 100, "model_man_gui::repop" );
        if( !ml )
            return;

        gl = (menu_gui_writelock *)o1.tryWriteLock( this->models_menu, 100, "model_man_gui::repop" );
        if( !gl )
            return;
        gl->removeButtons();

        ml->getModels( &l );

        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pl = (model_readlock *)o2.tryReadLock( p, 100, "model_man_gui::repop" );
            if( !pl )
                continue;
            pl->getName( &s );
            o2.unlock();
            gl->addButton( s.c_str() );
        }
    }


};
