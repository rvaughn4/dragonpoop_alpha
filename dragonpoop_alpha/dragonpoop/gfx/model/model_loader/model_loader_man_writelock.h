
#ifndef dragonpoop_model_loader_man_writelock_h
#define dragonpoop_model_loader_man_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    
    class model_loader_man;
    class core;
    class dpthread_lock;
    class model_loader_ref;
    class model_ref;
    class model_saver_ref;
    
    class model_loader_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_loader_man *t;
        
    protected:
        
        //ctor
        model_loader_man_writelock( model_loader_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_loader_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
        void run( dpthread_lock *thd );
        //load file into model
        bool load( model_ref *m, const char *path_name, const char *file_name, model_loader_ref **mldr );
        //save model into file
        bool save( model_ref *m, const char *path_name, const char *file_name, model_saver_ref **mldr );

        friend class model_loader_man;
    };
    
};

#endif