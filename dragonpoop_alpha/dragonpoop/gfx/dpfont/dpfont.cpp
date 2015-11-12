
#include "dpfont.h"
#include "../dpbitmap/dpbitmap.h"
#include <string>

namespace dragonpoop
{

    //ctor
    dpfont::dpfont( void )
    {
        this->lb_loaded = this->fc_loaded = 0;
        if( !FT_Init_FreeType( &this->lb ) )
            this->lb_loaded = 1;
    }

    //dtor
    dpfont::~dpfont( void )
    {
        if( this->fc_loaded )
            FT_Done_Face( this->fc );
        if( this->lb_loaded )
            FT_Done_FreeType( this->lb );
    }

    //open font file
    bool dpfont::openFont( const char *font_name, unsigned int size )
    {
        std::string s;
        bool r;
        int i;
        char c;

        if( !this->lb_loaded )
            return 0;
        if( this->fc_loaded )
            FT_Done_Face( this->fc );
        this->fc_loaded = 0;
        r = 0;

        s.assign( font_name );
        for( i = 0; i < (int)s.size(); i++ )
        {
            c = font_name[ i ];

            if( !
               (
                    ( c >= 48 && c <= 57 )
                    ||
                    ( c >= 65 && c <= 90 )
                    ||
                    ( c >= 97 && c <= 122 )
                    ||
                    ( c == 46 )
                    ||
                    ( c == 32 )
                )
               )
                return 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".ttf" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".ttc" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".otf" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".fnt" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".pfm" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".afm" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".dfont" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
        {
            s.assign( font_name );
            s.append( ".woff" );
            r = FT_New_Face( this->lb, s.c_str(), 0, &this->fc ) == 0;
        }

        if( !r )
            return 0;

        if( FT_Set_Pixel_Sizes( this->fc, size, 0 ) )
        {
            FT_Done_Face( this->fc );
            return 0;
        }
        this->sz = size;

        this->fc_loaded = 1;
        return 1;
    }

    //render chracter to bitmap at x, y
    void dpfont::draw( unsigned int letter, int x, int y, dpbitmap *bm, int *osx, int *osy, dprgba *clr )
    {
        if( !this->lb_loaded || !this->fc_loaded )
            return;

        if( FT_Load_Char( this->fc, letter, FT_LOAD_RENDER ) )
            return;
        this->drawBitmap( bm, x, y, osx, osy, clr );
    }

    //draw bitmap
    void dpfont::drawBitmap( dpbitmap *bm, int sx, int sy, int *osx, int *osy, dprgba *clr )
    {
        unsigned int x, y, by, bh, c;
        dpxy xy;
        dprgba rg;

        by = (int)this->fc->glyph->metrics.horiBearingY >> 6;
        bh = (int)this->fc->glyph->metrics.height >> 6;
        by = bh - by;
        bh = this->sz - bh + by;

        for( y = 0; y < (unsigned int)abs( this->fc->glyph->bitmap.rows ); y++ )
        {
            xy.y = y + sy + bh;
            for( x = 0; x < (unsigned int)abs( this->fc->glyph->bitmap.width ); x++ )
            {
                xy.x = x + sx;

                c = this->fc->glyph->bitmap.buffer[ y * abs( this->fc->glyph->bitmap.pitch ) + x ];

                if( c > 220 )
                    c = 255;
                else
                {
                    if( c < 20 )
                        c = 0;
                    else
                    {
                        c = c - 20;
                        c = ( c * 255 ) / 200;
                    }
                }

                if( clr )
                {
                    if( c > 5 )
                    {
                        rg.r = clr->r;
                        rg.g = clr->g;
                        rg.b = clr->b;
                        rg.a = clr->a * (float)c / 255.0f;
                    }
                    else
                    {
                        rg.r = rg.g = rg.b = rg.a = 0;
                    }
                }
                else
                {
                    rg.r = 255 - (float)c;
                    rg.g = 255 - (float)c;
                    rg.b = 255 - (float)c;
                    rg.a = (float)c;
                }
                if( bm )
                    bm->setColor( &rg, &xy, 1 );

            }
        }

        if( osx )
            *osx = (int)this->fc->glyph->advance.x >> 6;
        if( osy )
            *osy = bh + (int)(this->fc->glyph->advance.y >> 6);
    }

};
