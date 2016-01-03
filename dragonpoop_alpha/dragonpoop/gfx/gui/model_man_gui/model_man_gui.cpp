
#include "model_man_gui.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../model/model_man_ref.h"
#include "../../model/model_man_readlock.h"
#include "../../model/model_readlock.h"
#include "../../model/model_ref.h"
#include "../../model/model.h"
#include "../menu_gui/menu_gui_writelock.h"
#include "../menu_gui/menu_gui_readlock.h"
#include "../../gfx_writelock.h"
#include "../../gfx_ref.h"
#include "../../../core/core.h"
#include "model_man_model_gui/model_man_model_gui.h"
#include "model_man_load_gui/model_man_load_gui.h"

namespace dragonpoop
{

    //ctor
    model_man_gui::model_man_gui( gfx_writelock *g, dpid id, dpid pid ) : window_gui( g, id, pid, 100, 100, 600, 800, "Models" )
    {
        g->getModels( &this->m );

        this->loader_gui = 0;
        this->current_model = 0;
        this->models_menu = new menu_gui( g, this->genId(), id, 10, 80, 580, 600, 50, 0 );
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
        delete this->current_model;
        delete this->loader_gui;
    }

    //override to do processing
    void model_man_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o, og;
        menu_gui_readlock *gl;
        std::string s;
        model_ref *m;
        model_man_readlock *ml;
        gfx_ref *gf;
        gfx_writelock *gfl;

        this->window_gui::doProcessing( thd, g );

        if( this->current_model && this->current_model->wasClosed() )
        {
            delete this->current_model;
            this->current_model = 0;
            this->repop();
        }

        if( this->loader_gui && this->loader_gui->wasClosed() )
            this->closeLoader();

        gl = (menu_gui_readlock *)o.tryReadLock( this->models_menu, 100, "model_man_gui::doProcessing" );
        if( !gl )
            return;

        if( !gl->getClicked( &s ) )
            return;

        if( s.compare( "Load Model" ) == 0 )
        {
            this->openLoader();
            return;
        }

        ml = (model_man_readlock *)o.tryReadLock( this->m, 1000, "model_man_gui::doProcessing" );
        if( !ml )
            return;
        m = ml->findModel( s.c_str() );
        if( !m )
            return;
        gf = this->getCore()->getGfx();
        if( !gf )
        {
            delete m;
            return;
        }
        gfl = (gfx_writelock *)og.tryWriteLock( gf, 1000, "model_man_gui::doProcessing" );
        if( !gfl )
        {
            delete m;
            delete gf;
            return;
        }

        if( this->current_model )
            delete this->current_model;
        this->current_model = new model_man_model_gui( gfl, this->genId(), this->getId(), m, this->m );
        g->addGui( this->current_model );

        og.unlock();
        o.unlock();
        delete m;
        delete gf;
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
        gl->addButton( "Load Model" );

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

    //open loader
    void model_man_gui::openLoader( void )
    {
        shared_obj_guard o;
        gfx_ref *gr;
        gfx_writelock *gl;

        this->closeLoader();

        gr = this->getCore()->getGfx();
        if( !gr )
            return;
        gl = (gfx_writelock *)o.tryWriteLock( gr, 1000, "model_man_gui::openLoader" );

        if( gl )
        {
            this->loader_gui = new model_man_load_gui( gl, this->genId(), this->getId(), this->m );
            this->addGui( this->loader_gui );
        }

        delete gr;
    }

    //close loader
    void model_man_gui::closeLoader( void )
    {
        if( this->loader_gui )
            delete this->loader_gui;
        this->loader_gui = 0;
    }

};
