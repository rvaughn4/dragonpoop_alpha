
#include "dpbitmap_bitmap_loader.h"
#include <fstream>

namespace dragonpoop
{

    //ctor
    dpbitmap_bitmap_loader::dpbitmap_bitmap_loader( dpbitmap *img )
    {
        this->img = img;
    }

    //dtor
    dpbitmap_bitmap_loader::~dpbitmap_bitmap_loader( void )
    {
    }

    //open file
    bool dpbitmap_bitmap_loader::openFile( std::string *fname )
    {
        std::fstream f;
        std::size_t s;
        char *b;
        unsigned int sz;
        bool r;

        f.open( fname->c_str(), f.binary | f.in );
        if( !f.is_open() )
            return 0;

        f.seekg( 0, f.end );
        s = f.tellg();
        f.seekg( 0, f.beg );

        sz = (unsigned int)s;
        if( !sz )
            return 0;
        b = new char[ sz ];
        if( !b )
            return 0;
        f.read( b, sz );
        f.close();

        r = this->parseHeader( b, sz );

        delete[] b;
        return r;
    }

    //writes bitmap to file
    bool dpbitmap_bitmap_loader::writeFile( std::string *fname )
    {
        std::fstream f;
        char *b;
        unsigned int sz;

        f.open( fname->c_str(), f.binary | f.out | f.trunc );
        if( !f.is_open() )
            return 0;

        sz = this->writeHeader( 0, 0 );
        if( !sz )
            return 0;
        b = new char[ sz ];
        if( !b )
            return 0;

        this->writeHeader( b, sz );
        f.write( b, sz );

        delete[] b;
        return 1;
    }

    //parse header
    bool dpbitmap_bitmap_loader::parseHeader( char *b, unsigned int size )
    {
        bmp_file_header *fh;
        union
        {
            bmp_info *ih;
            bmp_info_v3 *ih3;
        };
        bmp_info dh;

        if( size < sizeof( bmp_file_header ) + sizeof( bmp_info_header ) )
            return 0;

        fh = (bmp_file_header *)b;
        if( fh->size_of_file > size )
            return 0;
        if( fh->offset_to_pixels > size )
            return 0;

        ih = (bmp_info *)&b[ sizeof( bmp_file_header ) ];
        if( ih->hdr.bits != 24 && ih->hdr.bits != 32 )
            return 0;

        if( !this->resizeDst( abs( ih->hdr.width ), abs( ih->hdr.height ) ) )
            return 0;

        if( ih->hdr.compression != 3 )
        {
            memset( &dh.color_masks, 0, sizeof( dh.color_masks ) );
            dh.color_masks.r.c[ 2 ] = 255;
            dh.color_masks.g.c[ 1 ] = 255;
            dh.color_masks.b.c[ 0 ] = 255;
            dh.color_masks.a.c[ 3 ] = 255;
            return this->parsePixels( &b[ fh->offset_to_pixels ], size - fh->offset_to_pixels, ih->hdr.width, ih->hdr.height, ih->hdr.bits, dh.color_masks.r.c, dh.color_masks.g.c, dh.color_masks.b.c, dh.color_masks.a.c );
        }
        if( ih->hdr.size == sizeof( bmp_info_header ) )
            return this->parsePixels( &b[ fh->offset_to_pixels ], size - fh->offset_to_pixels, ih->hdr.width, ih->hdr.height, ih->hdr.bits, ih->color_masks.r.c, ih->color_masks.g.c, ih->color_masks.b.c, ih->color_masks.a.c );
        return this->parsePixels( &b[ fh->offset_to_pixels ], size - fh->offset_to_pixels, ih->hdr.width, ih->hdr.height, ih->hdr.bits, ih3->hdr.color_masks.r.c, ih3->hdr.color_masks.g.c, ih3->hdr.color_masks.b.c, ih3->hdr.color_masks.a.c );
    }

