
#ifndef dragonpoop_openglx_1o5_renderer_model_material_h
#define dragonpoop_openglx_1o5_renderer_model_material_h

#include "../../../renderer_model/renderer_model_material/renderer_model_material.h"

namespace dragonpoop
{
    
    class dpbitmap;
    class openglx_1o5_renderer;
    
    class openglx_1o5_renderer_model_material : public renderer_model_material
    {
        
    private:
        
        struct
        {
            unsigned int diffuse, alphamask, bumpmap, specularmap;
            unsigned int diffuse_sz, alphamask_sz, bumpmap_sz, specularmap_sz;
        } tex;
        openglx_1o5_renderer *r;
        
    protected:
        
        //release texture
        void killTex( unsigned int *t, unsigned int *sz );
        //create and load texture
        void makeTex( unsigned int *t, unsigned int *sz, dpbitmap *bm );
        
    public:
        
        //ctor
        openglx_1o5_renderer_model_material( model_writelock *ml, model_material *m, openglx_1o5_renderer *r );
        //dtor
        virtual ~openglx_1o5_renderer_model_material( void );
        //sync with group
        virtual void sync( model_writelock *ml, model_material *m );
        //return diffuse tex
        unsigned int getDiffuseTex( void );
        //return alpha tex
        unsigned int getAlphaTex( void );
        //return bumpmap tex
        unsigned int getBumpTex( void );
        //return specular map tex
        unsigned int getSpecTex( void );
        
    };
    
};

#endif