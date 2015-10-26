
#ifndef dragonpoop_renderer_model_material_h
#define dragonpoop_renderer_model_material_h

#include "../../../gfx/model/model_component/model_components.h"
#include "../../../gfx/dpvertex/dprgba.h"

namespace dragonpoop
{
    
    class model_material;
    class model_writelock;
    class render_api_context_writelock;
    class render_api_texture_ref;
    
    class renderer_model_material : public model_component
    {
        
    private:
        
        struct
        {
            dprgba diffuse, specular, ambient, emissive;
        } colors;
        float shine, opacity;
        struct
        {
            render_api_texture_ref *diffuse, *alphamap, *specmap, *normalmap;
        } tex;
        
    protected:
        
    public:
        
        //ctor
        renderer_model_material( model_writelock *ml, model_material *m, render_api_context_writelock *cl );
        //dtor
        virtual ~renderer_model_material( void );
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
        //sync with group
        virtual void sync( model_writelock *ml, model_material *m, render_api_context_writelock *cl );
        //return diffuse tex
        render_api_texture_ref *getDiffuseTex( void );
        //return alpha tex
        
        

    };
    
};

#endif