    //writes header to memory, then rest of image, returns size used/needed
    unsigned int dpbitmap_bitmap_loader::writeHeader( char *b, unsigned int sizeMax )
    {
        const char *bm = "BM0";
        unsigned int szn, scn, bpp;
        bmp_file_header *fh;
        bmp_info_v3 *ih;
        unsigned int bits = 32, h_size = sizeof( bmp_info_header_v3 );

        bpp = bits / 8;
        scn = 0;
        while( scn < this->img->getWidth() * bpp )
            scn += 4;

        szn = sizeof( bmp_file_header ) + sizeof( bmp_info_v4 ) + this->img->getHeight() * scn;
        if( !b || sizeMax < szn )
            return szn;

        memset( b, 0, sizeof( bmp_file_header ) + sizeof( bmp_info ) );
        fh = (bmp_file_header *)b;
        ih = (bmp_info_v3 *)&b[ sizeof(bmp_file_header) ];

        memcpy( &fh->f_type, bm, 2 );
        fh->size_of_file = szn;
        fh->offset_to_pixels = sizeof( bmp_file_header ) + sizeof( bmp_info_header_v4 );

        ih->hdr.bits = bits;
        ih->hdr.planes = 1;
        ih->hdr.width = this->img->getWidth();
        ih->hdr.height = this->img->getHeight();
        ih->hdr.size = h_size;
        ih->hdr.size_image = scn * ih->hdr.height;

        if( ih->hdr.bits == 32 )
        {
            fh->offset_to_pixels = sizeof( bmp_file_header ) + sizeof( bmp_info_v4 );
            ih->hdr.compression = 3;
            ih->color_masks.r.c[ 0 ] = 255;
            ih->color_masks.g.c[ 1 ] = 255;
            ih->color_masks.b.c[ 2 ] = 255;
            ih->color_masks.a.c[ 3 ] = 255;
            ih->hdr.color_masks.r.c[ 0 ] = 255;
            ih->hdr.color_masks.g.c[ 1 ] = 255;
            ih->hdr.color_masks.b.c[ 2 ] = 255;
            ih->hdr.color_masks.a.c[ 3 ] = 255;
        }

        this->writePixels( &b[ fh->offset_to_pixels ], ih->hdr.size_image, ih->hdr.bits );

        return szn;
    }

    //resize dst image
    bool dpbitmap_bitmap_loader::resizeDst( unsigned int w, unsigned int h )
    {
        this->img->reset();
        return this->img->resize( w, h );
    }

    //copy pixels
    bool dpbitmap_bitmap_loader::parsePixels( char *b, unsigned int size, int w, int h, unsigned int bits, uint8_t *red_mask, uint8_t *green_mask, uint8_t *blue_mask, uint8_t *alpha_mask )
    {
        unsigned int wu, hu, px, py, sb, scn, pi;
        dprgba clr;
        dpxy pp;
        uint8_t *p;

        wu = abs( w );
        hu = abs( h );
        sb = bits / 8;

        scn = 0;
        while( scn < wu * sb )
            scn += 4;

        for( pp.y = 0; pp.y < hu; pp.y++ )
        {

            if( h <= 0 )
                py = pp.y * scn;
            else
                py = ( hu - pp.y - 1 ) * scn;

            for( pp.x = 0; pp.x < wu; pp.x++ )
            {

                if( w >= 0 )
                    px = pp.x * sb;
                else
                    px = ( wu - pp.x - 1 ) * sb;

                pi = py + px;
                if( pi >= size - sb )
                    continue;

                p = (uint8_t *)&b[ pi ];

                switch( bits )
                {
                    case 32:
                        clr.b = ( p[ 0 ] & blue_mask[ 0 ] )
                        + ( p[ 1 ] & blue_mask[ 1 ] )
                        + ( p[ 2 ] & blue_mask[ 2 ] )
                        + ( p[ 3 ] & blue_mask[ 3 ] );
                        clr.g = ( p[ 0 ] & green_mask[ 0 ] )
                        + ( p[ 1 ] & green_mask[ 1 ] )
                        + ( p[ 2 ] & green_mask[ 2 ] )
                        + ( p[ 3 ] & green_mask[ 3 ] );
                        clr.r = ( p[ 0 ] & red_mask[ 0 ] )
                        + ( p[ 1 ] & red_mask[ 1 ] )
                        + ( p[ 2 ] & red_mask[ 2 ] )
                        + ( p[ 3 ] & red_mask[ 3 ] );
                        clr.a = ( p[ 0 ] & alpha_mask[ 0 ] )
                        + ( p[ 1 ] & alpha_mask[ 1 ] )
                        + ( p[ 2 ] & alpha_mask[ 2 ] )
                        + ( p[ 3 ] & alpha_mask[ 3 ] );
                        break;
                    case 24:
                        clr.b = ( p[ 0 ] & blue_mask[ 0 ] )
                        + ( p[ 1 ] & blue_mask[ 1 ] )
                        + ( p[ 2 ] & blue_mask[ 2 ] )
                        + ( p[ 3 ] & blue_mask[ 3 ] );
                        clr.g = ( p[ 0 ] & green_mask[ 0 ] )
                        + ( p[ 1 ] & green_mask[ 1 ] )
                        + ( p[ 2 ] & green_mask[ 2 ] )
                        + ( p[ 3 ] & green_mask[ 3 ] );
                        clr.r = ( p[ 0 ] & red_mask[ 0 ] )
                        + ( p[ 1 ] & red_mask[ 1 ] )
                        + ( p[ 2 ] & red_mask[ 2 ] )
                        + ( p[ 3 ] & red_mask[ 3 ] );
                        clr.a = 255;
                        break;
                }

                this->img->setColor( &clr, &pp, 0 );
            }
        }

        return 1;
    }

