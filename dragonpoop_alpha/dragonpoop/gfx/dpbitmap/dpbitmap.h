
#ifndef dragonpoop_dpbitmap_h
#define dragonpoop_dpbitmap_h

#include "../dpvertex/dpvertexes.h"

namespace dragonpoop
{

    struct dpxy
    {
        int32_t x, y;
    };

    struct dpxywh
    {
        dpxy p;
        int32_t w, h;
    };

    class dpbitmap
    {

    private:

        //dims
        dpxywh dims;
        //buffer
        struct
        {
            unsigned int size;
            char *data;
        } buffer;

    protected:

        //blit image where both dimensions in dst are the same as src
        void blitSame( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect );
        //blit image where both dimensions in dst are larger than src
        void blitZoom( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect );
        //blit image where both dimensions in dst are smaller than src
        void blitShrink( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect );
        //blit image where dimensions are smaller and larger than src
        void blitBoth( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect );

    public:

        //ctor
        dpbitmap( void );
        //ctor
        dpbitmap( dpbitmap *c );
        //dtor
        virtual ~dpbitmap( void );
        //get width
        unsigned int getWidth( void );
        //get height
        unsigned int getHeight( void );
        //get dimensions
        void getDimensions( dpxywh *r );
        //get bits per pixel
        unsigned int getBitsPerPixel( void );
        //get bytes per pixel
        unsigned int getBytesPerPixel( void );
        //get scanline size in bytes
        unsigned int getScanlineSize( void );
        //get size in bytes of image
        unsigned int getSize( void );
        //get image buffer
        char *getBuffer( void );
        //resize image (optional stretch contents)
        bool resize( unsigned int w, unsigned int h );
        //copy image
        void copy( dpbitmap *c );
        //reset image
        void reset( void );
        //clear image to color
        void clear( dprgba *c );
        //set image to color in rect
        void clear( dprgba *c, dpxywh *r );
        //set color at pixel
        void setColor( dprgba *c, dpxy *p, bool doAlphaBlend );
        //get color at pixel
        void getColor( dprgba *c, dpxy *p );
        //get average color in rectangle
        void getColor( dprgba *c, dpxywh *r );
        //get color of supersampled pixel given destination size and pixel location
        void getColor( dprgba *c, dpxy *p, dpxywh *dst_dims, dpxy *dst_pos );
        //draw image into this image (stretch to fit, alpha blending optional)
        void blit( dpbitmap *c, bool doAlphaBlend );
        //draw image into this image at location  (no stretching, alpha blending optional)
        void blit( dpbitmap *c, bool doAlphaBlend, dpxy *dst_pos );
        //draw image into this image at rectangle (stretch to fit, alpha blending optional)
        void blit( dpbitmap *c, bool doAlphaBlend, dpxywh *dst_rect );
        //draw image from position into this image at rectangle (stretch to fit, alpha blending optional)
        void blit( dpbitmap *c, bool doAlphaBlend, dpxy *src_pos, dpxywh *dst_rect );
        //draw image from rectangle into this image at rectangle (stretch to fit, alpha blending optional)
        void blit( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect );
        //gausian blur
        void gausian_blur( unsigned int rad, float intensity );
        //blur
        void blur( float intensity );
        //sharpen
        void sharpen( float intensity );
        //loads a file, detects file type by using extension
        bool loadFile( const char *fname );
        //saves a file, detects file type by extension
        bool saveFile( const char *fname );
        
    };
    
};

#endif