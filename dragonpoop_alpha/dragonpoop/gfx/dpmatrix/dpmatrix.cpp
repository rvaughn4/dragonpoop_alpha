
#include "dpmatrix.h"
#include "dpquaternion.h"
#include <math.h>

namespace dragonpoop
{

    //ctor
    dpmatrix::dpmatrix( void )
    {
        this->setIdentity();
    }

    //dtor
    dpmatrix::~dpmatrix( void )
    {

    }

    //copy
    void dpmatrix::copy( dpmatrix *m )
    {
        unsigned int i;
        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = m->values.fv[ i ];
    }

    //load identity
    void dpmatrix::setIdentity( void )
    {
        this->setScaling( 1, 1, 1 );
    }

    //load raw matrix from pointer
    void dpmatrix::loadRaw4by4( float *fv )
    {
        unsigned int i;
        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = fv[ i ];
    }

    //get raw matrix pointer
    float *dpmatrix::getRaw4by4( void )
    {
        return this->values.fv;
    }

    //multiply matrix
    void dpmatrix::multiply( dpmatrix *m )
    {
        dpmatrix r;

        r.values.c1.r1 = this->values.c1.r1 * m->values.c1.r1 + this->values.c2.r1 * m->values.c1.r2 + this->values.c3.r1 * m->values.c1.r3 + this->values.c4.r1 * m->values.c1.r4;

        r.values.c2.r1 = this->values.c1.r1 * m->values.c2.r1 + this->values.c2.r1 * m->values.c2.r2 + this->values.c3.r1 * m->values.c2.r3 + this->values.c4.r1 * m->values.c2.r4;

        r.values.c3.r1 = this->values.c1.r1 * m->values.c3.r1 + this->values.c2.r1 * m->values.c3.r2 + this->values.c3.r1 * m->values.c3.r3 + this->values.c4.r1 * m->values.c3.r4;

        r.values.c4.r1 = this->values.c1.r1 * m->values.c4.r1 + this->values.c2.r1 * m->values.c4.r2 + this->values.c3.r1 * m->values.c4.r3 + this->values.c4.r1 * m->values.c4.r4;

        r.values.c1.r2 = this->values.c1.r2 * m->values.c1.r1 + this->values.c2.r2 * m->values.c1.r2 + this->values.c3.r2 * m->values.c1.r3 + this->values.c4.r2 * m->values.c1.r4;

        r.values.c2.r2 = this->values.c1.r2 * m->values.c2.r1 + this->values.c2.r2 * m->values.c2.r2 + this->values.c3.r2 * m->values.c2.r3 + this->values.c4.r2 * m->values.c2.r4;

        r.values.c3.r2 = this->values.c1.r2 * m->values.c3.r1 + this->values.c2.r2 * m->values.c3.r2 + this->values.c3.r2 * m->values.c3.r3 + this->values.c4.r2 * m->values.c3.r4;

        r.values.c4.r2 = this->values.c1.r2 * m->values.c4.r1 + this->values.c2.r2 * m->values.c4.r2 + this->values.c3.r2 * m->values.c4.r3 + this->values.c4.r2 * m->values.c4.r4;

        r.values.c1.r3 = this->values.c1.r3 * m->values.c1.r1 + this->values.c2.r3 * m->values.c1.r2 + this->values.c3.r3 * m->values.c1.r3 + this->values.c4.r3 * m->values.c1.r4;

        r.values.c2.r3 = this->values.c1.r3 * m->values.c2.r1 + this->values.c2.r3 * m->values.c2.r2 + this->values.c3.r3 * m->values.c2.r3 + this->values.c4.r3 * m->values.c2.r4;

        r.values.c3.r3 = this->values.c1.r3 * m->values.c3.r1 + this->values.c2.r3 * m->values.c3.r2 + this->values.c3.r3 * m->values.c3.r3 + this->values.c4.r3 * m->values.c3.r4;

        r.values.c4.r3 = this->values.c1.r3 * m->values.c4.r1 + this->values.c2.r3 * m->values.c4.r2 + this->values.c3.r3 * m->values.c4.r3 + this->values.c4.r3 * m->values.c4.r4;

        r.values.c1.r4 = this->values.c1.r4 * m->values.c1.r1 + this->values.c2.r4 * m->values.c1.r2 + this->values.c3.r4 * m->values.c1.r3 + this->values.c4.r4 * m->values.c1.r4;

        r.values.c2.r4 = this->values.c1.r4 * m->values.c2.r1 + this->values.c2.r4 * m->values.c2.r2 + this->values.c3.r4 * m->values.c2.r3 + this->values.c4.r4 * m->values.c2.r4;

        r.values.c3.r4 = this->values.c1.r4 * m->values.c3.r1 + this->values.c2.r4 * m->values.c3.r2 + this->values.c3.r4 * m->values.c3.r3 + this->values.c4.r4 * m->values.c3.r4;

        r.values.c4.r4 = this->values.c1.r4 * m->values.c4.r1 + this->values.c2.r4 * m->values.c4.r2 + this->values.c3.r4 * m->values.c4.r3 + this->values.c4.r4 * m->values.c4.r4;

        this->loadRaw4by4( r.values.fv );
    }

