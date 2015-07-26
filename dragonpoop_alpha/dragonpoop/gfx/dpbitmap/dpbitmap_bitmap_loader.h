
#ifndef dragonpoop_rimage_bitmap_loader_h
#define dragonpoop_rimage_bitmap_loader_h

#include "dpbitmap.h"
#include <stdint.h>
#include <string>

namespace dragonpoop
{

#pragma pack( 1 )
    struct bmp_file_header
    {
        uint16_t f_type;
        uint32_t size_of_file;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset_to_pixels;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info_header
    {
        uint32_t size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bits;
        uint32_t compression;
        uint32_t size_image;
        int32_t x_pels_per_meter;
        int32_t y_pels_per_meter;
        uint32_t colors_used;
        uint32_t colors_important;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info
    {
        bmp_info_header hdr;
        struct
        {
            struct
            {
                uint8_t c[ 4 ];
            } r, g, b, a;
        } color_masks;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info_header_v3
    {
        uint32_t size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bits;
        uint32_t compression;
        uint32_t size_image;
        int32_t x_pels_per_meter;
        int32_t y_pels_per_meter;
        uint32_t colors_used;
        uint32_t colors_important;
        struct
        {
            struct
            {
                uint8_t c[ 4 ];
            } r, g, b, a;
        } color_masks;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info_v3
    {
        bmp_info_header_v3 hdr;
        struct
        {
            struct
            {
                uint8_t c[ 4 ];
            } r, g, b, a;
        } color_masks;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info_header_v4
    {
        uint32_t size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bits;
        uint32_t compression;
        uint32_t size_image;
        int32_t x_pels_per_meter;
        int32_t y_pels_per_meter;
        uint32_t colors_used;
        uint32_t colors_important;
        struct
        {
            struct
            {
                uint8_t c[ 4 ];
            } r, g, b, a;
        } color_masks;
        uint32_t cs_type;
        struct
        {
            uint32_t r, g, b;
        } endpoints;
        uint32_t red_gamma;
        uint32_t green_gamma;
        uint32_t blue_gamma;
    };
#pragma pack( )

#pragma pack( 1 )
    struct bmp_info_v4
    {
        bmp_info_header_v4 hdr;
        struct
        {
            struct
            {
                uint8_t c[ 4 ];
            } r, g, b, a;
        } color_masks;
    };
#pragma pack( )

    class dpbitmap_bitmap_loader
    {

    private:

        dpbitmap *img;

    protected:

        //ctor
        dpbitmap_bitmap_loader( dpbitmap *img );
        //dtor
        virtual ~dpbitmap_bitmap_loader( void );
        //open file
        bool openFile( std::string *fname );
        //parse header
        bool parseHeader( char *b, unsigned int size );
        //resize dst image
        bool resizeDst( unsigned int w, unsigned int h );
        //copy pixels
        bool parsePixels( char *b, unsigned int size, int w, int h, unsigned int bits, uint8_t *red_mask, uint8_t *green_mask, uint8_t *blue_mask, uint8_t *alpha_mask );
        //writes bitmap to file
        bool writeFile( std::string *fname );
        //writes header to memory, then rest of image, returns size used/needed
        unsigned int writeHeader( char *b, unsigned int sizeMax );
        //write image pixels to memory
        unsigned int writePixels( char *b, unsigned int sizeMax, unsigned int bits );

    public:

        //loads a bitmap file
        static bool loadFile( dpbitmap *img, const char *fname );
        //loads a bitmap from memory
        static bool loadMemory( dpbitmap *img, char *b, unsigned int size );
        //saves a bitmap file
        static bool saveFile( dpbitmap *img, const char *fname );
        //saves a bitmap to memory, returns size used/needed when params are null
        static unsigned int saveMemory( dpbitmap *img, char *b, unsigned int sizeMax );
        
    };
    
};

#endif