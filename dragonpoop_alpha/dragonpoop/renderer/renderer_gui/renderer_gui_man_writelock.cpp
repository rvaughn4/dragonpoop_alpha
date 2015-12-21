
#include "renderer_gui_man_writelock.h"
#include "renderer_gui_man.h"

namespace dragonpoop
{

    //ctor
    renderer_gui_man_writelock::renderer_gui_man_writelock( renderer_gui_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_gui_man_writelock::~renderer_gui_man_writelock( void )
    {

    }

    //return core
    core *renderer_gui_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //delete guis
    void renderer_gui_man_writelock::deleteGuis( void )
    {
        this->t->deleteGuis();
    }

    //return guis
    void renderer_gui_man_writelock::getChildrenGuis( std::list<renderer_gui *> *l, dpid pid )
    {
        this->t->getChildrenGuis( l, pid );
    }

    //process mouse input
    bool renderer_gui_man_writelock::processGuiMouseInput( float w, float h, float x, float y, bool lb, bool rb )
    {
        return this->t->processGuiMouseInput( this, w, h, x, y, lb, rb );
    }

    //get hovering gui id
    dpid renderer_gui_man_writelock::getHoverId( void )
    {
        return this->t->getHoverId();
    }

    //process gui keyboard input
    void renderer_gui_man_writelock::processGuiKbInput( std::string *skey_name, bool isDown )
    {
        this->t->processGuiKbInput( this, skey_name, isDown );
    }

    //gets selected text from gui (copy or cut)
    bool renderer_gui_man_writelock::getSelectedText( std::string *s, bool bDoCut )
    {
        return this->t->getSelectedText( s, bDoCut );
    }

    //sets selected text in gui (paste)
    bool renderer_gui_man_writelock::setSelectedText( std::string *s )
    {
        return this->t->setSelectedText( s );
    }

    //run
    void renderer_gui_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }

};
