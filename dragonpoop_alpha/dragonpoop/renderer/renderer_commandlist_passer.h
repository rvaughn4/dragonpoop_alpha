
#ifndef dragonpoop_renderer_commandlist_passer_h
#define dragonpoop_renderer_commandlist_passer_h

#include "../core/shared_obj/shared_obj.h"
#include "../gfx/dpposition/dpposition.h"

namespace dragonpoop
{
    
    class render_api_commandlist_ref;
    
    class renderer_commandlist_passer : public shared_obj
    {
        
    private:
        
        std::atomic<render_api_commandlist_ref *> rmodel, rgui, rland;
        dpposition pos;
        std::atomic<bool> model_ready, gui_ready, land_ready;
        float w, h;
        
    protected:
        
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
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
        
    public:
        
        //ctor
        renderer_commandlist_passer( dpmutex_master *mm );
        //dtor
        virtual ~renderer_commandlist_passer( void );
        
        
        friend class renderer_commandlist_passer_writelock;
        friend class renderer_commandlist_passer_readlock;
        friend class renderer_commandlist_passer_ref;
        friend class renderer;
        friend class renderer_gui_man;
        friend class renderer_model_man;
        friend class renderer_land_man;

    };
    
};

#endif