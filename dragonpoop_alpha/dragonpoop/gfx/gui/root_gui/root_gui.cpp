
#include "root_gui.h"
#include "../menu_gui/menu_gui.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../menu_gui/menu_gui.h"
#include "../menu_gui/menu_gui_readlock.h"
#include "../menu_gui/menu_gui_writelock.h"
#include "../../gfx_ref.h"
#include "../../gfx_writelock.h"
#include "../../../core/dpthread/dpthread_lock.h"
#include "../../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    root_gui::root_gui( gfx_writelock *g, dpid id ) : gui( g, id )
    {
        this->g = (gfx_ref *)g->getRef();
        
        this->enableBg( 0 );
        this->enableFg( 0 );
        this->setFade( 0 );
        this->setPosition( 0, 0 );
        this->setWidthHeight( 1920, 1080 );
        
        this->esc_menu = 0;
        this->esc_menu_is_show = 0;
        this->esc_menu_do_show = 0;
    }
    
    //dtor
    root_gui::~root_gui( void )
    {
        if( this->esc_menu )
            delete this->esc_menu;
        delete this->g;
    }
    
    //override to handle keyboard button
    void root_gui::handleKbEvent( std::string *skey, bool isDown )
    {
        this->gui::handleKbEvent( skey, isDown );
        
        if( skey->compare( "Escape" ) == 0 && isDown )
            this->showEscapeMenu();
        
    }
    
    //override to do processing
    void root_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o;
        menu_gui_readlock *mr;
        menu_gui_writelock *mw;
        gfx_writelock *gl;
        uint64_t t;
        
        this->gui::doProcessing( thd, g );
        
        if( this->esc_menu_do_show != this->esc_menu_is_show )
        {
            if( this->esc_menu_do_show )
            {
                if( this->esc_menu )
                    delete this->esc_menu;
                this->esc_menu = 0;
                this->esc_menu_is_show = 0;
                
                gl = (gfx_writelock *)o.tryWriteLock( this->g, 1000, "root_gui::doProcessing" );
                if( gl )
                {
                    this->esc_menu = new menu_gui( gl, this->genId(), this->getId(), 0, 0, 400, 100, 70, "Escape Menu" );
                    this->addGui( this->esc_menu );
                    
                    mw = (menu_gui_writelock *)o.tryWriteLock( this->esc_menu, 1000, "root_gui::doProcessing" );
                    if( mw )
                        this->populateEscapeMenu( mw );
                    this->esc_menu_is_show = 1;
                }
            }
            else
            {
                if( this->esc_menu )
                    delete this->esc_menu;
                this->esc_menu = 0;
                this->esc_menu_is_show = 0;
            }
        }
        
        if( this->esc_menu_is_show && this->esc_menu )
        {
            t = thd->getTicks();
            if( t - this->last_esc_menu_process > 200 )
            {
                this->last_esc_menu_process = t;
                mr = (menu_gui_readlock *)o.tryReadLock( this->esc_menu, "root_gui::doProcessing" );
                if( mr )
                    this->processEscapeMenu( mr );
            }
        }
    }
    
    //populate escape menu
    void root_gui::populateEscapeMenu( menu_gui_writelock *m )
    {
        m->addButton( "Quit" );
        m->addButton( "Close" );
    }
    
    //process escape menu clicks
    void root_gui::processEscapeMenu( menu_gui_readlock *m )
    {
        if( m->wasClicked( "Quit" ) )
        {
            this->hideEscapeMenu();
            this->getCore()->kill();
            return;
        }
        if( m->wasClicked( "Close" ) )
        {
            this->hideEscapeMenu();
            return;
        }
    }
    
    //hide escape menu
    void root_gui::hideEscapeMenu( void )
    {
        this->esc_menu_do_show = 0;
    }
    
    //show escape menu
    void root_gui::showEscapeMenu( void )
    {
        this->esc_menu_do_show = 1;
    }
    
};
