
#ifndef dragonpoop_root_gui_factory_h
#define dragonpoop_root_gui_factory_h

#include "../gui_factory.h"

namespace dragonpoop
{
    
    class root_gui_factory : public gui_factory
    {
        
    private:
        
    protected:
        
        //generate gui
        virtual gui *genGui( gfx_writelock *g, dpid id );
        
    public:
        
        //ctor
        root_gui_factory( core *c );
        //dtor
        virtual ~root_gui_factory( void );
        
    };
    
};

#endif