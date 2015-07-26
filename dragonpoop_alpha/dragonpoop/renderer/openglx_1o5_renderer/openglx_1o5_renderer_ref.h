
#ifndef dragonpoop_openglx_1o5_renderer_ref_h
#define dragonpoop_openglx_1o5_renderer_ref_h

#include "../renderer_ref.h"

namespace dragonpoop
{

    class openglx_1o5_renderer;

    class openglx_1o5_renderer_ref : public renderer_ref
    {

    private:

        openglx_1o5_renderer *t;

    protected:

        //ctor
        openglx_1o5_renderer_ref( openglx_1o5_renderer *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~openglx_1o5_renderer_ref( void );

        friend class openglx_1o5_renderer;
    };
    
};

#endif