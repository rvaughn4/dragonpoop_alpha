
#include "model_man_model_gui.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../../model/model_ref.h"
#include "../../../model/model_readlock.h"
#include "../../../model/model_writelock.h"
#include "../../../model/model_man_ref.h"
#include "../../../model/model_man_writelock.h"
#include "../../../gfx_writelock.h"
#include "../../edit_gui/edit_gui.h"
#include "../../edit_gui/edit_gui_readlock.h"
#include "../../button_gui/button_gui.h"
#include "../model_man_save_gui/model_man_save_gui.h"
#include "../../../gfx_ref.h"
#include "../../../../core/core.h"

namespace dragonpoop
{

    //ctor
    model_man_model_gui::model_man_model_gui( gfx_writelock *g, dpid id, model_ref *m, model_man_ref *mm ) : window_gui( g, id, 600, 0, 1000, 600, "Model" )
    {
        shared_obj_guard o;
        model_writelock *ml;
        model_man_writelock *mml;
        std::string s;
        float h = 600;

        this->setDraggable( 1 );

        this->mm = 0;
        mml = (model_man_writelock *)o.tryWriteLock( mm, 1000, "model_man_model_gui::model_man_model_gui" );
        if( mml )
            this->mm = (model_man_ref *)mml->getRef();

        this->m = 0;
        ml = (model_writelock *)o.tryWriteLock( m, 1000, "model_man_model_gui::model_man_model_gui" );
        if( ml )
            this->m = (model_ref *)ml->getRef();

        if( ml )
            ml->getName( &s );
        this->name_edit = new edit_gui( g, this->genId(), 10, 100, 980, 50, "Name", s.c_str(), 250, 1 );
        this->addGui( this->name_edit );

        this->save_button = new button_gui( g, this->genId(), 10, h - 60, 200, 50, "Save", 1 );
        this->addGui( this->save_button );

        this->animations_button = new button_gui( g, this->genId(), 220, h - 60, 200, 50, "Animations", 1 );
        this->addGui( this->animations_button );

        this->materials_button = new button_gui( g, this->genId(), 430, h - 60, 200, 50, "Materials", 1 );
        this->addGui( this->materials_button );

        this->groups_button = new button_gui( g, this->genId(), 640, h - 60, 200, 50, "Groups", 1 );
        this->addGui( this->groups_button );

        this->export_button = new button_gui( g, this->genId(), 850, h - 60, 200, 50, "Export", 1 );
        this->addGui( this->export_button );

        this->save_gui = 0;
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
        delete this->mm;
        delete this->name_edit;

        delete this->save_button;
        delete this->animations_button;
        delete this->materials_button;
        delete this->groups_button;
        delete this->export_button;

        if( this->save_gui )
            delete this->save_gui;
    }

    //override to do processing
    void model_man_model_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o, o1;
        model_writelock *l;
        edit_gui_readlock *el;
        std::string s;

        this->window_gui::doProcessing( thd, g );


        if( this->save_button && this->save_button->wasClicked() )
        {
            l = (model_writelock *)o.tryWriteLock( this->m, 1000, "model_man_model_gui::doProcessing" );
            if( l )
            {
                el = (edit_gui_readlock *)o1.tryReadLock( this->name_edit, 1000, "model_man_model_gui::doProcessing" );
                if( el )
                {
                    el->getValue( &s );
                    l->setName( &s );
                }

                l->sync();
                this->close();
            }
        }

        if( this->save_gui && this->save_gui->wasClosed() )
            this->closeSave();

        if( this->export_button && this->export_button->wasClicked() )
            this->openSave();

    }

    //close the save window
    void model_man_model_gui::closeSave( void )
    {
        if( this->save_gui )
            delete this->save_gui;
        this->save_gui = 0;
    }

    //open the save window
    void model_man_model_gui::openSave( void )
    {
        gfx_writelock *gl;
        gfx_ref *gr;
        shared_obj_guard o;

        this->closeSave();

        gr = this->getCore()->getGfx();
        if( !gr )
            return;

        gl = (gfx_writelock *)o.tryWriteLock( gr, 1000, "model_man_model_gui::doProcessing" );
        if( gl )
        {
            this->save_gui = new model_man_save_gui( gl, this->genId(), this->m, this->mm );
            this->addGui( this->save_gui );
        }
        o.unlock();

        delete gr;
    }


};
