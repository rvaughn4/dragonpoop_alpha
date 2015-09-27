
#include "openglx_1o5_renderer_gui.h"
#include "openglx_1o5_renderer_gui_ref.h"
#include "openglx_1o5_renderer_gui_readlock.h"
#include "openglx_1o5_renderer_gui_writelock.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_gui::openglx_1o5_renderer_gui( gui_writelock *g ) : renderer_gui( g )
    {
        
    }
    
    //dtor
    openglx_1o5_renderer_gui::~openglx_1o5_renderer_gui( void )
    {
        
    }
    
    //generate read lock
    shared_obj_readlock *openglx_1o5_renderer_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new openglx_1o5_renderer_gui_readlock( (openglx_1o5_renderer_gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *openglx_1o5_renderer_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new openglx_1o5_renderer_gui_writelock( (openglx_1o5_renderer_gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *openglx_1o5_renderer_gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new openglx_1o5_renderer_gui_ref( (openglx_1o5_renderer_gui *)p, k );
    }
    
    //override to handle bg texture update
    void openglx_1o5_renderer_gui::updateBg( openglx_1o5_renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
    //override to handle fg texture update
    void openglx_1o5_renderer_gui::updateFg( openglx_1o5_renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
};
