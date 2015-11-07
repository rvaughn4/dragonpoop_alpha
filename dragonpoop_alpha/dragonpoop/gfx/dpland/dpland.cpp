
#include "dpland.h"
#include <random>

namespace dragonpoop
{
    
    //ctor
    dpland::dpland( dpid id, int64_t x, int64_t z, float land_sz, float tile_sz, float tex_size )
    {
        this->id = id;
        this->pos.x = x;
        this->pos.z = z;
        this->land_sz = land_sz;
        this->tile_sz = tile_sz;
        this->tex_size = tex_size;
        this->build();
    }
    
    //dtor
    dpland::~dpland( void )
    {
        
    }
    
    //get position
    void dpland::getPosition( dpposition *p )
    {
        dpposition_inner pi;
        
        pi.start.whole.x = this->pos.x;
        pi.start.whole.y = 0;
        pi.start.whole.z = this->pos.z;
        pi.start.fract.x = 0;
        pi.start.fract.y = 0;
        pi.start.fract.z = 0;
        pi.start.t = 0;
        pi.start.bReverse = 0;
        pi.end = pi.start;
        
        p->setData( &pi );
    }
    
    //get position
    void dpland::getPosition( int64_t *x, int64_t *z )
    {
        *x = this->pos.x;
        *z = this->pos.z;
    }
    
    //returns id
    dpid dpland::getId( void )
    {
        return this->id;
    }
    
    //returns vertex buffer
    dpvertex_buffer *dpland::getVB( void )
    {
        return &this->vb;
    }
    
    //returns index buffer
    dpindex_buffer *dpland::getIB( void )
    {
        return &this->ib;
    }
    
    //build land
    void dpland::build( void )
    {
        dpvertex v;
        float x, z;
        int ix, iz, ex, ez, i[ 2 ][ 2 ], cntv;
        
        x = this->land_sz / this->tile_sz;
        ex = (int)x + 1;
        ez = ex;
        
        z = -land_sz * 0.5f;
        cntv = 0;
        for( iz = 0; iz < ez; iz++, z += this->tile_sz )
        {
            x = -land_sz * 0.5f;
            for( ix = 0; ix < ex; ix++, x += this->tile_sz )
            {
                v.pos.x = x;
                v.pos.z = z;
                v.pos.y = this->getHeightAndNormal( x, z, &v.normal );
                v.texcoords[ 0 ].s = (float)ix / (float)ex;
                v.texcoords[ 0 ].t = (float)iz / (float)ez;
                v.texcoords[ 1 ].s = this->tex_size * (float)ix / (float)ex;
                v.texcoords[ 1 ].t = this->tex_size * (float)iz / (float)ez;
                this->vb.addVertex( &v );
                cntv++;
            }
        }
        
        for( iz = 0; iz < ez - 1; iz++ )
        {
            for( ix = 0; ix < ex - 1; ix++ )
            {
                i[ 0 ][ 0 ] = ix + iz * ex;
                i[ 0 ][ 1 ] = i[ 0 ][ 0 ] + 1;
                i[ 1 ][ 0 ] = i[ 0 ][ 0 ] + ex;
                i[ 1 ][ 1 ] = i[ 0 ][ 1 ] + ex;
                
                if( i[ 0 ][ 0 ] >= cntv || i[ 0 ][ 1 ] >= cntv || i[ 1 ][ 0 ] >= cntv || i[ 1 ][ 1 ] >= cntv )
                    continue;
                
                this->ib.addIndex( i[ 0 ][ 0 ] );
                this->ib.addIndex( i[ 1 ][ 0 ] );
                this->ib.addIndex( i[ 0 ][ 1 ] );
                
                this->ib.addIndex( i[ 0 ][ 1 ] );
                this->ib.addIndex( i[ 1 ][ 0 ] );
                this->ib.addIndex( i[ 1 ][ 1 ] );
            }
        }
    }
  
    //get height and normals
    float dpland::getHeightAndNormal( float x, float z, dpxyz_f *n )
    {
        float h[ 3 ];
        dpxyz_f vx, vz, vr;
        
        h[ 0 ] = this->getHeight( x - this->tile_sz, z - this->tile_sz );
        h[ 1 ] = this->getHeight( x + this->tile_sz, z - this->tile_sz );
        h[ 2 ] = this->getHeight( x - this->tile_sz, z + this->tile_sz );
        
        vx.x = -this->tile_sz * 2;
        vx.y = h[ 0 ] - h[ 1 ];
        vx.z = 0;
        
        vz.x = 0;
        vz.y = h[ 0 ] - h[ 2 ];
        vz.z = -this->tile_sz * 2;
        
        vr.x = vx.y * vz.z - vx.z * vz.y;
        vr.y = vx.z * vz.x - vx.x * vz.z;
        vr.z = vx.x * vr.y - vx.y * vz.x;

        x = vr.x * vr.x + vr.y * vr.y + vr.z * vr.z;
        if( x <= 0 )
            x = 1.0f;
        else
            x = sqrtf( x );
        
        n->x = vr.x / x;
        n->y = vr.y / x;
        n->z = vr.z / x;
        
        return h[ 0 ];
    }
    
    //get height
    float dpland::getHeight( float x, float z )
    {
        double dx, dz;
        
        dx = (double)this->pos.x + (double)x;
        dz = (double)this->pos.z + (double)z;
        
        return sinf( dx ) * sinf( dz * 0.25f );
    }
    
};