    //multiply translation to matrix
    void dpmatrix::translate( float x, float y, float z )
    {
        dpmatrix m;
        m.setTranslation( x, y, z );
        this->multiply( &m );
    }

    //multiply rotation to matrix
    void dpmatrix::rotateX( float deg )
    {
        dpmatrix m;
        m.setRotationX( deg );
        this->multiply( &m );
    }

    //multiply rotation to matrix
    void dpmatrix::rotateY( float deg )
    {
        dpmatrix m;
        m.setRotationY( deg );
        this->multiply( &m );
    }

    //multiply rotation to matrix
    void dpmatrix::rotateZ( float deg )
    {
        dpmatrix m;
        m.setRotationZ( deg );
        this->multiply( &m );
    }
    
    //multiply rotation to matrix
    void dpmatrix::rotateXrad( float rad )
    {
        dpmatrix m;
        m.setRotationXrad( rad );
        this->multiply( &m );
    }
    
    //multiply rotation to matrix
    void dpmatrix::rotateYrad( float rad )
    {
        dpmatrix m;
        m.setRotationYrad( rad );
        this->multiply( &m );
    }
    
    //multiply rotation to matrix
    void dpmatrix::rotateZrad( float rad )
    {
        dpmatrix m;
        m.setRotationZrad( rad );
        this->multiply( &m );
    }

    //multiply scaling to matrix
    void dpmatrix::scale( float x, float y, float z )
    {
        dpmatrix m;
        m.setScaling( x, y, z );
        this->multiply( &m );
    }

    //set ortho matrix
    void dpmatrix::setOrtho( float x0, float y0, float z0, float x1, float y1, float z1 )
    {
        unsigned int i;
        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = 0;

        this->values.c1.r1 = 2.0f / ( x1 - x0 );
        this->values.c2.r2 = 2.0f / ( y1 - y0 );
        this->values.c3.r3 = -2.0f / ( z1 - z0 );
        this->values.c4.r1 = -( x1 + x0 ) / ( x1 - x0 );
        this->values.c4.r2 = -( y1 + y0 ) / ( y1 - y0 );
        this->values.c4.r3 = -( z1 + z0 ) / ( z1 - z0 );
        this->values.c4.r4 = 1.0f;
    }

    //set perspective matrix
    void dpmatrix::setPerspective( float x0, float y0, float z0, float x1, float y1, float z1, float fov )
    {
        unsigned int i;
        float aspect;

        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = 0;

        fov = fov * 3.14f / 180.0f;
        fov = 1.0f / tan( fov * 0.5f );
        aspect = fabs( ( x1 - x0 ) / ( y1 - y0 ) );

        this->values.c1.r1 = fov / aspect;
        this->values.c2.r2 = fov;
        this->values.c3.r3 = ( z1 + z0 ) / ( z0 - z1 );
        this->values.c4.r3 = ( 2.0f * z1 * z0 ) / ( z0 - z1 );
        this->values.c3.r4 = -1.0f;
    }

