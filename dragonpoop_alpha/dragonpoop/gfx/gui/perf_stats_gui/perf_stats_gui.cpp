
#include "perf_stats_gui.h"
#include "../../../core/dpthread/dpthread_lock.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../gfx_readlock.h"
#include "../../gfx_writelock.h"
#include "../../gfx_ref.h"
#include "../button_gui/button_gui.h"
#include "../../../renderer/renderer_ref.h"
#include "../../../renderer/renderer_readlock.h"

#include <sstream>

namespace dragonpoop
{
    
    //ctor
    perf_stats_gui::perf_stats_gui( gfx_writelock *g, dpid id, dpid pid ) : gui( g, id )
    {
        this->g = (gfx_ref *)g->getRef();
        
        this->setParentId( pid );
        this->setPosition( 0, 1080 - 600 );
        this->setWidthHeight( 400, 600 );
        this->setEditMode( 0 );
        this->enableBg( 1 );
        this->enableFg( 1 );
        this->setFontSize( 20 );
        this->setMargin( 2 );
        this->setFade( 1 );
        this->bDoClose = 0;
        
        this->bclose = new button_gui( g, this->genId(), id, 400 - 42, 2, 40, 40, "X", 1 );
        this->bhide = new button_gui( g, this->genId(), id, 400 - 84, 2, 40, 40, "-", 1 );
        this->addGui( this->bclose );
        this->addGui( this->bhide );
    }
    
    //dtor
    perf_stats_gui::~perf_stats_gui( void )
    {
        delete this->bclose;
        delete this->bhide;
        delete this->g;
    }
    
    //override to paint background texture
    void perf_stats_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }
  
    //override to do processing
    void perf_stats_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        uint64_t t;
        std::stringstream ss;
        std::string s;
        shared_obj_guard og, o;
        gfx_readlock *gl;
        renderer_ref *rr;
        renderer_readlock *rl;
        int i;
        
        if( this->bclose->wasClicked() )
            this->bDoClose = 1;
        
        t = thd->getTicks();
        if( t - this->t_last_update < 4000 )
            return;
        this->t_last_update = t;
        
        gl = (gfx_readlock *)og.tryReadLock( this->g, 1000, "perf_stats_gui::doProcessing" );
        if( !gl )
            return;
        ss << "Performance Statistics\r\n";
        
        rr = gl->getRenderer();
        if( rr )
        {
            rl = (renderer_readlock *)o.tryReadLock( rr, 100, "perf_stats_gui::doProcessing" );
            if( rl )
            {
                rl->getName( &s );
                ss << "\r\nRenderer ( " << s << " )\r\n";
                ss << "\t" << (int)rl->getFps() << " frames per second\r\n";
                ss << "\t" << rl->getMsPerFrame() << " ms per frame\r\n";
                i = rl->getTextureMemory() / 1048576;
                ss << "\t" << i << " MB texture memory\r\n";
                i = rl->getVertexMemory() / 1048576;
                ss << "\t" << i << " MB vertex memory\r\n";
            }
        }
        
        
        this->setText( ss.str().c_str() );
        this->redraw();
    }
    
    //returns true if closed
    bool perf_stats_gui::wasClosed( void )
    {
        return this->bDoClose;
    }
    
};
