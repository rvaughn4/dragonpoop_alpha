
#include "openglx_1o5_renderer_gui_readlock.h"
#include "openglx_1o5_renderer_gui.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_gui_readlock::openglx_1o5_renderer_gui_readlock( openglx_1o5_renderer_gui *t, dpmutex_readlock *l ) : renderer_gui_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_gui_readlock::~openglx_1o5_renderer_gui_readlock( void )
    {
        
    }

    //return bg texture
    unsigned int openglx_1o5_renderer_gui_readlock::getBgTex( void )
    {
        return this->t->getBgTex();
    }
    
    //return fg texture
    unsigned int openglx_1o5_renderer_gui_readlock::getFgTex( void )
    {
        return this->t->getFgTex();
    }

};
