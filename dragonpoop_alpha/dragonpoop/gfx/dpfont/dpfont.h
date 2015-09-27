
#ifndef dragonpoop_dpfont_h
#define dragonpoop_dpfont_h

#include <ft2build.h>
#include FT_FREETYPE_H

namespace dragonpoop
{

    class dpbitmap;
    
    class dpfont
    {
        
    private:
        
        FT_Library  lb;
        FT_Face fc;
        bool lb_loaded, fc_loaded;
        
    protected:
        
    public:
        
        //ctor
        dpfont( void );
        //dtor
        virtual ~dpfont( void );
        //open font file
        bool openFont( const char *path_name, const char *file_name, unsigned int size );
        //render chracter to bitmap at x, y
        void draw( unsigned int letter, int x, int y, dpbitmap *bm );
        
    };
    
};

#endif