    //set tranlation matrix
    void dpmatrix::setTranslation( float x, float y, float z )
    {
        unsigned int i;
        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = 0;
        this->values.c4.r1 = x;
        this->values.c4.r2 = y;
        this->values.c4.r3 = z;
        this->values.c1.r1 = 1;
        this->values.c2.r2 = 1;
        this->values.c3.r3 = 1;
        this->values.c4.r4 = 1;
    }

    //set rotation matrix
    void dpmatrix::setRotationX( float deg )
    {
        this->setIdentity();
        deg = deg * 3.14f / 180.0f;
        this->values.c2.r2 = cos( deg );
        this->values.c3.r2 = -sin( deg );
        this->values.c2.r3 = sin( deg );
        this->values.c3.r3 = cos( deg );
    }

    //set rotation matrix
    void dpmatrix::setRotationY( float deg )
    {
        this->setIdentity();
        deg = deg * 3.14f / 180.0f;
        this->values.c1.r1 = cos( deg );
        this->values.c3.r1 = sin( deg );
        this->values.c1.r3 = -sin( deg );
        this->values.c3.r3 = cos( deg );
    }

    //set rotation matrix
    void dpmatrix::setRotationZ( float deg )
    {
        this->setIdentity();
        deg = deg * 3.14f / 180.0f;
        this->values.c1.r1 = cos( deg );
        this->values.c2.r1 = -sin( deg );
        this->values.c1.r2 = sin( deg );
        this->values.c2.r2 = cos( deg );
    }
    
    //set rotation matrix
    void dpmatrix::setRotationXrad( float deg )
    {
        this->setIdentity();
        this->values.c2.r2 = cos( deg );
        this->values.c3.r2 = -sin( deg );
        this->values.c2.r3 = sin( deg );
        this->values.c3.r3 = cos( deg );
    }
    
    //set rotation matrix
    void dpmatrix::setRotationYrad( float deg )
    {
        this->setIdentity();
        this->values.c1.r1 = cos( deg );
        this->values.c3.r1 = sin( deg );
        this->values.c1.r3 = -sin( deg );
        this->values.c3.r3 = cos( deg );
    }
    
    //set rotation matrix
    void dpmatrix::setRotationZrad( float deg )
    {
        this->setIdentity();
        this->values.c1.r1 = cos( deg );
        this->values.c2.r1 = -sin( deg );
        this->values.c1.r2 = sin( deg );
        this->values.c2.r2 = cos( deg );
    }

    //set scaling matrix
    void dpmatrix::setScaling( float x, float y, float z )
    {
        unsigned int i;
        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = 0;
        this->values.c1.r1 = x;
        this->values.c2.r2 = y;
        this->values.c3.r3 = z;
        this->values.c4.r4 = 1;
    }

    //transform
    void dpmatrix::transform( float *px, float *py, float *pz, float *pw )
    {
        float x, y, z, w;
        float rx, ry, rz, rw;

        x = y = z = 0;
        w = 1.0f;

        if( px )
            x = *px;
        if( py )
            y = *py;
        if( pz )
            z = *pz;
        if( pw )
            w = *pw;

        rx = this->values.c1.r1 * x + this->values.c2.r1 * y + this->values.c3.r1 * z + this->values.c4.r1 * w;
        ry = this->values.c1.r2 * x + this->values.c2.r2 * y + this->values.c3.r2 * z + this->values.c4.r2 * w;
        rz = this->values.c1.r3 * x + this->values.c2.r3 * y + this->values.c3.r3 * z + this->values.c4.r3 * w;
        rw = this->values.c1.r4 * x + this->values.c2.r4 * y + this->values.c3.r4 * z + this->values.c4.r4 * w;
        
        if( px )
            *px = rx;
        if( py )
            *py = ry;
        if( pz )
            *pz = rz;
        if( pw )
            *pw = rw;
    }