    //write image pixels to memory
    unsigned int dpbitmap_bitmap_loader::writePixels( char *b, unsigned int sizeMax, unsigned int bits )
    {
        dpxy pp;
        unsigned int w, h, py, px, pi, scn, sb;
        uint8_t *p;
        dprgba c;

        w = this->img->getWidth();
        h = this->img->getHeight();
        sb = bits / 8;
        scn = 0;
        while( scn < w * sb )
            scn += 4;

        for( pp.y = 0; pp.y < h; pp.y++ )
        {
            py = (h - pp.y - 1) * scn;
            for( pp.x = 0; pp.x < w; pp.x++ )
            {
                px = pp.x * sb;
                pi = py + px;
                if( pi >= sizeMax )
                    continue;
                p = (uint8_t *)&b[ pi ];
                this->img->getColor( &c, &pp );
                switch( bits )
                {
                    case 24:
                        p[ 2 ] = c.r;
                        p[ 1 ] = c.g;
                        p[ 0 ] = c.b;
                        break;
                    case 32:
                        p[ 0 ] = c.r;
                        p[ 1 ] = c.g;
                        p[ 2 ] = c.b;
                        p[ 3 ] = c.a;
                        break;
                }
            }
        }

        return h * scn;
    }

    //loads a bitmap file
    bool dpbitmap_bitmap_loader::loadFile( dpbitmap *img, const char *fname )
    {
        dpbitmap_bitmap_loader bl( img );
        std::string s( fname );
        return bl.openFile( &s );
    }

    //loads a bitmap from memory
    bool dpbitmap_bitmap_loader::loadMemory( dpbitmap *img, char *b, unsigned int size )
    {
        dpbitmap_bitmap_loader bl( img );
        return bl.parseHeader( b, size );
    }

    //saves a bitmap file
    bool dpbitmap_bitmap_loader::saveFile( dpbitmap *img, const char *fname )
    {
        dpbitmap_bitmap_loader bl( img );
        std::string s( fname );
        return bl.writeFile( &s );
    }
    
    //saves a bitmap to memory, returns size used/needed when params are null
    unsigned int dpbitmap_bitmap_loader::saveMemory( dpbitmap *img, char *b, unsigned int sizeMax )
    {
        dpbitmap_bitmap_loader bl( img );
        return bl.writeHeader( b, sizeMax );
    }
    
};
