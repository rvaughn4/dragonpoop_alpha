
#include "model_man_model_gui.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../../model/model_ref.h"
#include "../../../model/model_readlock.h"
#include "../../../model/model_writelock.h"
#include "../../../gfx_writelock.h"

#include <iostream>

namespace dragonpoop
{

    //ctor
    model_man_model_gui::model_man_model_gui( gfx_writelock *g, dpid id, dpid pid, model_ref *m ) : window_gui( g, id, pid, 600, 0, 600, 800, "Model" )
    {
        shared_obj_guard o;
        model_writelock *ml;

        this->setDraggable( 1 );
        this->m = 0;
        ml = (model_writelock *)o.tryWriteLock( m, 1000, "model_man_model_gui::model_man_model_gui" );
        if( ml )
            this->m = (model_ref *)ml->getRef();
        o.unlock();
    }

    //dtor
    model_man_model_gui::~model_man_model_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "model_man_model_gui::~model_man_model_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_man_model_gui::~model_man_model_gui" );
        delete this->m;
    }

    //override to do processing
    void model_man_model_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {

        this->window_gui::doProcessing( thd, g );
    }



};