    //transform
    void dpmatrix::transform( dpxyzw *p )
    {
        this->transform( &p->x, &p->y, &p->z, &p->w );
    }

    //transform
    void dpmatrix::transform( dpxyzw_f *p )
    {
        this->transform( &p->x, &p->y, &p->z, &p->w );
    }

    //transform
    void dpmatrix::transform( dpxyz_f *p )
    {
        this->transform( &p->x, &p->y, &p->z, 0 );
    }

    //inverse transform
    void dpmatrix::itransform( float *px, float *py, float *pz, float *pw )
    {
        float x, y, z, w;
        float rx, ry, rz, rw;
        
        x = y = z = 0;
        w = 1.0f;
        
        if( px )
            x = *px;
        if( py )
            y = *py;
        if( pz )
            z = *pz;
        if( pw )
            w = *pw;
        
        x = x - this->values.c4.r1;
        y = y - this->values.c4.r2;
        z = z - this->values.c4.r3;
        
        rx = x * this->values.c1.r1 + y * this->values.c1.r2 + z * this->values.c1.r3;
        ry = y * this->values.c2.r1 + y * this->values.c2.r2 + z * this->values.c2.r3;
        rz = z * this->values.c3.r1 + y * this->values.c3.r2 + z * this->values.c3.r3;
        
        if( px )
            *px = rx;
        if( py )
            *py = ry;
        if( pz )
            *pz = rz;
        if( pw )
            *pw = rw;
    }
    
    //inverse transform
    void dpmatrix::itransform( dpxyzw *p )
    {
        this->itransform( &p->x, &p->y, &p->z, &p->w );
    }
    
    //inverse transform
    void dpmatrix::itransform( dpxyzw_f *p )
    {
        this->itransform( &p->x, &p->y, &p->z, &p->w );
    }
    
    //inverse transform
    void dpmatrix::itransform( dpxyz_f *p )
    {
        this->itransform( &p->x, &p->y, &p->z, 0 );
    }
    
    //rotate ( Z * Y ) * X
    void dpmatrix::rotateRad( float x, float y, float z )
    {
        dpmatrix zm, ym, xm;
        
        xm.setRotationXrad( x );
        ym.setRotationYrad( y );
        zm.setRotationZrad( z );
        
        zm.multiply( &ym );
        zm.multiply( &xm );
        
        this->multiply( &zm );
    }
    
    //rotate ( Z * Y ) * X
    void dpmatrix::rotate( float x, float y, float z )
    {
        dpmatrix zm, ym, xm;
        
        xm.setRotationX( x );
        ym.setRotationY( y );
        zm.setRotationZ( z );
        
        zm.multiply( &ym );
        zm.multiply( &xm );
        
        this->multiply( &zm );
    }

    //set angle (radians) and position
    void dpmatrix::setAngleRadAndPosition( dpxyz_f *angles, dpxyz_f *pos )
    {
        unsigned int i;
        float sr, sp, sy, cr, cp, cy;

        for( i = 0; i < 16; i++ )
            this->values.fv[ i ] = 0;
        
        sy = sin( angles->z );
        cy = cos( angles->z );
        
        sp = sin( angles->y );
        cp = cos( angles->y );
        
        sr = sin( angles->x );
        cr = cos( angles->x );
        
        this->values.c1.r1 = cp * cy;
        this->values.c1.r2 = cp * sy;
        this->values.c1.r3 = -sp;

        this->values.c2.r1 = sr * sp * cy + cr * -sy;
        this->values.c2.r2 = sr * sp * sy + cr * cy;
        this->values.c2.r3 = sr * cp;

        this->values.c3.r1 = ( cr * sp * cy + -sr * -sy );
        this->values.c3.r2 = ( cr * sp * sy + -sr * cy );
        this->values.c3.r3 = cr * cp;

        this->values.c4.r1 = pos->x;
        this->values.c4.r2 = pos->y;
        this->values.c4.r3 = pos->z;
        this->values.c4.r4 = 1;
    }
    
