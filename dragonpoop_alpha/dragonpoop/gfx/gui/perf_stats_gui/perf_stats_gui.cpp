
#include "perf_stats_gui.h"

namespace dragonpoop
{
    
    //ctor
    perf_stats_gui::perf_stats_gui( gfx_writelock *g, dpid id, dpid pid ) : gui( g, id )
    {
        this->setParentId( pid );
        this->setPosition( 0, 1080 - 600 );
        this->setWidthHeight( 400, 600 );
        this->setEditMode( 1 );
        this->enableBg( 1 );
        this->enableFg( 1 );
    }
    
    //dtor
    perf_stats_gui::~perf_stats_gui( void )
    {
        
    }
    
    //override to paint background texture
    void perf_stats_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }
    
};
