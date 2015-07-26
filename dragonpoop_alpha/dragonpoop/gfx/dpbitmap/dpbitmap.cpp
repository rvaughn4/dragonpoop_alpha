
#include "dpbitmap.h"
#include "dpbitmap_bitmap_loader.h"
#include <string.h>
#include <string>

namespace dragonpoop
{

    //ctor
    dpbitmap::dpbitmap( void )
    {
        this->buffer.data = 0;
        this->reset();
    }

    //ctor
    dpbitmap::dpbitmap( dpbitmap *c )
    {
        this->buffer.data = 0;
        this->reset();
        this->copy( c );
    }

    //dtor
    dpbitmap::~dpbitmap( void )
    {
        this->reset();
    }

    //get width
    unsigned int dpbitmap::getWidth( void )
    {
        return this->dims.w;
    }

    //get height
    unsigned int dpbitmap::getHeight( void )
    {
        return this->dims.h;
    }

    //get dimensions
    void dpbitmap::getDimensions( dpxywh *r )
    {
        *r = this->dims;
    }

    //get bits per pixel
    unsigned int dpbitmap::getBitsPerPixel( void )
    {
        return 32;
    }

    //get bytes per pixel
    unsigned int dpbitmap::getBytesPerPixel( void )
    {
        return 4;
    }

    //get scanline size in bytes
    unsigned int dpbitmap::getScanlineSize( void )
    {
        return this->getWidth() * this->getBytesPerPixel();
    }

    //get size in bytes of image
    unsigned int dpbitmap::getSize( void )
    {
        return this->buffer.size;
    }

    //get image buffer
    char *dpbitmap::getBuffer( void )
    {
        return this->buffer.data;
    }

    //resize image (optional stretch contents)
    bool dpbitmap::resize( unsigned int w, unsigned int h )
    {
        this->reset();
        this->buffer.size = w * h * 4;
        this->buffer.data = new char[ this->buffer.size ];
        this->dims.w = w;
        this->dims.h = h;
        return this->buffer.data != 0;
    }

    //copy image
    void dpbitmap::copy( dpbitmap *c )
    {
        unsigned int w, h;
        dprgba r;
        dpxy p;

        w = c->getWidth();
        h = c->getHeight();
        this->reset();
        this->resize( w, h );

        for( p.y = 0; p.y < h; p.y++ )
        {
            for( p.x = 0; p.x < w; p.x++ )
            {
                c->getColor( &r, &p );
                this->setColor( &r, &p, 0 );
            }
        }
    }

    //reset image
    void dpbitmap::reset( void )
    {
        if( this->buffer.data )
            delete[] this->buffer.data;
        this->buffer.data = 0;
        this->buffer.size= 0;
        memset( &this->dims, 0, sizeof( this->dims ) );
    }

    //clear image to color
    void dpbitmap::clear( dprgba *c )
    {
        this->clear( c, &this->dims );
    }

    //set image to color in rect
    void dpbitmap::clear( dprgba *c, dpxywh *r )
    {
        dpxy p;

        for( p.y = r->p.y; p.y < r->p.y + r->h; p.y++ )
        {
            for( p.x = r->p.x; p.x < r->p.x + r->w; p.x++ )
                this->setColor( c, &p, 1 );
        }
    }

    //set color at pixel
    void dpbitmap::setColor( dprgba *c, dpxy *p, bool doAlphaBlend )
    {
        uint8_t *px;
        unsigned int pi, da, sa, v;

        if( p->x < 0 || p->x >= this->dims.w )
            return;
        if( p->y < 0 || p->y >= this->dims.h )
            return;

        pi = p->y * this->getScanlineSize();
        pi += p->x * this->getBytesPerPixel();
        if( pi >= this->buffer.size || !this->buffer.data )
            return;

        px = (uint8_t *)&this->buffer.data[ pi ];

        if( !doAlphaBlend )
        {
            px[ 0 ] = c->r;
            px[ 1 ] = c->g;
            px[ 2 ] = c->b;
            px[ 3 ] = c->a;
            return;
        }

        sa = c->a;
        da = px[ 3 ];

        v = ( ( px[ 0 ] * da ) + ( c->r * sa ) ) / 255;
        if( v > 255 )
            v = 255;
        px[ 0 ] = (uint8_t)v;

        v = ( ( px[ 1 ] * da ) + ( c->g * sa ) ) / 255;
        if( v > 255 )
            v = 255;
        px[ 1 ] = (uint8_t)v;

        v = ( ( px[ 2 ] * da ) + ( c->b * sa ) ) / 255;
        if( v > 255 )
            v = 255;
        px[ 2 ] = (uint8_t)v;

        v = ( ( px[ 3 ] * da ) + ( c->a * sa ) ) / 255;
        if( v > 255 )
            v = 255;
        px[ 3 ] = (uint8_t)v;
    }

