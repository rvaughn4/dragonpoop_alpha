
#ifndef dragonpoop_perf_stats_gui_h
#define dragonpoop_perf_stats_gui_h

#include "../gui.h"

namespace dragonpoop
{
    
    class perf_stats_gui : public gui
    {
        
    private:
        
    protected:
        
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        
    public:
        
        //ctor
        perf_stats_gui( gfx_writelock *g, dpid id, dpid pid );
        //dtor
        virtual ~perf_stats_gui( void );
        
    };
    
};

#endif