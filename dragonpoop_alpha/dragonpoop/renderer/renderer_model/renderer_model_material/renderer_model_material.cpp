
#include "renderer_model_material.h"
#include "../../../gfx/model/model_material/model_material.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_material::renderer_model_material( model_writelock *ml, model_material *m ) : model_component( m->getId(), model_component_type_material )
    {
        this->sync( ml, m );
    }
    
    //dtor
    renderer_model_material::~renderer_model_material( void )
    {
        
    }
    
    //get diffuse color
    void renderer_model_material::getDiffuseColor( dprgba *x )
    {
        *x = this->colors.diffuse;
    }
    
    //get specular color
    void renderer_model_material::getSpecularColor( dprgba *x )
    {
        *x = this->colors.specular;
    }
    
    //get ambient color
    void renderer_model_material::getAmbientColor( dprgba *x )
    {
        *x = this->colors.ambient;
    }
    
    //get emissive color
    void renderer_model_material::getEmissiveColor( dprgba *x )
    {
        *x = this->colors.emissive;
    }
    
    //set diffuse color
    void renderer_model_material::setDiffuseColor( dprgba *x )
    {
        this->colors.diffuse = *x;
    }
    
    //set specular color
    void renderer_model_material::setSpecularColor( dprgba *x )
    {
        this->colors.specular = *x;
    }
    
    //set ambient color
    void renderer_model_material::setAmbientColor( dprgba *x )
    {
        this->colors.ambient = *x;
    }
    
    //set emissive color
    void renderer_model_material::setEmissiveColor( dprgba *x )
    {
        this->colors.emissive = *x;
    }
    
    //set shine
    void renderer_model_material::setShine( float f )
    {
        this->shine = f;
    }
    
    //get shine
    float renderer_model_material::getShine( void )
    {
        return this->shine;
    }
    
    //set opacity
    void renderer_model_material::setOpacity( float f )
    {
        this->opacity = f;
    }
    
    //get opacity
    float renderer_model_material::getOpacity( void )
    {
        return this->opacity;
    }
    
    //sync with group
    void renderer_model_material::sync( model_writelock *ml, model_material *m )
    {
        m->getDiffuseColor( &this->colors.diffuse );
        m->getSpecularColor( &this->colors.specular );
        m->getAmbientColor( &this->colors.ambient );
        m->getEmissiveColor( &this->colors.emissive );
        this->shine = m->getShine();
        this->opacity = m->getOpacity();
    }
    
};
