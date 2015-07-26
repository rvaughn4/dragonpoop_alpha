
#include "dpmatrix.h"
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

    //transform
    void dpmatrix::transform( dpvertex_unit *p )
    {
        this->transform( &p->normal );
        this->transform( &p->pos );
    }

};
