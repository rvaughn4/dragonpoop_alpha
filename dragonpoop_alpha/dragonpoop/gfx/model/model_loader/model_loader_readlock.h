
#ifndef dragonpoop_model_loader_readlock_h
#define dragonpoop_model_loader_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class model_loader;
    
    class model_loader_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_loader *t;
        
    protected:
        
        //ctor
        model_loader_readlock( model_loader *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_loader_readlock( void );
        
    public:
        
        //returns true if running
        bool isRunning( void );
        //returns true if sucessful
        bool wasSucessful( void );
        
        friend class model_loader;
    };
    
};

#endif