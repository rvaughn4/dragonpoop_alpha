
#include "model.h"
#include "model_task.h"
#include "model_readlock.h"
#include "model_writelock.h"
#include "model_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/core.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"

namespace dragonpoop
{

    //ctor
    model::model( core *c, dptaskpool_writelock *tp, dpid id ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->id = id;
        this->gtsk = new model_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 500, 0 );
        tp->addTask( this->tsk );
    }

    //dtor
    model::~model( void )
    {
        this->kill();
        delete this->gtsk;
    }

    //return core
    core *model::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *model::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_readlock( (model *)p, l );
    }

    //generate write lock
    shared_obj_writelock *model::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_writelock( (model *)p, l );
    }

    //generate ref
    shared_obj_ref *model::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_ref( (model *)p, k );
    }

    //stop model task
    void model::kill( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk );
        tl->kill();
        o.unlock();

        delete this->tsk;
        this->tsk = 0;
    }

    //run model from task
    void model::run( dpthread_lock *thd, model_writelock *g )
    {
    }

    //set name
    void model::setName( std::string *sname )
    {
        this->sname.assign( *sname );
    }

    //get name
    void model::getName( std::string *sname )
    {
        sname->assign( this->sname );
    }

    //compare name
    bool model::compareName( std::string *sname )
    {
        return this->sname.compare( *sname ) == 0;
    }

    //set id
    void model::setId( dpid id )
    {
        dpid_copy( &this->id, &id );
    }

    //get id
    dpid model::getId( void )
    {
        return this->id;
    }

    //compare id
    bool model::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }

    //set comment
    void model::setComment( std::string *s )
    {
        this->scmmt.assign( *s );
    }

    //get comment
    void model::getComment( std::string *s )
    {
        s->assign( this->scmmt );
    }

    //append to comment
    void model::appendComment( std::string *s )
    {
        this->scmmt.append( *s );
    }

    //add component to list and trees
    void model::addComponent( model_component *c )
    {
        this->comps.lst.push_back( c );
        this->comps.bytype.addLeaf( c->getType(), c );
        this->comps.bytypeid.addLeaf( c->getType(), c->getId(), c );
    }
    
    //add component, 1 parent
    void model::addComponent( model_component *c, dpid p1 )
    {
        this->addComponent( c );
        this->comps.bytypeowner.addLeaf( c->getType(), p1, c );
    }
    
    //add component, 2 parents
    void model::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->addComponent( c, p1 );
        this->comps.bytypeowner.addLeaf( c->getType(), p2, c );
    }
    
    //find component by type and id
    model_component *model::findComponent( uint16_t mtype, dpid id )
    {
        return this->comps.bytypeid.findLeaf( mtype, id );
    }
    
    //find components by type
    void model::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( mtype, l );
    }
    
    //find components by type and 1 parent
    void model::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->comps.bytypeowner.findLeaves( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        std::list<model_component *> l1, l2;
        std::list<model_component *>::iterator i1, i2;
        model_component *c1, *c2;
        
        this->getComponentsByParent( mtype, p1, &l1 );
        this->getComponentsByParent( mtype, p2, &l2 );
        
        for( i1 = l1.begin(); i1 != l1.end(); ++i1 )
        {
            c1 = *i1;
            c2 = 0;
            
            for( i2 = l2.begin(); i2 != l2.end() && c2 != c1; ++i2 )
            {
                c2 = *i2;
                
                if( c2 == c1 )
                    l->push_back( c2 );
            }
        }
    }
    
    //remove component
    void model::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
        this->comps.bytype.removeLeaf( c );
        this->comps.bytypeid.removeLeaf( c );
        this->comps.bytypeowner.removeLeaf( c );
    }
    
};
