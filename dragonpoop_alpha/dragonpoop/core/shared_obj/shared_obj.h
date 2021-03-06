
#ifndef dragonpoop_shared_obj_h
#define dragonpoop_shared_obj_h

#include "shared_obj_interface.h"
#include "shared_obj_refkernal.h"
#include <list>
#include <memory>

namespace dragonpoop
{

    class dpmutex_master;
    class dpmutex;

    class shared_obj : public shared_obj_interface
    {

    private:

        dpmutex_master *mm;
        dpmutex *l;
        std::list<std::shared_ptr<shared_obj_refkernal>> refs;

        //generate refkernal
        bool makeRef( std::shared_ptr<shared_obj_refkernal> *k );
        //remove dead refs
        void removeDeadRefs( void );

    protected:

        //ctor
        shared_obj( dpmutex_master *mm );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //return a new ref
        virtual shared_obj_ref *getRef( void );
        //blocking read lock
        virtual shared_obj_readlock *readLock( const char *c_lock_location );
        //blocking write lock
        virtual shared_obj_writelock *writeLock( const char *c_lock_location );
        //read lock with timeout
        virtual shared_obj_readlock *tryReadLock( const char *c_lock_location, uint64_t ms );
        //write lock with timeout
        virtual shared_obj_writelock *tryWriteLock( const char *c_lock_location, uint64_t ms );
        //attempt read lock
        virtual shared_obj_readlock *tryReadLock( const char *c_lock_location );
        //attempt write lock
        virtual shared_obj_writelock *tryWriteLock( const char *c_lock_location );

    public:

        //dtor
        virtual ~shared_obj( void );
        //returns true if linked
        virtual bool isLinked( void );
        //unlink ref
        virtual void unlink( void );

    };

};

#endif
