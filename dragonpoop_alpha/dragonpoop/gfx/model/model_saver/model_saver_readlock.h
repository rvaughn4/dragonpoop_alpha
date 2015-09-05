
#ifndef dragonpoop_model_saver_readlock_h
#define dragonpoop_model_saver_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class model_saver;
    class model_ref;
    
    class model_saver_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_saver *t;
        
    protected:
        
        //ctor
        model_saver_readlock( model_saver *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_saver_readlock( void );
        
    public:
        
        //returns true if running
        bool isRunning( void );
        //returns true if sucessful
        bool wasSucessful( void );
        //returns model
        model_ref *getModel( void );
        
        friend class model_saver;
    };
    
};

#endif