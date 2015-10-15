
#ifndef dragonpoop_model_loader_man_readlock_h
#define dragonpoop_model_loader_man_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    
    class model_loader_man;
    class core;
    
    class model_loader_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_loader_man *t;
        
    protected:
        
        //ctor
        model_loader_man_readlock( model_loader_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_loader_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        
        friend class model_loader_man;
    };
    
};

#endif