
#ifndef dragonpoop_gui_factory_ref_h
#define dragonpoop_gui_factory_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class gui_factory;
    
    class gui_factory_ref : public shared_obj_ref
    {
        
    private:
        
        gui_factory *t;
        
    protected:
        
        //ctor
        gui_factory_ref( gui_factory *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~gui_factory_ref( void );
        
        friend class gui_factory;
    };
    
};

#endif