
#include "gui_readlock.h"
#include "gui.h"

namespace dragonpoop
{

    //ctor
    gui_readlock::gui_readlock( gui *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    gui_readlock::~gui_readlock( void )
    {

    }

    //return core
    core *gui_readlock::getCore( void )
    {
        return this->t->getCore();
    }

    //returns id
    dpid gui_readlock::getId( void )
    {
        return this->t->getId();
    }

    //compares id
    bool gui_readlock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //get dimensions
    void gui_readlock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }

    //returns true if has background texture
    bool gui_readlock::hasBg( void )
    {
        return this->t->hasBg();
    }

    //returns true if has forground texture
    bool gui_readlock::hasFg( void )
    {
        return this->t->hasFg();
    }

    //returns pointer to bg texture
    dpbitmap *gui_readlock::getBg( void )
    {
        return this->t->getBg();
    }

    //returns pointer to fg texture
    dpbitmap *gui_readlock::getFg( void )
    {
        return this->t->getFg();
    }

    //add gui
    void gui_readlock::addGui( gui *g )
    {
        return this->t->addGui( g );
    }

    //returns true if editable
    bool gui_readlock::isEditable( void )
    {
        return this->t->isEditable();
    }

    //returns true if hoverable
    bool gui_readlock::isHoverable( void )
    {
        return this->t->isHoverable();
    }

    //returns true if gui should swoosh in and out
    bool gui_readlock::isFade( void )
    {
        return this->t->isFade();
    }

    //get text
    void gui_readlock::getText( std::string *s )
    {
        this->t->getText( s );
    }

    //get children
    void gui_readlock::getChildren( std::list<gui_ref *> *l )
    {
        this->t->getChildren( l );
    }

    //returns true if follows cursor
    bool gui_readlock::followsCursor( float *px, float *py )
    {
        return this->t->followsCursor( px, py );
    }

};
