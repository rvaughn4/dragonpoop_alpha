
#ifndef dragonpoop_model_loader_ms3d_h
#define dragonpoop_model_loader_ms3d_h

#include "../model_loader.h"

namespace dragonpoop
{
    
    class model_loader_ms3d : public model_loader
    {
        
    private:
        
    protected:
        
        //generate second state
        virtual model_loader_state *genState( dpbuffer *b );
        
    public:
        
        //ctor
        model_loader_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname );
        //dtor
        virtual ~model_loader_ms3d( void );
        
    };
    
};

#endif