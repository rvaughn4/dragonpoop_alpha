
#include "menu_gui.h"
#include "menu_gui_readlock.h"
#include "menu_gui_writelock.h"
#include "../gui_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../gfx_ref.h"
#include "../../gfx_writelock.h"
#include "../../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    menu_gui::menu_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, float bh, const char *txt ) : gui( g, id )
    {
        this->g = (gfx_ref *)g->getRef();
        
        this->setParentId( pid );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setHoverMode( 1 );
        this->setEditMode( 0 );
        this->enableBg( 1 );
        this->enableFg( 0 );
        this->setFade( 1 );

        this->sz = bh;
        if( txt )
        {
            this->last_y = bh;
            this->top_y = bh;
            this->btitle = new button_gui( g, this->genId(), this->getId(), 0, 0, w, this->sz, txt, 0 );
            this->addGui( this->btitle );
        }
        else
        {
            this->top_y = 0;
            this->last_y = 0;
            this->btitle = 0;
        }
    }
    
    //dtor
    menu_gui::~menu_gui( void )
    {
        shared_obj_writelock *l;
        shared_obj_guard o;
        
        l = (shared_obj_writelock *)o.tryWriteLock( this, 2000, "menu_gui::~menu_gui" );
        
        this->removeButtons();
        if( this->btitle )
            delete this->btitle;
        delete this->g;
        
        o.unlock();
    }
    
    //generate read lock
    shared_obj_readlock *menu_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new menu_gui_readlock( (menu_gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *menu_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new menu_gui_writelock( (menu_gui *)p, l );
    }
    
    //override to paint background texture
    void menu_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }
    
    //add button
    void menu_gui::addButton( const char *btn_name )
    {
        unsigned int i, e;
        menu_gui_btn *p, np;
        gfx_writelock *g;
        shared_obj_guard o;
        gui_dims gp;

        e = (unsigned int)this->btns.size();
        for( i = 0; i < e; i++ )
        {
            p = &this->btns[ i ];
            if( p->s.compare( btn_name ) != 0 )
                continue;
            if( p->b )
                delete p->b;
            p->b = 0;
        }

        g = (gfx_writelock *)o.tryWriteLock( this->g, 2000, "menu_gui::addButton" );
        if( !g )
            return;
        
        this->getDimensions( &gp );
        np.s.assign( btn_name );
        np.b = new button_gui( g, this->genId(), this->getId(), 0, 0, gp.w, this->sz, btn_name, 1 );
        this->addGui( np.b );
        o.unlock();
        this->btns.push_back( np );
        
        this->redoButtons();
    }
    
    //remove button
    void menu_gui::removeButton( const char *btn_name )
    {
        unsigned int i, e;
        menu_gui_btn *p;
        
        e = (unsigned int)this->btns.size();
        for( i = 0; i < e; i++ )
        {
            p = &this->btns[ i ];
            if( p->s.compare( btn_name ) != 0 )
                continue;
            if( p->b )
                delete p->b;
            p->b = 0;
        }
        
        this->redoButtons();
    }
    
    //remove all buttons
    void menu_gui::removeButtons( void )
    {
        unsigned int i, e;
        menu_gui_btn *p;

        e = (unsigned int)this->btns.size();
        for( i = 0; i < e; i++ )
        {
            p = &this->btns[ i ];
            if( p->b )
                delete p->b;
            p->b = 0;
        }
        
        this->redoButtons();
    }

    //recreate all buttons
    void menu_gui::redoButtons( void )
    {
        unsigned int i, e;
        menu_gui_btn *p;
        std::vector<menu_gui_btn> ll;
        gui_writelock *l;
        shared_obj_guard o;
        gui_dims gp;
        
        e = (unsigned int)this->btns.size();
        for( i = 0; i < e; i++ )
        {
            p = &this->btns[ i ];
            if( p->b )
                ll.push_back( *p );
        }
        this->btns.clear();
        
        this->last_y = this->top_y;
        e = (unsigned int)ll.size();
        for( i = 0; i < e; i++ )
        {
            p = &ll[ i ];
            this->btns.push_back( *p );
            l = (gui_writelock *)o.tryWriteLock( p->b, 1000, "menu_gui::redoButtons" );
            if( !l )
                continue;
            l->setPosition( 0, this->last_y );
            l->redraw();
            this->last_y += this->sz;
        }
        
        this->getDimensions( &gp );
        this->setWidthHeight( gp.w, this->last_y );
        this->redraw();
    }
    
    //returns true if button was clicked
    bool menu_gui::wasClicked( const char *btn_name )
    {
        unsigned int i, e;
        menu_gui_btn *p;
        bool r = 0;
        
        e = (unsigned int)this->btns.size();
        for( i = 0; i < e; i++ )
        {
            p = &this->btns[ i ];
            if( btn_name && p->s.compare( btn_name ) != 0 )
                continue;
            r |= p->b->wasClicked();
        }
        
        return r;
    }
    
    //do processing
    void menu_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {

    }
    
};
