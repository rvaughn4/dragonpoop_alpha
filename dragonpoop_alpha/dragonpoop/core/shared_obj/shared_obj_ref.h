
#ifndef dragonpoop_shared_obj_ref_h
#define dragonpoop_shared_obj_ref_h

#include "shared_obj_interface.h"
#include "shared_obj_refkernal.h"
#include <memory>

namespace dragonpoop
{

    class shared_obj;

    class shared_obj_ref : public shared_obj_interface
    {

    private:

        std::shared_ptr<shared_obj_refkernal> k;
        shared_obj_interface *p;

    protected:

        //ctor
        shared_obj_ref( shared_obj_interface *p, std::shared_ptr<shared_obj_refkernal> *k );
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
        virtual ~shared_obj_ref( void );
        //returns true if linked
        virtual bool isLinked( void );
        //unlink ref
        virtual void unlink( void );

        friend class shared_obj;
    };
    
};

#endif