
#ifndef dragonpoop_renderer_ref_h
#define dragonpoop_renderer_ref_h

#include "../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{

    class renderer;

    class renderer_ref : public shared_obj_ref
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_ref( renderer *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~renderer_ref( void );
        
        friend class renderer;
        friend class renderer_gui_man;
        friend class renderer_model_man;
        friend class renderer_model_man_task;
        friend class renderer_gui_man_task;
        friend class renderer_task;
    };
    
};

#endif