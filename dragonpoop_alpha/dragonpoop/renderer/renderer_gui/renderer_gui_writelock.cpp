
#include "renderer_gui_writelock.h"
#include "renderer_gui.h"

namespace dragonpoop
{

    //ctor
    renderer_gui_writelock::renderer_gui_writelock( renderer_gui *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_gui_writelock::~renderer_gui_writelock( void )
    {
        
    }
    
    //run gui
    void renderer_gui_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //return core
    core *renderer_gui_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //returns id
    dpid renderer_gui_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //compares id
    bool renderer_gui_writelock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }
    
    //get dimensions
    void renderer_gui_writelock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }
    
    //returns true if has background texture
    bool renderer_gui_writelock::hasBg( void )
    {
        return this->t->hasBg();
    }
    
    //returns true if has forground texture
    bool renderer_gui_writelock::hasFg( void )
    {
        return this->t->hasFg();
    }
    
    //get parent id
    dpid renderer_gui_writelock::getParentId( void )
    {
        return this->t->getParentId();
    }

    //compares parent id
    bool renderer_gui_writelock::compareParentId( dpid id )
    {
        return this->t->compareParentId( id );
    }

    //called to force pos update
    void renderer_gui_writelock::syncPos( void )
    {
        this->t->syncPos();
    }
    
    //called to force bg update
    void renderer_gui_writelock::syncBg( void )
    {
        this->t->syncBg();
    }
    
    //called to force fg update
    void renderer_gui_writelock::syncFg( void )
    {
        this->t->syncFg();
    }
    
    //redo matrix
    void renderer_gui_writelock::redoMatrix( dpthread_lock *thd, renderer_writelock *r, dpmatrix *p_matrix )
    {
        this->t->redoMatrix( thd, r, this, p_matrix );
    }
    
    //process mouse input
    bool renderer_gui_writelock::processMouse( float x, float y, bool lb, bool rb )
    {
        return this->t->processMouse( x, y, lb, rb );
    }
    
    //process kb input
    bool renderer_gui_writelock::processKb( std::string *sname, bool bIsDown )
    {
        return this->t->processKb( sname, bIsDown );
    }
    
    //returns true if alive
    bool renderer_gui_writelock::isAlive( void )
    {
        return this->t->isAlive();
    }
    
    //kills gui
    void renderer_gui_writelock::kill( void )
    {
        this->t->kill();
    }
    
    //returns true if has focus
    bool renderer_gui_writelock::hasFocus( void )
    {
        return this->t->hasFocus();
    }
    
    //gets gui id of focused child
    bool renderer_gui_writelock::getFocusChild( renderer_writelock *r, dpid *fid )
    {
        return this->t->getFocusChild( r, fid );
    }
    
    //gets selected text from gui (copy or cut)
    bool renderer_gui_writelock::getSelectedText( std::string *s, bool bDoCut )
    {
        return this->t->getSelectedText( s, bDoCut );
    }
    
    //sets selected text in gui (paste)
    bool renderer_gui_writelock::setSelectedText( std::string *s )
    {
        return this->t->setSelectedText( s );
    }
    
};
