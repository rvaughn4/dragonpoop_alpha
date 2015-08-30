
#ifndef dragonpoop_model_vector_h
#define dragonpoop_model_vector_h

#include "../../dpvertex/dpxyz_f.h"

namespace dragonpoop
{
    
    struct model_vector_f
    {
        union
        {
            float fv[ 3 ];
            struct
            {
                float x, y, z;
            } sv;
        };
    };
    
    class model_matrix;
    class model_quaternion;
    
    class model_vector
    {
        
    private:
        
        model_vector_f f;
        
    protected:
        
    public:
        
        //ctor
        model_vector( void );
        //dtor
        virtual ~model_vector( void );
        //get raw data
        model_vector_f *getData( void );
        //set identity
        void setIdentity( void );
        //set position
        void setPosition( float x, float y, float z );
        //set position
        void setPosition( dpxyz_f *p );
        //get position
        void getPosition( dpxyz_f *p );
        
    };
    
};

#endif