
#ifndef dragonpoop_perf_stats_gui_h
#define dragonpoop_perf_stats_gui_h

#include "../gui.h"

namespace dragonpoop
{
    
    class gfx_ref;
    class button_gui;
    
    class perf_stats_gui : public gui
    {
        
    private:
        
        uint64_t t_last_update;
        gfx_ref *g;
        button_gui *bclose, *bhide;
        bool bDoClose;
        
    protected:
        
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );

    public:
        
        //ctor
        perf_stats_gui( gfx_writelock *g, dpid id, dpid pid );
        //dtor
        virtual ~perf_stats_gui( void );
        //returns true if closed
        bool wasClosed( void );
        
    };
    
};

#endif