    //get color at pixel
    void dpbitmap::getColor( dprgba *c, dpxy *p )
    {
        uint8_t *px;
        unsigned int pi;

        if( p->x < 0 )
            p->x = 0;
        if( p->y < 0 )
            p->y = 0;
        if( p->x >= this->dims.w )
            p->x = this->dims.w - 1;
        if( p->y >= this->dims.h )
            p->y = this->dims.h - 1;

        pi = p->y * this->getScanlineSize();
        pi += p->x * this->getBytesPerPixel();
        if( pi >= this->buffer.size || !this->buffer.data )
            return;

        px = (uint8_t *)&this->buffer.data[ pi ];
        c->r = px[ 0 ];
        c->g = px[ 1 ];
        c->b = px[ 2 ];
        c->a = px[ 3 ];
    }

    //get average color in rectangle
    void dpbitmap::getColor( dprgba *c, dpxywh *r )
    {
        struct
        {
            unsigned int r, g, b, a, c;
        } cr;
        dpxy p;
        dprgba t;

        memset( &cr, 0, sizeof( cr ) );

        for( p.y = r->p.y; p.y < r->p.y + r->h; p.y++ )
        {
            for( p.x = r->p.x; p.x < r->p.x + r->w; p.x++ )
            {
                this->getColor( &t, &p );
                cr.r += t.r;
                cr.g += t.g;
                cr.b += t.b;
                cr.a += t.a;
                cr.c += 1;
            }
        }

        if( !cr.c )
            cr.c = 1;
        cr.r /= cr.c;
        cr.g /= cr.c;
        cr.b /= cr.c;
        cr.a /= cr.c;

        c->r = (uint8_t)cr.r;
        c->g = (uint8_t)cr.g;
        c->b = (uint8_t)cr.b;
        c->a = (uint8_t)cr.a;
    }

    //get color of supersampled pixel given destination size and pixel location
    void dpbitmap::getColor( dprgba *c, dpxy *p, dpxywh *dst_dims, dpxy *dst_pos )
    {
        dprgba tl, tr, bl, br;
        dpxy cp;
        float scale_x, scale_y, rat_x, rat_y, p_x, p_y, m_rat_x, m_rat_y, v;

        scale_x = dst_dims->w / this->dims.w;
        scale_y = dst_dims->h / this->dims.h;

        p_x = dst_pos->x / scale_x;
        p_y = dst_pos->y / scale_y;
        p_x = p->x - p_x;
        p_y = p->y - p_y;

        rat_x = p_x / scale_x;
        rat_y = p_y / scale_y;
        m_rat_x = 1.0f - rat_x;
        m_rat_y = 1.0f - rat_y;

        cp = *p;
        this->getColor( &tl, &cp );
        cp.x += 1;
        this->getColor( &tr, &cp );
        cp = *p;
        cp.y += 1;
        this->getColor( &bl, &cp );
        cp.x += 1;
        this->getColor( &br, &cp );

        v = tl.r * m_rat_x * m_rat_y + tr.r * rat_x * m_rat_y + bl.r * m_rat_x * rat_y + br.r * rat_x * rat_y;
        if( v > 255 )
            v = 255;
        c->r = (uint8_t)v;
        v = tl.g * m_rat_x * m_rat_y + tr.g * rat_x * m_rat_y + bl.g * m_rat_x * rat_y + br.g * rat_x * rat_y;
        if( v > 255 )
            v = 255;
        c->g = (uint8_t)v;
        v = tl.b * m_rat_x * m_rat_y + tr.b * rat_x * m_rat_y + bl.b * m_rat_x * rat_y + br.b * rat_x * rat_y;
        if( v > 255 )
            v = 255;
        c->b = (uint8_t)v;
        v = tl.a * m_rat_x * m_rat_y + tr.a * rat_x * m_rat_y + bl.a * m_rat_x * rat_y + br.a * rat_x * rat_y;
        if( v > 255 )
            v = 255;
        c->a = (uint8_t)v;
    }

