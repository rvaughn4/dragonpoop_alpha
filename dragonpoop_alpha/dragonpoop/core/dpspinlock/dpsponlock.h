
#ifndef dragonpoop_dpspinlock_h
#define dragonpoop_dpspinlock_h

#include <atomic>

namespace dragonpoop
{

    class dpspinlock
    {

    private:

        std::atomic_flag f = ATOMIC_FLAG_INIT;

    protected:

    public:

        //ctor
        dpspinlock( void );
        //dtor
        virtual ~dpspinlock( void );
        //returns true if locked
        bool isLocked( void );
        //lock blocking
        void lock( void );
        //unlocks
        void unlock( void );

    };
    
};

#endif