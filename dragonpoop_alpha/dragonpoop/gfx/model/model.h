
#ifndef dragonpoop_model_h
#define dragonpoop_model_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{

    class dptaskpool_writelock;
    class dptask;
    class model_task;
    class dpthread_lock;
    class core;
    class model_writelock;

    class model : public shared_obj
    {

    private:

        dptask *tsk;
        model_task *gtsk;
        core *c;
        std::string sname, scmmt;
        dpid id;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //stop model task
        void kill( void );
        //run model from task
        void run( dpthread_lock *thd, model_writelock *g );
        //set name
        void setName( std::string *sname );
        //get name
        void getName( std::string *sname );
        //compare name
        bool compareName( std::string *sname );
        //set id
        void setId( dpid id );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //set comment
        void setComment( std::string *s );
        //get comment
        void getComment( std::string *s );
        //append to comment
        void appendComment( std::string *s );

    public:

        //ctor
        model( core *c, dptaskpool_writelock *tp, dpid id );
        //dtor
        virtual ~model( void );
        //return core
        core *getCore( void );

        friend class model_readlock;
        friend class model_writelock;
    };
    
};

#endif