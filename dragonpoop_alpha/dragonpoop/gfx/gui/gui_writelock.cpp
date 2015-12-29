
#include "gui_writelock.h"
#include "gui.h"

namespace dragonpoop
{

    //ctor
    gui_writelock::gui_writelock( gui *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    gui_writelock::~gui_writelock( void )
    {

    }

    //run gui
    void gui_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }

    //return core
    core *gui_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //returns id
    dpid gui_writelock::getId( void )
    {
        return this->t->getId();
    }

    //compares id
    bool gui_writelock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //set width and height
    void gui_writelock::setWidthHeight( float w, float h )
    {
        this->t->setWidthHeight( w, h );
    }

    //set top left pos
    void gui_writelock::setPosition( float x, float y )
    {
        this->t->setPosition( x, y );
    }

    //get dimensions
    void gui_writelock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }

    //cause redraw of background and forground texture
    void gui_writelock::redraw( void )
    {
        this->t->redraw();
    }

    //set background redraw mode
    void gui_writelock::enableBgRedraw( bool b )
    {
        this->t->enableBgRedraw( b );
    }

    //set forground redraw mode
    void gui_writelock::enableFgRedraw( bool b )
    {
        this->t->enableFgRedraw( b );
    }

    //set background mode
    void gui_writelock::enableBg( bool b )
    {
        this->t->enableBg( b );
    }

    //set forground mode
    void gui_writelock::enableFg( bool b )
    {
        this->t->enableFg( b );
    }

    //returns true if has background texture
    bool gui_writelock::hasBg( void )
    {
        return this->t->hasBg();
    }

    //returns true if has forground texture
    bool gui_writelock::hasFg( void )
    {
        return this->t->hasFg();
    }

    //set parent id
    void gui_writelock::setParentId( dpid id )
    {
        this->t->setParentId( id );
    }

    //get parent id
    dpid gui_writelock::getParentId( void )
    {
        return this->t->getParentId();
    }

    //returns pointer to bg texture
    dpbitmap *gui_writelock::getBg( void )
    {
        return this->t->getBg();
    }

    //returns pointer to fg texture
    dpbitmap *gui_writelock::getFg( void )
    {
        return this->t->getFg();
    }

    //returns z order
    unsigned int gui_writelock::getZ( void )
    {
        return this->t->getZ();
    }

    //sets focus
    void gui_writelock::setFocus( void )
    {
        this->t->setFocus();
    }

    //returns true if has focus
    bool gui_writelock::hasFocus( void )
    {
        return this->t->hasFocus();
    }

    //process mouse input
    void gui_writelock::processMouse( float x, float y, float sx, float sy, bool lb, bool rb )
    {
        this->t->processMouse( x, y, sx, sy, lb, rb );
    }

    //process kb input
    void gui_writelock::processKb( std::string *skey, bool bDown )
    {
        this->t->processKb( skey, bDown );
    }

    //returns true if has renderer
    bool gui_writelock::hasRenderer( void )
    {
        return this->t->hasRenderer();
    }

    //set renderer
    void gui_writelock::setRenderer( renderer_gui *g )
    {
        this->t->setRenderer( g );
    }

    //gets selected text from gui (copy or cut)
    bool gui_writelock::getSelectedText( std::string *s, bool bDoCut )
    {
        return this->t->getSelectedText( s, bDoCut );
    }

    //sets selected text in gui (paste)
    bool gui_writelock::setSelectedText( std::string *s )
    {
        return this->t->setSelectedText( s );
    }

    //add gui
    void gui_writelock::addGui( gui *g )
    {
        return this->t->addGui( g );
    }

    //set editable
    void gui_writelock::setEditMode( bool b )
    {
        this->t->setEditMode( b );
    }

    //returns true if editable
    bool gui_writelock::isEditable( void )
    {
        return this->t->isEditable();
    }

    //sets hoverable
    void gui_writelock::setHoverMode( bool b )
    {
        this->t->setHoverMode( b );
    }

    //returns true if hoverable
    bool gui_writelock::isHoverable( void )
    {
        return this->t->isHoverable();
    }

    //returns true if gui should swoosh in and out
    bool gui_writelock::isFade( void )
    {
        return this->t->isFade();
    }

    //sets fade mode
    void gui_writelock::setFade( bool b )
    {
        this->t->setFade( b );
    }

    //set text
    void gui_writelock::setText( const char *c )
    {
        this->t->setText( c );
    }

    //set text
    void gui_writelock::setText( std::string *s )
    {
        this->t->setText( s );
    }

    //get text
    void gui_writelock::getText( std::string *s )
    {
        this->t->getText( s );
    }

};
