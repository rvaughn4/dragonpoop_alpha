
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
#include "../../dpposition/dpposition.h"
#include "../../dpposition/dpposition_share_ref.h"
#include "../../dpposition/dpposition_share_readlock.h"

#include "../gui_man_readlock.h"
#include "../../model/model_man_readlock.h"
#include "../gui_man_ref.h"
#include "../../model/model_man_ref.h"

#include <sstream>

namespace dragonpoop
{

    //ctor
    perf_stats_gui::perf_stats_gui( gfx_writelock *g, dpid id, dpid pid ) : window_gui( g, id, pid, 0, 1080-600, 400, 600, "Performance Stats" )
    {
        this->g = (gfx_ref *)g->getRef();
        this->rcampos = g->getCameraPosition();
    }

    //dtor
    perf_stats_gui::~perf_stats_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "root_gui::~root_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "root_gui::~root_gui" );
        delete this->g;
        delete this->rcampos;
        o.unlock();
    }

    //override to do processing
    void perf_stats_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        uint64_t t;
        std::stringstream ss;
        std::string s;
        shared_obj_guard og, o, ogui, omod;
        gfx_readlock *gl;
        renderer_ref *rr;

        renderer_readlock *rl;
        dpposition_inner ppi;
        int i;
        gui_man_readlock *guil;
        model_man_readlock *modl;
        dpposition_share_readlock *pl;

        this->window_gui::doProcessing( thd, g );

        t = thd->getTicks();
        if( t - this->t_last_update < 1500 )
            return;
        this->t_last_update = t;

        gl = (gfx_readlock *)og.tryReadLock( this->g, 1000, "perf_stats_gui::doProcessing" );
        if( !gl )
            return;
        if( !gl->getModels( &modl, &omod ) )
            return;
        if( !gl->getGuis( &guil, &ogui ) )
            return;
        rr = gl->getRenderer();
        og.unlock();

        ss << "\fsans \e016";

        //gfx stats
        ss << "Graphics \r\n";
        ss << "\t" << modl->getModelCount() << " models open\r\n";
        ss << "\t" << modl->getInstanceCount() << " model instances\r\n";
        ss << "\t" << guil->getGuiCount() << " guis open\r\n";

        //camera
        if( this->rcampos && this->campos_t != this->rcampos->getTime() )
        {
            pl = (dpposition_share_readlock *)og.tryReadLock( this->rcampos, 100, "perf_stats_gui::doProcessing" );
            if( pl )
            {
                this->campos.copy( pl->getPosition() );
                this->campos_t = pl->getTime();
                og.unlock();
            }
        }
        this->campos.getData( &ppi );
        ss << "\r\nCamera \r\n";
        ss << "\tX " << ppi.end.x << " meters\r\n";
        ss << "\tY " << ppi.end.y << " meters\r\n";
        ss << "\tZ " << ppi.end.z << " meters\r\n";

        //renderer stats
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
        delete rr;

        this->setText( ss.str().c_str() );
        this->redraw();
    }

};
