
#ifndef dragonpoop_perf_stats_gui_factory_h
#define dragonpoop_perf_stats_gui_factory_h

#include "../gui_factory.h"

namespace dragonpoop
{

    class perf_stats_gui_factory : public gui_factory
    {

    private:

    protected:

        //generate gui
        virtual gui *genGui( gfx_writelock *g, dpid id );

    public:

        //ctor
        perf_stats_gui_factory( core *c );
        //dtor
        virtual ~perf_stats_gui_factory( void );

    };

};

#endif