    //draw image into this image (stretch to fit, alpha blending optional)
    void dpbitmap::blit( dpbitmap *c, bool doAlphaBlend )
    {
        this->blit( c, doAlphaBlend, &this->dims );
    }

    //draw image into this image at location  (no stretching, alpha blending optional)
    void dpbitmap::blit( dpbitmap *c, bool doAlphaBlend, dpxy *dst_pos )
    {
        dpxywh r;
        r.p = *dst_pos;
        r.w = this->dims.w;
        r.h = this->dims.h;
        this->blit( c, doAlphaBlend, &r );
    }

    //draw image into this image at rectangle (stretch to fit, alpha blending optional)
    void dpbitmap::blit( dpbitmap *c, bool doAlphaBlend, dpxywh *dst_rect )
    {
        dpxywh r;
        c->getDimensions( &r );
        this->blit( c, doAlphaBlend, &r, dst_rect );
    }

    //draw image from position into this image at rectangle (stretch to fit, alpha blending optional)
    void dpbitmap::blit( dpbitmap *c, bool doAlphaBlend, dpxy *src_pos, dpxywh *dst_rect )
    {
        dpxywh r;
        c->getDimensions( &r );
        r.p = *src_pos;
        r.w -= r.p.x;
        r.h -= r.p.y;
        this->blit( c, doAlphaBlend, &r, dst_rect );
    }

    //draw image from rectangle into this image at rectangle (stretch to fit, alpha blending optional)
    void dpbitmap::blit( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect )
    {
        if( src_rect->w == dst_rect->w && src_rect->h == dst_rect->h )
            return this->blitSame( c, doAlphaBlend, src_rect, dst_rect );
        if( src_rect->w <= dst_rect->w && src_rect->h <= dst_rect->h )
            return this->blitZoom( c, doAlphaBlend, src_rect, dst_rect );
        if( src_rect->w >= dst_rect->w && src_rect->h >= dst_rect->h )
            return this->blitShrink( c, doAlphaBlend, src_rect, dst_rect );
        return this->blitBoth( c, doAlphaBlend, src_rect, dst_rect );
    }

    //blit image where both dimensions in dst are the same as src
    void dpbitmap::blitSame( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect )
    {
        dpxy p, pc;
        dprgba rc;

        for( p.y = dst_rect->p.y; p.y < dst_rect->h + dst_rect->p.y; p.y++ )
        {
            for( p.x = dst_rect->p.x; p.x < dst_rect->w + dst_rect->p.x; p.x++ )
            {
                pc.x = p.x - dst_rect->p.x + src_rect->p.x;
                pc.y = p.y - dst_rect->p.y + src_rect->p.y;
                c->getColor( &rc, &pc );
                this->setColor( &rc, &p, doAlphaBlend );
            }
        }
    }

    //blit image where both dimensions in dst are larger than src
    void dpbitmap::blitZoom( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect )
    {
        dpxy p, pc;
        dprgba rc;
        float rat_x, rat_y, f;

        rat_x = dst_rect->w / src_rect->w;
        rat_y = dst_rect->h / src_rect->h;

        for( p.y = dst_rect->p.y; p.y < dst_rect->h + dst_rect->p.y; p.y++ )
        {
            for( p.x = dst_rect->p.x; p.x < dst_rect->w + dst_rect->p.x; p.x++ )
            {
                f = p.x - dst_rect->p.x;
                f /= rat_x;
                pc.x = f + src_rect->p.x;
                f = p.y - dst_rect->p.y;
                f /= rat_y;
                pc.y = f + src_rect->p.y;
                c->getColor( &rc, &pc, dst_rect, &p );
                this->setColor( &rc, &p, doAlphaBlend );
            }
        }
    }

