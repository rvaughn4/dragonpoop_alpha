
#ifndef dragonpoop_dpmutex_lock_h
#define dragonpoop_dpmutex_lock_h

namespace dragonpoop
{

    class dpmutex;
    class dpmutex_readlock;
    class dpmutex_writelock;

    class dpmutex_lock
    {

    private:

        dpmutex *m;

    protected:

        //ctor
        dpmutex_lock( dpmutex *m );
        //dtor
        virtual ~dpmutex_lock( void );

    public:

        //returns mutex parent
        dpmutex *getMutex( void );
        //returns true if read lock
        virtual bool isReadLock( void );
        //returns true if write lock
        virtual bool isWriteLock( void );
        //destroy/unlock mutex lock
        static void unlock( dpmutex_lock **l );
        //destroy/unlock mutex lock
        static void unlock( dpmutex_readlock **l );
        //destroy/unlock mutex lock
        static void unlock( dpmutex_writelock **l );

        friend class dpmutex_master;
    };
    
};

#endif