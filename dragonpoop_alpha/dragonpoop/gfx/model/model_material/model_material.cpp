
#include "model_material.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_material::model_material( dpid id ) : model_component( id, model_component_type_material )
    {
        
    }
    
    //dtor
    model_material::~model_material( void )
    {
        
    }
    
    //get diffuse tex
    dpbitmap *model_material::getDiffuseTexture( void )
    {
        return &this->tex.diffuse;
    }
    
    //get alpha tex
    dpbitmap *model_material::getAlphaMaskTexture( void )
    {
        return &this->tex.alphamask;
    }
    
    //get bump tex
    dpbitmap *model_material::getBumpMapTexture( void )
    {
        return &this->tex.bumpmap;
    }
    
    //get specular tex
    dpbitmap *model_material::getSpecularMapTexture( void )
    {
        return &this->tex.specularmap;
    }
    
    //get diffuse color
    void model_material::getDiffuseColor( dprgba *x )
    {
        *x = this->colors.diffuse;
    }
    
    //get specular color
    void model_material::getSpecularColor( dprgba *x )
    {
        *x = this->colors.specular;
    }
    
    //get ambient color
    void model_material::getAmbientColor( dprgba *x )
    {
        *x = this->colors.ambient;
    }
    
    //get emissive color
    void model_material::getEmissiveColor( dprgba *x )
    {
        *x = this->colors.emissive;
    }
    
    //set diffuse color
    void model_material::setDiffuseColor( dprgba *x )
    {
        this->colors.diffuse = *x;
    }
    
    //set specular color
    void model_material::setSpecularColor( dprgba *x )
    {
        this->colors.specular = *x;
    }
    
    //set ambient color
    void model_material::setAmbientColor( dprgba *x )
    {
        this->colors.ambient = *x;
    }
    
    //set emissive color
    void model_material::setEmissiveColor( dprgba *x )
    {
        this->colors.emissive = *x;
    }
    
    //set shine
    void model_material::setShine( float f )
    {
        this->shine = f;
    }
    
    //get shine
    float model_material::getShine( void )
    {
        return this->shine;
    }
    
    //set opacity
    void model_material::setOpacity( float f )
    {
        this->opacity = f;
    }
    
    //get opacity
    float model_material::getOpacity( void )
    {
        return this->opacity;
    }
    
    //write data to disk/memory
    bool model_material::writeData( dpbuffer *b )
    {
        model_material_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        
        h.colors = this->colors;
        h.opacity = this->opacity;
        h.shine = this->shine;
        
        h.tex.diffuse.size = this->tex.diffuse.getSize();
        h.tex.diffuse.w = this->tex.diffuse.getWidth();
        h.tex.diffuse.h = this->tex.diffuse.getHeight();
        h.tex.diffuse.bits = this->tex.diffuse.getBitsPerPixel();
        
        h.tex.alphamask.size = this->tex.alphamask.getSize();
        h.tex.alphamask.w = this->tex.alphamask.getWidth();
        h.tex.alphamask.h = this->tex.alphamask.getHeight();
        h.tex.alphamask.bits = this->tex.alphamask.getBitsPerPixel();
        
        h.tex.bumpmap.size = this->tex.bumpmap.getSize();
        h.tex.bumpmap.w = this->tex.bumpmap.getWidth();
        h.tex.bumpmap.h = this->tex.bumpmap.getHeight();
        h.tex.bumpmap.bits = this->tex.bumpmap.getBitsPerPixel();
        
        h.tex.specularmap.size = this->tex.specularmap.getSize();
        h.tex.specularmap.w = this->tex.specularmap.getWidth();
        h.tex.specularmap.h = this->tex.specularmap.getHeight();
        h.tex.specularmap.bits = this->tex.specularmap.getBitsPerPixel();
        
        if( !b->writeBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        
        if( h.tex.diffuse.size && !b->writeBytes( (uint8_t *)this->tex.diffuse.getBuffer(), h.tex.diffuse.size ) )
            return 0;
        if( h.tex.alphamask.size && !b->writeBytes( (uint8_t *)this->tex.alphamask.getBuffer(), h.tex.alphamask.size ) )
            return 0;
        if( h.tex.bumpmap.size && !b->writeBytes( (uint8_t *)this->tex.bumpmap.getBuffer(), h.tex.bumpmap.size ) )
            return 0;
        if( h.tex.specularmap.size && !b->writeBytes( (uint8_t *)this->tex.specularmap.getBuffer(), h.tex.specularmap.size ) )
            return 0;
        
        return 1;
    }

};
