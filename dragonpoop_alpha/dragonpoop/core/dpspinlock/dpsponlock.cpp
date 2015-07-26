
#include "dpsponlock.h"
#include <thread>

namespace dragonpoop
{

    //ctor
    dpspinlock::dpspinlock( void )
    {
        this->unlock();
    }

    //dtor
    dpspinlock::~dpspinlock( void )
    {
    }

    //locks
    void dpspinlock::lock( void )
    {
        unsigned int t;
        bool b;

        t = 0;
        b = 1;
        while( b )
        {
            __asm volatile ("pause" ::: "memory");
            t++;
            if( t > 20 )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
                t = 0;
            }
            b = this->f.test_and_set( std::memory_order_acquire );
        }
    }

    //unlocks
    void dpspinlock::unlock( void )
    {
        this->f.clear( std::memory_order_release );
    }

};
