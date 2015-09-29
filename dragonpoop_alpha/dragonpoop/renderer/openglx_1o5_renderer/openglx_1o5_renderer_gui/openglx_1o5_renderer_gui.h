
#ifndef dragonpoop_openglx_1o5_renderer_gui_h
#define dragonpoop_openglx_1o5_renderer_gui_h

#include "../../renderer_gui/renderer_gui.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_gui_writelock;
    class openglx_1o5_renderer_gui_readlock;
    class openglx_1o5_renderer_writelock;
    
    class openglx_1o5_renderer_gui : public renderer_gui
    {
        
    private:
        
        unsigned int bg_tex, fg_tex;
        
        //release texture
        void killTex( unsigned int *t );
        //create and load texture
        void makeTex( unsigned int *t, dpbitmap *bm, bool isBg );

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //override to handle bg texture update
        virtual void updateBg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm );
        //override to handle fg texture update
        virtual void updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm );
        //return bg texture
        unsigned int getBgTex( void );
        //return fg texture
        unsigned int getFgTex( void );
        
    public:
        
        //ctor
        openglx_1o5_renderer_gui( gui_writelock *g );
        //dtor
        virtual ~openglx_1o5_renderer_gui( void );
        
        friend class openglx_1o5_renderer_gui_readlock;
        friend class openglx_1o5_renderer_gui_writelock;
        
    };
    
};

#endif