
#include "perf_stats_gui_factory.h"
#include "perf_stats_gui.h"

namespace dragonpoop
{
    
    //ctor
    perf_stats_gui_factory::perf_stats_gui_factory( core *c ) : gui_factory( c )
    {
        
    }
    
    //dtor
    perf_stats_gui_factory::~perf_stats_gui_factory( void )
    {
        
    }
    
    //generate gui
    gui *perf_stats_gui_factory::genGui( gfx_writelock *g, dpid id, dpid pid )
    {
        return new perf_stats_gui( g, id, pid );
    }
    
};
