
#ifndef dragonpoop_gui_man_ref_h
#define dragonpoop_gui_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class gui_man;
    
    class gui_man_ref : public shared_obj_ref
    {
        
    private:
        
        gui_man *t;
        
    protected:
        
        //ctor
        gui_man_ref( gui_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~gui_man_ref( void );
        
        friend class gui_man;
    };
    
};

#endif