
#ifndef dragonpoop_renderer_h
#define dragonpoop_renderer_h

#include "../core/shared_obj/shared_obj.h"
#include "../gfx/dpmatrix/dpmatrix.h"
#include "../gfx/dpposition/dpposition.h"

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

    class renderer : public shared_obj
    {

    private:

        dptask *tsk;
        renderer_task *gtsk;
        core *c;
        gfx_ref *g;
        model_man_ref *m;
        gui_man_ref *gui_mgr;
        renderer_gui_man *rgui_mgr;
        dptaskpool_ref *tp;
        
        std::atomic<bool> bDoRun, bIsRun;
        std::list<renderer_model *> models;
        std::list<renderer_gui *> guis;
        
        uint64_t t_last_m_ran, t_last_m_synced, t_last_gui_ran, t_last_gui_synced, t_last_fps, ms_each_frame;
        float fps, fthiss;
        bool bActive, bActiveOld, bCamSync;
        dpmatrix m_world, m_gui, m_gui_undo;
        dpid hover_gui, focus_gui;
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


        //run models
        void runModels( dpthread_lock *thd, renderer_writelock *rl );
        //delete models
        void deleteModels( void );
        //run guis
        void runGuis( dpthread_lock *thd, renderer_writelock *rl );
        //delete guis
        void deleteGuis( void );
        //render guis
        void renderGuis( dpthread_lock *thd, renderer_writelock *rl, dpmatrix *m_world );
        //render
        void render( dpthread_lock *thd, renderer_writelock *rl );
        //render models
        void renderModels( dpthread_lock *thd, renderer_writelock *rl, bool doGui, dpmatrix *m_world );
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
        //set viewport size
        virtual void setViewport( unsigned int w, unsigned int h );
        //clear screen with color
        virtual void clearScreen( float r, float g, float b );
        //prepare for rendering world
        virtual void prepareWorldRender( unsigned int w, unsigned int h );
        //prepare for rendering gui
        virtual void prepareGuiRender( unsigned int w, unsigned int h );
        //flip backbuffer and present scene to screen
        virtual void flipBuffer( void );
        //generate renderer model
        virtual renderer_model *genModel( model_writelock *ml );
        //generate renderer gui
        virtual renderer_gui *genGui( gui_writelock *ml );
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
        //render model instance group
        virtual void renderGroup( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world ) = 0;
        //render gui
        virtual void renderGui( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world ) = 0;
        //return guis
        void getChildrenGuis( std::list<renderer_gui *> *l, dpid pid );
        //process mouse input
        void processMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb );
        //process mouse input
        bool processGuiMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb );
        //get hovering gui id
        dpid getHoverId( void );
        //process keyboard input
        void processKbInput( std::string *skey_name, bool isDown );
        //process gui keyboard input
        void processGuiKbInput( std::string *skey_name, bool isDown );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //gets camera position
        void getCameraPosition( dpposition *p );
        //sync camera position
        void syncCamera( void );
        //get position difference from camera
        void getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz );
        
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
    };
    
};

#endif