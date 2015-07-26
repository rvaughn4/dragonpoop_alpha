
#include "model_material.h"

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

};
