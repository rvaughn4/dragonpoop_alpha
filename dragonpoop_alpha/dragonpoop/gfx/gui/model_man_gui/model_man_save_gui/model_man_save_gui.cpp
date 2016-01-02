
#include "model_man_save_gui.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../../model/model_ref.h"
#include "../../../model/model_readlock.h"
#include "../../../model/model_writelock.h"
#include "../../../gfx_writelock.h"
#include "../../edit_gui/edit_gui.h"
#include "../../edit_gui/edit_gui_readlock.h"
#include "../../button_gui/button_gui.h"

namespace dragonpoop
{

    //ctor
    model_man_save_gui::model_man_save_gui( gfx_writelock *g, dpid id, dpid pid, model_ref *m ) : window_gui( g, id, pid, 600, 0, 1000, 800, "Model" )
    {
        shared_obj_guard o;
        model_writelock *ml;
        std::string s;
        float h = 800;

        this->setDraggable( 1 );
        this->m = 0;
        ml = (model_writelock *)o.tryWriteLock( m, 1000, "model_man_save_gui::model_man_save_gui" );
        if( ml )
            this->m = (model_ref *)ml->getRef();

        if( ml )
            ml->getName( &s );
        this->name_edit = new edit_gui( g, this->genId(), id, 10, 100, 980, 50, "Name", s.c_str(), 250, 1 );
        this->addGui( this->name_edit );

        this->save_button = new button_gui( g, this->genId(), id, 10, h - 60, 200, 50, "Save", 1 );
        this->addGui( this->save_button );

        this->animations_button = new button_gui( g, this->genId(), id, 220, h - 60, 200, 50, "Animations", 1 );
        this->addGui( this->animations_button );

        this->materials_button = new button_gui( g, this->genId(), id, 430, h - 60, 200, 50, "Materials", 1 );
        this->addGui( this->materials_button );

        this->groups_button = new button_gui( g, this->genId(), id, 640, h - 60, 200, 50, "Groups", 1 );
        this->addGui( this->groups_button );
    }

    //dtor
    model_man_save_gui::~model_man_save_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "model_man_save_gui::~model_man_save_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_man_save_gui::~model_man_save_gui" );
        delete this->m;
        delete this->name_edit;


        delete this->save_button;
        delete this->animations_button;
        delete this->materials_button;
        delete this->groups_button;
    }

    //override to do processing
    void model_man_save_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o, o1;
        model_writelock *l;
        edit_gui_readlock *el;
        std::string s;

        this->window_gui::doProcessing( thd, g );


        if( this->save_button && this->save_button->wasClicked() )
        {
            l = (model_writelock *)o.tryWriteLock( this->m, 1000, "model_man_save_gui::doProcessing" );
            if( l )
            {
                el = (edit_gui_readlock *)o1.tryReadLock( this->name_edit, 1000, "model_man_save_gui::doProcessing" );
                if( el )
                {
                    el->getValue( &s );
                    l->setName( &s );
                }

                l->sync();
                this->close();
            }
        }
    }



};
