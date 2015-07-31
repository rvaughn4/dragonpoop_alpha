
#ifndef dragonpoop_model_h
#define dragonpoop_model_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include "model_component/model_components.h"

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
        struct
        {
            std::list<model_component *> lst;
            model_component_tree_bytype bytype;
            model_component_tree_bytypeid bytypeid;
            model_component_tree_bytypeid bytypeowner;
        } comps;
        
        //delete all components
        void deleteComponents( void );
        
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
        //add component to list and trees
        void addComponent( model_component *c );
        //add component, 1 parent
        void addComponent( model_component *c, dpid p1 );
        //add component, 2 parents
        void addComponent( model_component *c, dpid p1, dpid p2 );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //remove component
        void removeComponent( model_component *c );

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