
#include "model_man_load_gui.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../../model/model_ref.h"
#include "../../../model/model_readlock.h"
#include "../../../model/model_writelock.h"
#include "../../../model/model_man_ref.h"
#include "../../../model/model_man_writelock.h"
#include "../../../model/model_man_readlock.h"
#include "../../../model/model_loader/model_loader_ref.h"
#include "../../../gfx_writelock.h"
#include "../../edit_gui/edit_gui.h"
#include "../../edit_gui/edit_gui_readlock.h"
#include "../../button_gui/button_gui.h"

namespace dragonpoop
{

    //ctor
    model_man_load_gui::model_man_load_gui( gfx_writelock *g, dpid id, model_man_ref *mm ) : window_gui( g, id, 600, 0, 1000, 170, "Load Model" )
    {
        shared_obj_guard o;
        model_man_writelock *mml;
        std::string s;

        this->loader_ref = 0;
        this->mm = 0;
        mml = (model_man_writelock *)o.tryWriteLock( mm, 1000, "model_man_model_gui::model_man_model_gui" );
        if( mml )
            this->mm = (model_man_ref *)mml->getRef();

        this->setDraggable( 1 );

        this->name_edit = new edit_gui( g, this->genId(), 10, 55, 980, 50, "Name", "", 150, 1 );
        this->addGui( this->name_edit );

        this->file_edit = new edit_gui( g, this->genId(), 10, 110, 980, 50, "Path", "", 150, 1 );
        this->addGui( this->file_edit );

        this->load_button = new button_gui( g, this->genId(), 10, 165, 490, 50, "Load", 1 );
        this->addGui( this->load_button );

        this->cancel_button = new button_gui( g, this->genId(), 500, 165, 490, 50, "Cancel", 1 );
        this->addGui( this->cancel_button );
    }

    //dtor
    model_man_load_gui::~model_man_load_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "model_man_load_gui::~model_man_load_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "model_man_load_gui::~model_man_load_gui" );
        delete this->mm;
        delete this->name_edit;
        delete this->file_edit;
        delete this->load_button;
        delete this->cancel_button;

        if( this->loader_ref )
            delete this->loader_ref;
    }

    //override to do processing
    void model_man_load_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {

        this->window_gui::doProcessing( thd, g );

        if( this->load_button && this->load_button->wasClicked() )
            this->load();
        if( this->cancel_button && this->cancel_button->wasClicked() )
            this->close();
        if( this->loader_ref && !this->loader_ref->isLinked() )
            this->close();
    }

    //save
    void model_man_load_gui::load( void )
    {
        shared_obj_guard o;
        model_man_writelock *mml;
        edit_gui_readlock *el;
        std::string sname, sfile;

        delete this->load_button;
        this->load_button = 0;
        delete this->cancel_button;
        this->cancel_button = 0;

        el = (edit_gui_readlock *)o.tryReadLock( this->name_edit, 1000, "model_man_load_gui::load" );
        if( !el )
            return;
        el->getValue( &sname );

        el = (edit_gui_readlock *)o.tryReadLock( this->file_edit, 1000, "model_man_load_gui::load" );
        if( !el )
            return;
        el->getValue( &sfile );

        mml = (model_man_writelock *)o.tryWriteLock( this->mm, 1000, "model_man_load_gui::load" );
        if( !mml )
            return;

        if( this->loader_ref )
            delete this->loader_ref;
        this->loader_ref = 0;

        mml->loadModel( sname.c_str(), "", sfile.c_str(), 0, &this->loader_ref );
    }

};
