
#include "renderer_gui_readlock.h"
#include "renderer_gui.h"

namespace dragonpoop
{

    //ctor
    renderer_gui_readlock::renderer_gui_readlock( renderer_gui *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_gui_readlock::~renderer_gui_readlock( void )
    {

    }

    //return core
    core *renderer_gui_readlock::getCore( void )
    {
        return this->t->getCore();
    }

    //returns id
    dpid renderer_gui_readlock::getId( void )
    {
        return this->t->getId();
    }

    //compares id
    bool renderer_gui_readlock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //get dimensions
    void renderer_gui_readlock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }

    //returns true if has background texture
    bool renderer_gui_readlock::hasBg( void )
    {
        return this->t->hasBg();
    }

    //returns true if has forground texture
    bool renderer_gui_readlock::hasFg( void )
    {
        return this->t->hasFg();
    }

    //returns opacity
    float renderer_gui_readlock::getOpacity( void )
    {
        return this->t->getOpacity();
    }

    //return bg vb
    dpvertexindex_buffer *renderer_gui_readlock::getBgBuffer( void )
    {
        return this->t->getBgBuffer();
    }

    //return fg vb
    dpvertexindex_buffer *renderer_gui_readlock::getFgBuffer( void )
    {
        return this->t->getFgBuffer();
    }

    //returns true if alive
    bool renderer_gui_readlock::isAlive( void )
    {
        return this->t->isAlive();
    }

    //returns true if has focus
    bool renderer_gui_readlock::hasFocus( void )
    {
        return this->t->hasFocus();
    }

    //gets gui id of focused child
    bool renderer_gui_readlock::getFocusChild( renderer_gui_man_writelock *r, dpid *fid )
    {
        return this->t->getFocusChild( r, fid );
    }

};
