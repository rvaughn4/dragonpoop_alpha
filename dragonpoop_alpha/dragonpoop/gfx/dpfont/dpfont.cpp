
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
        
        this->fc_loaded = 1;
        return 1;
    }
    
    //render chracter to bitmap at x, y
    void dpfont::draw( unsigned int letter, int x, int y, dpbitmap *bm )
    {
        
        if( !this->lb_loaded || !this->fc_loaded )
            return;
        
        
    }

};
