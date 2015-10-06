
#include "root_gui.h"
#include "../menu_gui/menu_gui.h"

namespace dragonpoop
{
    
    //ctor
    root_gui::root_gui( gfx_writelock *g, dpid id ) : gui( g, id )
    {
        this->enableBg( 0 );
        this->enableFg( 0 );
        this->setFade( 0 );
        this->setPosition( 0, 0 );
        this->setWidthHeight( 1920, 1080 );
        
        this->b = new menu_gui( g, this->genId(), this->getId(), 100, 100, 400, 800, 100, "testing..." );
        this->addGui( this->b );
    }
    
    //dtor
    root_gui::~root_gui( void )
    {
        delete this->b;
    }
    
};
