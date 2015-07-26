
#ifndef dragonpoop_core_h
#define dragonpoop_core_h

#include <atomic>

namespace dragonpoop
{

    class dpmutex_master;
    class dptaskpool;
    class dptaskpool_ref;
    class dpmutex;
    class gfx;
    class gfx_ref;
    class dptask_ref;
    class dptask;

    class core
    {

    private:

        std::atomic<bool> bIsRun;
        dpmutex_master *mm;
        dptaskpool *tp;
        gfx *g;

    protected:

    public:

        //ctor
        core( void );
        //dtor
        virtual ~core( void );
        //returns true if running
        bool isRunning( void );
        //kills core
        void kill( void );
        //returns mutex master
        dpmutex_master *getMutexMaster( void );
        //returns taskpool lock
        dptaskpool_ref *getTaskpool( void );
        //returns gfx ref
        gfx_ref *getGfx( void );
        //add task, will create ref so caller retains original
        void addTask( dptask *t );
        //add task, will create ref so caller retains original
        void addTask( dptask_ref *t );

    };

};

#endif