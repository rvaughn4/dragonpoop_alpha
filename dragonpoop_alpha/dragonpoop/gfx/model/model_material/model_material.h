
#ifndef dragonpoop_model_material_h
#define dragonpoop_model_material_h

#include "../model_component/model_components.h"
#include "../../../gfx/dpvertex/dprgba.h"
#include "../../../gfx/dpbitmap/dpbitmap.h"

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct model_material_colors
    {
        dprgba diffuse, specular, ambient, emissive;
    };
#pragma pack()

#pragma pack( 1 )
    struct model_material_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct model_material_header_v1
    {
        model_material_header_hdr h;
        struct
        {
            struct
            {
                uint16_t w, h;
                uint8_t bits;
                uint32_t size;
            } diffuse, alphamask, bumpmap, specularmap;
        } tex;
        model_material_colors colors;
        float shine, opacity;

    };
#pragma pack()

    class model_material : public model_component
    {
        
    private:

        struct
        {
            dpbitmap diffuse, alphamask, bumpmap, specularmap;
        } tex;
        model_material_colors colors;
        float shine, opacity;
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );

    public:
        
        //ctor
        model_material( dpid id );
        //dtor
        virtual ~model_material( void );
        //get diffuse tex
        dpbitmap *getDiffuseTexture( void );
        //get alpha tex
        dpbitmap *getAlphaMaskTexture( void );
        //get bump tex
        dpbitmap *getBumpMapTexture( void );
        //get specular tex
        dpbitmap *getSpecularMapTexture( void );
        //get diffuse color
        void getDiffuseColor( dprgba *x );
        //get specular color
        void getSpecularColor( dprgba *x );
        //get ambient color
        void getAmbientColor( dprgba *x );
        //get emissive color
        void getEmissiveColor( dprgba *x );
        //set diffuse color
        void setDiffuseColor( dprgba *x );
        //set specular color
        void setSpecularColor( dprgba *x );
        //set ambient color
        void setAmbientColor( dprgba *x );
        //set emissive color
        void setEmissiveColor( dprgba *x );
        //set shine
        void setShine( float f );
        //get shine
        float getShine( void );
        //set opacity
        void setOpacity( float f );
        //get opacity
        float getOpacity( void );
        
    };
    
};

#endif