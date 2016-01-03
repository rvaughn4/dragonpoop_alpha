
#include "model_man_save_gui.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../../model/model_ref.h"
#include "../../../model/model_readlock.h"
#include "../../../model/model_writelock.h"
#include "../../../model/model_man_ref.h"
#include "../../../model/model_man_writelock.h"
#include "../../../model/model_man_readlock.h"
#include "../../../model/model_saver/model_saver_ref.h"
#include "../../../gfx_writelock.h"
#include "../../edit_gui/edit_gui.h"
#include "../../edit_gui/edit_gui_readlock.h"
#include "../../button_gui/button_gui.h"

namespace dragonpoop
{

    //ctor
    model_man_save_gui::model_man_save_gui( gfx_writelock *g, dpid id, dpid pid, model_ref *m, model_man_ref *mm ) : window_gui( g, id, pid, 0, 600, 1000, 120, "Save Model As" )
    {
        shared_obj_guard o;
        model_writelock *ml;
        model_man_writelock *mml;
        std::string s;

        this->saver_ref = 0;
        this->mm = 0;
        mml = (model_man_writelock *)o.tryWriteLock( mm, 1000, "model_man_model_gui::model_man_model_gui" );
        if( mml )
            this->mm = (model_man_ref *)mml->getRef();

        this->setDraggable( 1 );
        this->m = 0;
        ml = (model_writelock *)o.tryWriteLock( m, 1000, "model_man_save_gui::model_man_save_gui" );
        if( ml )
            this->m = (model_ref *)ml->getRef();

        if( ml )
            ml->getName( &s );
        s.append( ".dpmodel" );
        this->file_edit = new edit_gui( g, this->genId(), id, 10, 55, 980, 50, "Path", s.c_str(), 150, 1 );
        this->addGui( this->file_edit );

        this->save_button = new button_gui( g, this->genId(), id, 10, 110, 490, 50, "Save", 1 );
        this->addGui( this->save_button );

        this->cancel_button = new button_gui( g, this->genId(), id, 500, 110, 490, 50, "Cancel", 1 );
        this->addGui( this->cancel_button );
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
        delete this->mm;
        delete this->file_edit;
        delete this->save_button;
        delete this->cancel_button;

        if( this->saver_ref )
            delete this->saver_ref;
    }

    //override to do processing
    void model_man_save_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {

        this->window_gui::doProcessing( thd, g );

        if( this->save_button && this->save_button->wasClicked() )
            this->save();
        if( this->cancel_button && this->cancel_button->wasClicked() )
            this->close();
        if( this->saver_ref && !this->saver_ref->isLinked() )
            this->close();
    }

    //save
    void model_man_save_gui::save( void )
    {
        shared_obj_guard o;
        model_man_readlock *mml;
        model_readlock *ml;
        edit_gui_readlock *el;
        std::string sname, sfile;

        delete this->save_button;
        this->save_button = 0;
        delete this->cancel_button;
        this->cancel_button = 0;

        el = (edit_gui_readlock *)o.tryReadLock( this->file_edit, 1000, "model_man_save_gui::save" );
        if( !el )
            return;
        el->getValue( &sfile );

        ml = (model_readlock *)o.tryReadLock( this->m, 1000, "model_man_save_gui::save" );
        if( !ml )
            return;
        ml->getName( &sname );

        mml = (model_man_readlock *)o.tryReadLock( this->mm, 1000, "model_man_save_gui::save" );
        if( !mml )
            return;

        if( this->saver_ref )
            delete this->saver_ref;
        this->saver_ref = 0;

        mml->saveModel( sname.c_str(), "", sfile.c_str(), &this->saver_ref );
    }

};
