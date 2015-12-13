
#include "dptaskpool_logger.h"
#include "../shared_obj/shared_obj.h"
#include "../shared_obj/shared_obj_guard.h"
#include "dptaskpool_logger_ref.h"
#include "dptaskpool_logger_readlock.h"
#include "dptaskpool_logger_writelock.h"
#include "../dptask/dptask_readlock.h"
#include "../dptask/dptask_ref.h"
#include "../dptask/dptask.h"
#include <string.h>
#include <string>

namespace dragonpoop
{

    //ctor
    dptaskpool_logger::dptaskpool_logger( dpmutex_master *mm ) : shared_obj( mm )
    {
        std::shared_ptr<std::atomic<uint64_t>> tt( new std::atomic<uint64_t>() );
        this->tm = tt;
        this->lastTask = 0;
    }

    //dtor
    dptaskpool_logger::~dptaskpool_logger( void )
    {

    }

    //generate write lock
    shared_obj_writelock *dptaskpool_logger::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dptaskpool_logger_writelock( (dptaskpool_logger *)p, l );
    }

    //generate read lock
    shared_obj_readlock *dptaskpool_logger::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dptaskpool_logger_readlock( (dptaskpool_logger *)p, l );
    }

    //generate ref
    shared_obj_ref *dptaskpool_logger::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dptaskpool_logger_ref( (dptaskpool_logger *)p, k, this->tm );
    }

    //clear
    void dptaskpool_logger::clear( void )
    {
        this->lastTask = 0;
    }

    //sync
    void dptaskpool_logger::sync( dptaskpool_logger_ref *r )
    {
        dptaskpool_logger_readlock *l;
        shared_obj_guard o;
        dptaskpool_logger_value *t;
        unsigned int cnt;

        if( r->getTime() == this->getTime() )
            return;

        l = (dptaskpool_logger_readlock *)o.tryReadLock( r, 100, "dptaskpool_logger::sync" );
        if( !l )
            return;

        cnt = l->getTaskList( &t );
        if( !cnt )
            return;

        this->lastTask = cnt;
        cnt *= sizeof(dptaskpool_logger_value);
        if( cnt >= sizeof( this->values ) )
            cnt = sizeof( this->values );

        memcpy( &this->values, t, cnt );
        this->setTime( l->getTime() );
    }

    //add task
    void dptaskpool_logger::addTask( unsigned int tid, dptask *t )
    {
        shared_obj_guard o;
        dptask_readlock *l;

        l = (dptask_readlock *)o.tryReadLock( t, 100, "dptaskpool_logger::addTask" );
        if( !l )
            return;

        this->addTask( tid, l );
    }

    //add task
    void dptaskpool_logger::addTask( unsigned int tid, dptask_ref *t )
    {
        shared_obj_guard o;
        dptask_readlock *l;

        l = (dptask_readlock *)o.tryReadLock( t, 100, "dptaskpool_logger::addTask" );
        if( !l )
            return;

        this->addTask( tid, l );
    }

    //add task
    void dptaskpool_logger::addTask( unsigned int tid, dptask_readlock *t )
    {
        dptaskpool_logger_value *v;
        std::string s;

        if( this->lastTask >= dptaskpool_logger_value_max )
            return;

        v = &this->values[ this->lastTask ];

        t->getName( &s );

        memset( v, 0, sizeof(dptaskpool_logger_value) );
        v->tid = tid;
        s.copy( v->name, sizeof( v->name ) );
        v->percent_usage = t->getUsage();
        v->isStatic = t->isSingleThread();

        this->lastTask++;
    }

    //get task list
    unsigned int dptaskpool_logger::getTaskList( dptaskpool_logger_value **p )
    {
        if( this->lastTask > dptaskpool_logger_value_max )
            this->lastTask = dptaskpool_logger_value_max;
        *p = this->values;
        return this->lastTask;
    }

    //get time
    uint64_t dptaskpool_logger::getTime( void )
    {
        return *( this->tm.get() );
    }

    //set time
    void dptaskpool_logger::setTime( uint64_t t )
    {
        *( this->tm.get() ) = t;
    }

};
