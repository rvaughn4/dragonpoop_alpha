
#ifndef dragonpoop_render_api_texture_writelock_h
#define dragonpoop_render_api_texture_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_texture;

    class render_api_texture_writelock : public shared_obj_writelock
    {

    private:

        render_api_texture *t;

    protected:

        //ctor
        render_api_texture_writelock( render_api_texture *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_texture_writelock( void );

    public:

        //returns true if works
        bool works( void );

        friend class render_api_texture;
    };

};

#endif
