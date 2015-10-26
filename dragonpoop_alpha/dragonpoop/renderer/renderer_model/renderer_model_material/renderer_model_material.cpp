
#include "renderer_model_material.h"
#include "../../../gfx/model/model_material/model_material.h"
#include "../../api_stuff/render_api/render_api_context_writelock.h"
#include "../../api_stuff/render_api/render_api_texture_writelock.h"
#include "../../api_stuff/render_api/render_api_texture_ref.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_material::renderer_model_material( model_writelock *ml, model_material *m, render_api_context_writelock *cl ) : model_component( m->getId(), model_component_type_material )
    {
        memset( &this->tex, 0, sizeof( this->tex ) );
        this->sync( ml, m, cl );
    }
    
    //dtor
    renderer_model_material::~renderer_model_material( void )
    {
        if( this->tex.alphamap )
            delete this->tex.alphamap;
        if( this->tex.diffuse )
            delete this->tex.diffuse;
        if( this->tex.normalmap )
            delete this->tex.normalmap;
        if( this->tex.specmap )
            delete this->tex.specmap;
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
    void renderer_model_material::sync( model_writelock *ml, model_material *m, render_api_context_writelock *cl )
    {
        dpbitmap *bm;
        
        m->getDiffuseColor( &this->colors.diffuse );
        m->getSpecularColor( &this->colors.specular );
        m->getAmbientColor( &this->colors.ambient );
        m->getEmissiveColor( &this->colors.emissive );
        this->shine = m->getShine();
        this->opacity = m->getOpacity();
        
        if( this->tex.alphamap )
            delete this->tex.alphamap;
        if( this->tex.diffuse )
            delete this->tex.diffuse;
        if( this->tex.normalmap )
            delete this->tex.normalmap;
        if( this->tex.specmap )
            delete this->tex.specmap;
        memset( &this->tex, 0, sizeof( this->tex ) );

        bm = m->getAlphaMaskTexture();
        if( bm && bm->getSize() )
            this->tex.alphamap = cl->makeTexture( bm );
        bm = m->getDiffuseTexture();
        if( bm && bm->getSize() )
            this->tex.diffuse = cl->makeTexture( bm );
        bm = m->getBumpMapTexture();
        if( bm && bm->getSize() )
            this->tex.normalmap = cl->makeTexture( bm );
        bm = m->getSpecularMapTexture();
        if( bm && bm->getSize() )
            this->tex.specmap = cl->makeTexture( bm );
        
    }

    //return diffuse tex
    render_api_texture_ref *renderer_model_material::getDiffuseTex( void )
    {
        return this->tex.diffuse;
    }
    
};