    //set this to inverse of matrix
    void dpmatrix::inverse( dpmatrix *m )
    {
        float s0, s1, s2, s3, s4, s5;
        float c0, c1, c2, c3, c4, c5;
        float det, invdet;
        
        s0 = m->values.c1.r1 * m->values.c2.r2 - m->values.c2.r1 * m->values.c1.r2;
        s1 = m->values.c1.r1 * m->values.c2.r3 - m->values.c2.r1 * m->values.c1.r3;
        s2 = m->values.c1.r1 * m->values.c2.r4 - m->values.c2.r1 * m->values.c1.r4;
        s3 = m->values.c1.r2 * m->values.c2.r3 - m->values.c2.r2 * m->values.c1.r3;
        s4 = m->values.c1.r2 * m->values.c2.r4 - m->values.c2.r2 * m->values.c1.r4;
        s5 = m->values.c1.r3 * m->values.c2.r4 - m->values.c2.r3 * m->values.c1.r4;
        
        c5 = m->values.c3.r3 * m->values.c4.r4 - m->values.c4.r3 * m->values.c3.r4;
        c4 = m->values.c3.r2 * m->values.c4.r4 - m->values.c4.r2 * m->values.c3.r4;
        c3 = m->values.c3.r2 * m->values.c4.r3 - m->values.c4.r2 * m->values.c3.r3;
        c2 = m->values.c3.r1 * m->values.c4.r4 - m->values.c4.r1 * m->values.c3.r4;
        c1 = m->values.c3.r1 * m->values.c4.r3 - m->values.c4.r1 * m->values.c3.r3;
        c0 = m->values.c3.r1 * m->values.c4.r2 - m->values.c4.r1 * m->values.c3.r2;
    
    // Should check for 0 determinant
        det = ( s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0 );
        if( det )
            invdet = 1.0 / det;
        else
            invdet = 0;
        
        this->values.c1.r1 = ( m->values.c2.r2 * c5 - m->values.c2.r3 * c4 + m->values.c2.r4 * c3) * invdet;
        this->values.c1.r2 = (-m->values.c1.r2 * c5 + m->values.c1.r3 * c4 - m->values.c1.r4 * c3) * invdet;
        this->values.c1.r3 = ( m->values.c4.r2 * s5 - m->values.c4.r3 * s4 + m->values.c4.r4 * s3) * invdet;
        this->values.c1.r4 = (-m->values.c3.r2 * s5 + m->values.c3.r3 * s4 - m->values.c3.r4 * s3) * invdet;
        
        this->values.c2.r1 = (-m->values.c2.r1 * c5 + m->values.c2.r3 * c2 - m->values.c2.r4 * c1) * invdet;
        this->values.c2.r2 = ( m->values.c1.r1 * c5 - m->values.c1.r3 * c2 + m->values.c1.r4 * c1) * invdet;
        this->values.c2.r3 = (-m->values.c4.r1 * s5 + m->values.c4.r3 * s2 - m->values.c4.r4 * s1) * invdet;
        this->values.c2.r4 = ( m->values.c3.r1 * s5 - m->values.c3.r3 * s2 + m->values.c3.r4 * s1) * invdet;
        
        this->values.c3.r1 = ( m->values.c2.r1 * c4 - m->values.c2.r2 * c2 + m->values.c2.r4 * c0) * invdet;
        this->values.c3.r2 = (-m->values.c1.r1 * c4 + m->values.c1.r2 * c2 - m->values.c1.r4 * c0) * invdet;
        this->values.c3.r3 = ( m->values.c4.r1 * s4 - m->values.c4.r2 * s2 + m->values.c4.r4 * s0) * invdet;
        this->values.c3.r4 = (-m->values.c3.r1 * s4 + m->values.c3.r2 * s2 - m->values.c3.r4 * s0) * invdet;
        
        this->values.c4.r1 = (-m->values.c2.r1 * c3 + m->values.c2.r2 * c1 - m->values.c2.r3 * c0) * invdet;
        this->values.c4.r2 = ( m->values.c1.r1 * c3 - m->values.c1.r2 * c1 + m->values.c1.r3 * c0) * invdet;
        this->values.c4.r3 = (-m->values.c4.r1 * s3 + m->values.c4.r2 * s1 - m->values.c4.r3 * s0) * invdet;
        this->values.c4.r4 = ( m->values.c3.r1 * s3 - m->values.c3.r2 * s1 + m->values.c3.r3 * s0) * invdet;
    
    }

