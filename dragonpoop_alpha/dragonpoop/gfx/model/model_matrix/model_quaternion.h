
#ifndef dragonpoop_model_quaternion_h
#define dragonpoop_model_quaternion_h

namespace dragonpoop
{
    
    struct model_quaternion_f
    {
        union
        {
            float fv[ 4 ];
            struct
            {
                float x, y, z, w;
            } sv;
        };
    };
    
    class model_matrix;
    class model_vector;
    struct dpxyz_f;
    
    class model_quaternion
    {
        
    private:
        
        model_quaternion_f f;
        
    protected:
        
    public:
        
        //ctor
        model_quaternion( void );
        //dtor
        virtual ~model_quaternion( void );
        //get raw data
        model_quaternion_f *getData( void );
        //set identity
        void setIdentity( void );
        //set position
        void setPosition( float x, float y, float z, float w );
        //set angle
        void setAngle( float x, float y, float z );
        //set angle
        void setAngle( dpxyz_f *x );
        
    };
    
};

#endif