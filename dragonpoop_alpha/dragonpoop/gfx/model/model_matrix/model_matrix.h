
#ifndef dragonpoop_model_matrix_h
#define dragonpoop_model_matrix_h

namespace dragonpoop
{
    
    struct model_matrix_f
    {
        union
        {
            float fv[ 3 ][ 4 ];
            struct
            {
                struct
                {
                    float c0, c1, c2, c3;
                } r0, r1, r2;
            } sv;
        };
    };
    
    class model_vector;
    class model_quaternion;
    
    class model_matrix
    {
        
    private:
        
        model_matrix_f f;
        
    protected:
        
    public:
        
        //ctor
        model_matrix( void );
        //dtor
        virtual ~model_matrix( void );
        //get raw data
        model_matrix_f *getData( void );
        //set identity
        void setIdentity( void );
        //set position from vector
        void setPosition( model_vector *v );
        //set angle from quaternion
        void setAngle( model_quaternion *q );
        //transform vector
        void transform( model_vector *v );
        //inverse transform vector
        void inverse_transform( model_vector *v );
        //rotate vector
        void rotate( model_vector *v );
        //inverse rotate vector
        void inverse_rotate( model_vector *v );
        //concatinate
        void concat( model_matrix *a, model_matrix *b );
        
    };
    
};

#endif