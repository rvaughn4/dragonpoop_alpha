
#ifndef dragonpoop_dpmatrix_h
#define dragonpoop_dpmatrix_h

#include "../dpvertex/dpvertexes.h"

namespace dragonpoop
{

    class model_instance_joint_cache;
    struct model_instance_joint_cache_element;
    class dpquaternion;
    
    struct dpmatrix_f
    {
        union
        {
            float fv[ 16 ];
            float f4v[ 4 ][ 4 ];
            struct
            {
                struct
                {
                    float r1, r2, r3, r4;
                } c1, c2, c3, c4;
            };
        };
    };
    
    class dpmatrix
    {

    private:

        dpmatrix_f values;

    protected:

    public:

        //ctor
        dpmatrix( void );
        //dtor
        ~dpmatrix( void );
        //copy
        void copy( dpmatrix *m );
        //load identity
        void setIdentity( void );
        //load raw matrix from pointer
        void loadRaw4by4( float *fv );
        //get raw matrix pointer
        float *getRaw4by4( void );
        //multiply matrix
        void multiply( dpmatrix *m );
        //multiply translation to matrix
        void translate( float x, float y, float z );
        //multiply rotation to matrix
        void rotateX( float deg );
        //multiply rotation to matrix
        void rotateY( float deg );
        //multiply rotation to matrix
        void rotateZ( float deg );
        //multiply rotation to matrix
        void rotateXrad( float rad );
        //multiply rotation to matrix
        void rotateYrad( float rad );
        //multiply rotation to matrix
        void rotateZrad( float rad );
        //multiply scaling to matrix
        void scale( float x, float y, float z );
        //set ortho matrix
        void setOrtho( float x0, float y0, float z0, float x1, float y1, float z1 );
        //set perspective matrix
        void setPerspective( float x0, float y0, float z0, float x1, float y1, float z1, float fov );
        //set tranlation matrix
        void setTranslation( float x, float y, float z );
        //set rotation matrix
        void setRotationX( float deg );
        //set rotation matrix
        void setRotationY( float deg );
        //set rotation matrix
        void setRotationZ( float deg );
        //set rotation matrix
        void setRotationXrad( float rad );
        //set rotation matrix
        void setRotationYrad( float rad );
        //set rotation matrix
        void setRotationZrad( float rad );
        //set scaling matrix
        void setScaling( float x, float y, float z );
        //transform
        void transform( float *px, float *py, float *pz, float *pw );
        //transform
        void transform( dpxyzw *p );
        //transform
        void transform( dpxyzw_f *p );
        //transform
        void transform( dpxyz_f *p );
        //inverse transform
        void itransform( float *px, float *py, float *pz, float *pw );
        //inverse transform
        void itransform( dpxyzw *p );
        //inverse transform
        void itransform( dpxyzw_f *p );
        //inverse transform
        void itransform( dpxyz_f *p );
        //rotate ( Z * Y ) * X
        void rotateRad( float x, float y, float z );
        //rotate ( Z * Y ) * X
        void rotate( float x, float y, float z );
        //set angle (radians) and position
        void setAngleRadAndPosition( dpxyz_f *angles, dpxyz_f *pos );
        //set this to inverse of matrix
        void inverse( dpmatrix *m );
        //returns position
        void getPosition( dpxyz_f *p );        
        //sets position
        void setPosition( dpxyz_f *p );
        //create matrix from quaternion
        void setQuat( dpquaternion *q );
        //find angles
        void getAngles( dpxyz_f *o );

    };

};

#endif