    //returns position
    void dpmatrix::getPosition( dpxyz_f *p )
    {
        p->x = this->values.c4.r1;
        p->y = this->values.c4.r2;
        p->z = this->values.c4.r3;
    }

    //sets position
    void dpmatrix::setPosition( dpxyz_f *p )
    {
        this->values.c4.r1 = p->x;
        this->values.c4.r2 = p->y;
        this->values.c4.r3 = p->z;
    }
    
    
    //create matrix from quaternion
    void dpmatrix::setQuat( dpquaternion *q )
    {
        dpmatrix_f *matrix;
        dpquaternion_f *quaternion;
        
        matrix = &this->values;
        quaternion = q->getData();
        
        matrix->f4v[0][0] = 1.0 - 2.0 * quaternion->fv[1] * quaternion->fv[1] - 2.0 * quaternion->fv[2] * quaternion->fv[2];
        matrix->f4v[0][1] = 2.0 * quaternion->fv[0] * quaternion->fv[1] + 2.0 * quaternion->fv[3] * quaternion->fv[2];
        matrix->f4v[0][2] = 2.0 * quaternion->fv[0] * quaternion->fv[2] - 2.0 * quaternion->fv[3] * quaternion->fv[1];
        
        matrix->f4v[1][0] = 2.0 * quaternion->fv[0] * quaternion->fv[1] - 2.0 * quaternion->fv[3] * quaternion->fv[2];
        matrix->f4v[1][1] = 1.0 - 2.0 * quaternion->fv[0] * quaternion->fv[0] - 2.0 * quaternion->fv[2] * quaternion->fv[2];
        matrix->f4v[1][2] = 2.0 * quaternion->fv[1] * quaternion->fv[2] + 2.0 * quaternion->fv[3] * quaternion->fv[0];
        
        matrix->f4v[2][0] = 2.0 * quaternion->fv[0] * quaternion->fv[2] + 2.0 * quaternion->fv[3] * quaternion->fv[1];
        matrix->f4v[2][1] = 2.0 * quaternion->fv[1] * quaternion->fv[2] - 2.0 * quaternion->fv[3] * quaternion->fv[0];
        matrix->f4v[2][2] = 1.0 - 2.0 * quaternion->fv[0] * quaternion->fv[0] - 2.0 * quaternion->fv[1] * quaternion->fv[1];
    }

    //find angles
    void dpmatrix::getAngles( dpxyz_f *o )
    {
        float a;
        
        o->x = atan2f( this->values.f4v[ 1 ][ 2 ], this->values.f4v[ 2 ][ 2 ] );
        
        a = this->values.f4v[ 1 ][ 2 ] * this->values.f4v[ 1 ][ 2 ];
        a += this->values.f4v[ 2 ][ 2 ] * this->values.f4v[ 2 ][ 2 ];
        o->y = atan2f( -this->values.f4v[ 0 ][ 2 ], sqrtf( a ) );
        
        o->z = atan2f( this->values.f4v[ 0 ][ 1 ], this->values.f4v[ 0 ][ 0 ] );
    }
    
};
