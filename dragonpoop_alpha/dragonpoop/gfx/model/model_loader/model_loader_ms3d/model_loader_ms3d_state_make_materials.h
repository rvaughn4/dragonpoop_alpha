
#ifndef dragonpoop_model_loader_ms3d_state_make_materials_h
#define dragonpoop_model_loader_ms3d_state_make_materials_h

#include "../model_loader_state.h"
#include <string>

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    struct ms3d_model_material_m;
    class dpbitmap;
    
    class model_loader_ms3d_state_make_materials : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //make material
        void makeMaterial( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_material_m *t, model_writelock *m );
        //load bitmap
        bool loadBitmap( dpbitmap *bm, std::string *fname );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_materials( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_materials( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif