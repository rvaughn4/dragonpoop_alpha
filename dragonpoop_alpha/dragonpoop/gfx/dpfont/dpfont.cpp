
#include "dpfont.h"
#include "../dpbitmap/dpbitmap.h"

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
    bool dpfont::openFont( const char *path_name, const char *file_name, unsigned int size )
    {

        if( !this->lb_loaded )
            return 0;
        if( this->fc_loaded )
            FT_Done_Face( this->fc );
        this->fc_loaded = 0;
        
        if( FT_New_Face( this->lb, file_name, 0, &this->fc ) )
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
        unsigned int x, y, mx, my, by, bh;
        unsigned char c;
        dpxy xy;
        dprgba rg;
        
        by = (int)this->fc->glyph->metrics.horiBearingY >> 6;
        bh = (int)this->fc->glyph->metrics.height >> 6;
        by = bh - by;
        bh = this->sz - bh + by;

        mx = my = 0;
        for( y = 0; y < this->fc->glyph->bitmap.rows; y++ )
        {
            xy.y = y + sy + bh;
            for( x = 0; x < this->fc->glyph->bitmap.width; x++ )
            {
                xy.x = x + sx;
                
                c = this->fc->glyph->bitmap.buffer[ y * abs( this->fc->glyph->bitmap.pitch ) + x ];

                if( clr )
                {
                    rg.r = clr->r * (float)c / 256.0f;
                    rg.g = clr->g * (float)c / 256.0f;
                    rg.b = clr->b * (float)c / 256.0f;
                    rg.a = clr->a * (float)c / 256.0f;
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
