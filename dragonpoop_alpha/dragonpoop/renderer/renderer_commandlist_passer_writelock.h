
#ifndef dragonpoop_renderer_commandlist_passer_writelock_h
#define dragonpoop_renderer_commandlist_passer_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class renderer_commandlist_passer;
    class render_api_commandlist_ref;
    class dpposition;
    
    class renderer_commandlist_passer_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_commandlist_passer *t;
        
    protected:
        
        //ctor
        renderer_commandlist_passer_writelock( renderer_commandlist_passer *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_commandlist_passer_writelock( void );
        
    public:
        
        //set camera position
        void setPosition( dpposition *p );
        //get camera position
        dpposition *getPosition( void );
        //set model commandlist
        void setModel( render_api_commandlist_ref *r );
        //get model commandlist
        render_api_commandlist_ref *getModel( void );
        //set gui commandlist
        void setGui( render_api_commandlist_ref *r );
        //get gui commandlist
        render_api_commandlist_ref *getGui( void );
        //set land commandlist
        void setLand( render_api_commandlist_ref *r );
        //get land commandlist
        render_api_commandlist_ref *getLand( void );

        friend class renderer_commandlist_passer;
    };
    
};

#endif