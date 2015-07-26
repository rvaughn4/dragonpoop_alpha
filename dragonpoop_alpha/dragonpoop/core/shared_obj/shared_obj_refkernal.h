
#ifndef dragonpoop_shared_obj_refkernal_h
#define dragonpoop_shared_obj_refkernal_h

#include <atomic>

namespace dragonpoop
{

    class dpspinlock;

    class shared_obj_refkernal
    {

    private:

        std::atomic<bool> bIsOrigLinked, bIsRefLinked;
        dpspinlock *slk;

    protected:

    public:

        //ctor
        shared_obj_refkernal( void );
        //dtor
        virtual ~shared_obj_refkernal( void );
        //returns true if linked
        bool isLinked( void );
        //unlink original
        void unlinkOriginal( void );
        //unlink ref
        void unlinkRef( void );
        //lock
        void lock( void );
        //unlock
        void unlock( void );

    };
    
};

#endif