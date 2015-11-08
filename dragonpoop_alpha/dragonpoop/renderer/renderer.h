
#ifndef dragonpoop_renderer_h
#define dragonpoop_renderer_h

#include "../core/shared_obj/shared_obj.h"
#include "../gfx/dpmatrix/dpmatrix.h"
#include "../gfx/dpposition/dpposition.h"
#include "x11_opengl_1o5_renderer/x11_opengl_1o5_renderer_factory.h"

namespace dragonpoop
{

    class dptaskpool_writelock;
    class dptask;
    class renderer_task;
    class dpthread_lock;
    class dptask_writelock;
    class core;
    class gfx_writelock;
    class renderer_writelock;
    class gfx_ref;
    class gfx_writelock;
    class renderer_model;
    class model_writelock;
    class renderer_model_instance_group;
    class renderer_model_instance_readlock;
    class renderer_model_readlock;
    class renderer_model_material;
    class renderer_gui;
    class gui_writelock;
    class renderer_gui_readlock;
    class model_man_ref;
    class gui_man_ref;
    class renderer_gui_man;
    class gfx;
    class renderer_state;
    class dptaskpool_ref;
    class renderer_gui_man;
    class renderer_model_man;
    class render_api;
    class render_api_writelock;
    class render_api_context_ref;
    class render_api_commandlist_ref;
    class renderer_gui_man_writelock;
    class dpthread_singletask;
    class renderer_commandlist_passer;
    class renderer_land_man;

    class renderer : public shared_obj
    {

    private:

        dptask *tsk;
        renderer_task *gtsk;
        core *c;
        gfx_ref *g;
        renderer_gui_man *rgui_mgr;
        renderer_model_man *rmodel_mgr;
        renderer_land_man *rland_mgr;
        dptaskpool_ref *tp;
        dpthread_singletask *thd;
        render_api *api;
        render_api_context_ref *main_ctx;
        renderer_commandlist_passer *clpasser;
        
        std::atomic<bool> bDoRun, bIsRun;
        render_api_commandlist_ref *new_gui_cl, *new_model_cl, *new_land_cl, *new_sky_cl;
        render_api_commandlist_ref *gui_cl, *model_cl, *land_cl, *sky_cl;
        
        uint64_t t_last_fps, ms_each_frame, t_last_input;
        float fps, fthiss;
        bool bActive, bActiveOld, bCamSync;
        dpmatrix m_world, m_gui, m_gui_undo;
        dpposition cam_pos;
        renderer_state *cs;
        
        //run renderer
        void state_run( dpthread_lock *thd, renderer_writelock *rl );
        //init api
        bool state_initApi( dpthread_lock *thd, renderer_writelock *rl );
        //init gui manager
        bool state_initGui( dpthread_lock *thd, renderer_writelock *rl );
        //init model manager
        bool state_initModel( dpthread_lock *thd, renderer_writelock *rl );
        //start api
        void state_startApi( dpthread_lock *thd, renderer_writelock *rl );
        //start gui manager
        void state_startGui( dpthread_lock *thd, renderer_writelock *rl );
        //start model manager
        void state_startModel( dpthread_lock *thd, renderer_writelock *rl );
        //stop api
        void state_stopApi( dpthread_lock *thd, renderer_writelock *rl );
        //stop gui manager
        void state_stopGui( dpthread_lock *thd, renderer_writelock *rl );
        //stop model manager
        void state_stopModel( dpthread_lock *thd, renderer_writelock *rl );
        //deinit api
        void state_deinitApi( dpthread_lock *thd, renderer_writelock *rl );
        //init gui manager
        void state_deinitGui( dpthread_lock *thd, renderer_writelock *rl );
        //init model manager
        void state_deinitModel( dpthread_lock *thd, renderer_writelock *rl );
        //render
        void render( dpthread_lock *thd, renderer_writelock *rl );
        //sync camera
        void _syncCam( void );
        
    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //stop renderer task
        void kill( void );
        //run renderer from task
        void run( dptask_writelock *tskl, dpthread_lock *thd, renderer_writelock *r );
        //init graphics api
        virtual bool initApi( void );
        //deinit graphics api
        virtual void deinitApi( void );
        //do background graphics api processing
        virtual bool runApi( renderer_writelock *r, dpthread_lock *thd );
        //stop task and deinit api
        void _kill( void );
        //return screen/window width
        virtual unsigned int getWidth( void );
        //return screen/window height
        virtual unsigned int getHeight( void );
        //prepare for rendering world
        virtual void prepareWorldRender( unsigned int w, unsigned int h );
        //prepare for rendering gui
        virtual void prepareGuiRender( unsigned int w, unsigned int h );
        
        //generate renderer model
        virtual renderer_model_man *genModelMan( dptaskpool_writelock *tp );
        //generate renderer gui manager
        virtual renderer_gui_man *genGuiMan( dptaskpool_writelock *tp );
        //generate renderer land manager
        virtual renderer_land_man *genLandMan( dptaskpool_writelock *tp );
        
        //returns fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsPerFrame( void );
        //get renderer name
        virtual void getName( std::string *s );
        //get texture memory used
        virtual unsigned int getTextureMemory( void );
        //get vertex memory used
        virtual unsigned int getVertexMemory( void );
        //process mouse input
        void processMouseInput( renderer_gui_man_writelock *gl, render_api_writelock *al, float x, float y, bool lb, bool rb );
        //process keyboard input
        void processKbInput( renderer_gui_man_writelock *gl, std::string *skey_name, bool isDown );
        //gets camera position
        void getCameraPosition( dpposition *p );
        //sync camera position
        void syncCamera( void );
        //get position difference from camera
        void getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //generate render api
        virtual render_api *genRenderApi( dpmutex_master *mm );

    public:

        //ctor
        renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~renderer( void );
        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //populate renderer list
        static void addRenderers( gfx *g );

        friend class renderer_readlock;
        friend class renderer_writelock;
        friend class renderer_state;
        friend class renderer_model_man_task;
        friend class renderer_gui_man_task;
        friend class renderer_model_man;
        friend class renderer_gui_man;
        friend class renderer_task;
        
        friend class openglx_1o5_renderer;
    };
    
};

#endif