
#ifndef dragonpoop_renderer_state_h
#define dragonpoop_renderer_state_h

namespace dragonpoop
{
    
    class renderer;
    class dpthread_lock;
    class renderer_writelock;
    
    class renderer_state
    {
        
    private:
        
        renderer *r;
        
    protected:
        
        //ctor
        renderer_state( renderer *r );
        //run renderer
        void _run( dpthread_lock *thd, renderer_writelock *rl );
        //init api
        bool initApi( dpthread_lock *thd, renderer_writelock *rl );
        //init gui manager
        bool initGui( dpthread_lock *thd, renderer_writelock *rl );
        //init model manager
        bool initModel( dpthread_lock *thd, renderer_writelock *rl );
        //start api
        void startApi( dpthread_lock *thd, renderer_writelock *rl );
        //start gui manager
        void startGui( dpthread_lock *thd, renderer_writelock *rl );
        //start model manager
        void startModel( dpthread_lock *thd, renderer_writelock *rl );
        //stop api
        void stopApi( dpthread_lock *thd, renderer_writelock *rl );
        //stop gui manager
        void stopGui( dpthread_lock *thd, renderer_writelock *rl );
        //stop model manager
        void stopModel( dpthread_lock *thd, renderer_writelock *rl );
        //deinit api
        void deinitApi( dpthread_lock *thd, renderer_writelock *rl );
        //init gui manager
        void deinitGui( dpthread_lock *thd, renderer_writelock *rl );
        //init model manager
        void deinitModel( dpthread_lock *thd, renderer_writelock *rl );
        //kill renderer
        void kill( void );
        //returns true if should run
        bool doRun( void );
        
    public:
        
        //dtor
        virtual ~renderer_state( void );
        //run, returns next state
        virtual renderer_state *run( renderer *r, renderer_writelock *rl, dpthread_lock *thd );
        
    };
    
};

#endif