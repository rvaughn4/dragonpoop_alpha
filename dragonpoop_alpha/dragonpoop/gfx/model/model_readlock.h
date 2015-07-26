
#ifndef dragonpoop_model_readlock_h
#define dragonpoop_model_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{
    class model;
    class core;

    class model_readlock : public shared_obj_readlock
    {

    private:

        model *t;

    protected:

        //ctor
        model_readlock( model *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_readlock( void );

    public:

        //return core
        core *getCore( void );
        //get name
        void getName( std::string *sname );
        //compare name
        bool compareName( std::string *sname );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //get comment
        void getComment( std::string *s );

        friend class model;
    };
    
};

#endif