    //blit image where both dimensions in dst are smaller than src
    void dpbitmap::blitShrink( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect )
    {
        dpxy p;
        dpxywh pc;
        dprgba rc;
        float rat_x, rat_y, f;

        rat_x = src_rect->w / dst_rect->w;
        rat_y = src_rect->h / dst_rect->h;
        pc.w = rat_x;
        if( pc.w < 1 )
            pc.w = 1;
        pc.h = rat_y;
        if( pc.h < 1 )
            pc.h = 1;

        for( p.y = dst_rect->p.y; p.y < dst_rect->h + dst_rect->p.y; p.y++ )
        {
            for( p.x = dst_rect->p.x; p.x < dst_rect->w + dst_rect->p.x; p.x++ )
            {
                f = p.x - dst_rect->p.x;
                f *= rat_x;
                pc.p.x = f + src_rect->p.x;
                f = p.y - dst_rect->p.y;
                f *= rat_y;
                pc.p.y = f + src_rect->p.y;
                c->getColor( &rc, &pc );
                this->setColor( &rc, &p, doAlphaBlend );
            }
        }
    }

    //blit image where dimensions are smaller and larger than src
    void dpbitmap::blitBoth( dpbitmap *c, bool doAlphaBlend, dpxywh *src_rect, dpxywh *dst_rect )
    {
        dpxy p, pc;
        dprgba rc;
        float rat_x, rat_y, f;

        rat_x = src_rect->w / dst_rect->w;
        rat_y = src_rect->h / dst_rect->h;

        for( p.y = dst_rect->p.y; p.y < dst_rect->h + dst_rect->p.y; p.y++ )
        {
            for( p.x = dst_rect->p.x; p.x < dst_rect->w + dst_rect->p.x; p.x++ )
            {
                f = p.x - dst_rect->p.x;
                f *= rat_x;
                pc.x = f + src_rect->p.x;
                f = p.y - dst_rect->p.y;
                f *= rat_y;
                pc.y = f + src_rect->p.y;
                c->getColor( &rc, &pc );
                this->setColor( &rc, &p, doAlphaBlend );
            }
        }
    }

    //gausian blur
    void dpbitmap::gausian_blur( unsigned int rad, float intensity )
    {
        dpbitmap i;
        dpxy pp;
        dprgba p_d, p_s;
        dpxywh pps;
        float f;

        i.copy( this );
        for( pp.y = 0; pp.y < this->dims.h; pp.y++ )
        {
            for( pp.x = 0; pp.x < this->dims.w; pp.x++ )
            {
                pps.w = rad;
                pps.h = rad;
                pps.p.x = pp.x - pps.w;
                pps.p.y = pp.y - pps.h;
                i.getColor( &p_s, &pps );
                this->getColor( &p_d, &pp );

                f = ( (float)p_d.r - (float)p_s.r ) * intensity;
                f = (float)p_d.r - f;
                if( f > 255 )
                    f = 255;
                if( f < 0 )
                    f = 0;
                p_d.r = (uint8_t)f;

                f = ( (float)p_d.g - (float)p_s.g ) * intensity;
                f = (float)p_d.g - f;
                if( f > 255 )
                    f = 255;
                if( f < 0 )
                    f = 0;
                p_d.g = (uint8_t)f;

                f = ( (float)p_d.b - (float)p_s.b ) * intensity;
                f = (float)p_d.b - f;
                if( f > 255 )
                    f = 255;
                if( f < 0 )
                    f = 0;
                p_d.b = (uint8_t)f;

                this->setColor( &p_d, &pp, 0 );
            }
        }
    }


    //gausian blur
    void dpbitmap::blur( float intensity )
    {
        this->gausian_blur( 3, intensity );
    }

    //gausian blur
    void dpbitmap::sharpen( float intensity )
    {
        this->gausian_blur( 2, -intensity );
    }

    //loads a file, detects file type by using extension
    bool dpbitmap::loadFile( const char *fname )
    {
        std::string sname, ext;
        std::size_t lext;

        sname.assign( fname );
        lext = sname.find_last_of( "." );
        if( lext >= sname.size() )
            return 0;

        ext = sname.substr( lext + 1 );

        if( ext.compare( "bmp" ) == 0 )
            return dpbitmap_bitmap_loader::loadFile( this, fname );
        
        return 0;
    }
    
    //saves a file, detects file type by extension
    bool dpbitmap::saveFile( const char *fname )
    {
        std::string sname, ext;
        std::size_t lext;
        
        sname.assign( fname );
        lext = sname.find_last_of( "." );
        if( lext >= sname.size() )
            return 0;
        
        ext = sname.substr( lext + 1 );
        
        if( ext.compare( "bmp" ) == 0 )
            return dpbitmap_bitmap_loader::saveFile( this, fname );
        
        return 0;
    }
    
};

