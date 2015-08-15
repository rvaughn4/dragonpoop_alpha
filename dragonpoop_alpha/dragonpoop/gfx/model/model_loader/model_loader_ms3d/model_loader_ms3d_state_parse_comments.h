
#ifndef dragonpoop_model_loader_ms3d_state_parse_comments_h
#define dragonpoop_model_loader_ms3d_state_parse_comments_h

#include "../model_loader_state.h"
#include <string>

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    
    class model_loader_ms3d_state_parse_comments : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        
    protected:
        
        //read comment into string
        bool readComment( dpbuffer *b, std::string *s );
        //read group comments
        bool readGroupComments( dpbuffer *b, model_loader_writelock *ml );
        //read material comments
        bool readMaterialComments( dpbuffer *b, model_loader_writelock *ml );
        //read joint comments
        bool readJointComments( dpbuffer *b, model_loader_writelock *ml );
        //read model comments
        bool readModelComments( dpbuffer *b, model_loader_writelock *ml );

    public:
        
        //ctor
        model_loader_ms3d_state_parse_comments( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_parse_comments( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif