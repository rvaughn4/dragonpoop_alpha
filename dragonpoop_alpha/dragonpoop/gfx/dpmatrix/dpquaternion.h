
#ifndef dragonpoop_dpquaternion_h
#define dragonpoop_dpquaternion_h

namespace dragonpoop
{
    
    struct dpquaternion_f
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
    
    struct dpxyz_f;
    
    class dpquaternion
    {
        
    private:
        
        dpquaternion_f f;
        
    protected:
        
    public:
        
        //ctor
        dpquaternion( void );
        //dtor
        virtual ~dpquaternion( void );
        //get raw data
        dpquaternion_f *getData( void );
        //set identity
        void setIdentity( void );
        //set position
        void setPosition( float x, float y, float z, float w );
        //set angle
        void setAngle( float x, float y, float z );
        //set angle
        void setAngle( dpxyz_f *x );
        //get angle
        void getAngle( dpxyz_f *x );
        //slerp
        void slerp( dpquaternion *a, dpquaternion *b, float r );
        
    };
    
};

#endif