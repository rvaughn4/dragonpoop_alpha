
#ifndef dragonpoop_gui_ref_h
#define dragonpoop_gui_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class gui;
    
    class gui_ref : public shared_obj_ref
    {
        
    private:
        
        gui *t;
        
    protected:
        
        //ctor
        gui_ref( gui *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~gui_ref( void );
        
        friend class gui;
    };
    
};

#endif