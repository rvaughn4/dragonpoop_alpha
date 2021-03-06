
#ifndef dragonpoop_dpactor_state_h
#define dragonpoop_dpactor_state_h

namespace dragonpoop
{

    class dpactor;
    class dpthread_lock;
    
    class dpactor_state
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        dpactor_state( dpactor *a );
        //dtor
        virtual ~dpactor_state( void );
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_state **next_state );
        
    };
    
};

#endif