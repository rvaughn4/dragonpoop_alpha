
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
    
    //run from manager thread
    void renderer_gui_man_writelock::runFromTask( dpthread_lock *thd )
    {
        this->t->runFromTask( thd, this );
    }
    
    //run from renderer thread
    void renderer_gui_man_writelock::runFromRenderer( dpthread_lock *thd )
    {
        this->t->runFromRenderer( thd, this );
    }
  
    //delete guis
    void renderer_gui_man_writelock::deleteGuis( void )
    {
        this->t->deleteGuis();
    }
    
    //render guis
    void renderer_gui_man_writelock::renderGuis( dpthread_lock *thd, renderer_writelock *rl, dpmatrix *m_world )
    {
        this->t->renderGuis( thd, rl, m_world );
    }
    
    //return guis
    void renderer_gui_man_writelock::getChildrenGuis( std::list<renderer_gui *> *l, dpid pid )
    {
        this->t->getChildrenGuis( l, pid );
    }
    
    //process mouse input
    bool renderer_gui_man_writelock::processGuiMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb )
    {
        return this->t->processGuiMouseInput( r, x, y, lb, rb );
    }
    
    //get hovering gui id
    dpid renderer_gui_man_writelock::getHoverId( void )
    {
        return this->t->getHoverId();
    }
    
    //process gui keyboard input
    void renderer_gui_man_writelock::processGuiKbInput( std::string *skey_name, bool isDown )
    {
        this->t->processGuiKbInput( skey_name, isDown );
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
    
};
