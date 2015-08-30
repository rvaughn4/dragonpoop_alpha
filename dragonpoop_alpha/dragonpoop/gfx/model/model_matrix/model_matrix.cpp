
#include "model_matrix.h"
#include "model_quaternion.h"
#include "model_vector.h"
#include <math.h>
#include <string.h>

#define	Q_PI	3.14159265358979323846
#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])

namespace dragonpoop
{
    
    //ctor
    model_matrix::model_matrix( void )
    {
        this->setIdentity();
    }
    
    //dtor
    model_matrix::~model_matrix( void )
    {
        
    }
    
    //get raw data
    model_matrix_f *model_matrix::getData( void )
    {
        return &this->f;
    }
    
    //set identity
    void model_matrix::setIdentity( void )
    {
        memset( &this->f, 0, sizeof( this->f ) );
    }
    
    //set position from vector
    void model_matrix::setPosition( model_vector *v )
    {
        model_matrix_f *matrix;
        model_vector_f *pos;
        
        pos = v->getData();
        matrix = &this->f;

        matrix->fv[0][3] = pos->fv[0];
        matrix->fv[1][3] = pos->fv[1];
        matrix->fv[2][3] = pos->fv[2];
    }
    
    //set angle from quaternion
    void model_matrix::setAngle( model_quaternion *q )
    {
        float		angle;
        float		sr, sp, sy, cr, cp, cy;
        model_quaternion_f *angles;
        model_matrix_f *matrix;
        
        angles = q->getData();
        matrix = &this->f;
        
        angle = angles->fv[ 2 ];
        sy = sin(angle);
        cy = cos(angle);
        angle = angles->fv[ 1 ];
        sp = sin(angle);
        cp = cos(angle);
        angle = angles->fv[ 0 ];
        sr = sin(angle);
        cr = cos(angle);
        
        // matrix = (Z * Y) * X
        matrix->fv[0][0] = cp*cy;
        matrix->fv[1][0] = cp*sy;
        matrix->fv[2][0] = -sp;
        matrix->fv[0][1] = sr*sp*cy+cr*-sy;
        matrix->fv[1][1] = sr*sp*sy+cr*cy;
        matrix->fv[2][1] = sr*cp;
        matrix->fv[0][2] = (cr*sp*cy+-sr*-sy);
        matrix->fv[1][2] = (cr*sp*sy+-sr*cy);
        matrix->fv[2][2] = cr*cp;
        matrix->fv[0][3] = 0.0;
        matrix->fv[1][3] = 0.0;
        matrix->fv[2][3] = 0.0;
    }
    
    //transform vector
    void model_matrix::transform( model_vector *v )
    {
        model_vector_f in1, *out;
        model_matrix_f *in2;
        
        out = v->getData();
        in1 = *out;
        in2 = &this->f;

        out->fv[0] = DotProduct(in1.fv, in2->fv[0]) + in2->fv[0][3];
        out->fv[1] = DotProduct(in1.fv, in2->fv[1]) + in2->fv[1][3];
        out->fv[2] = DotProduct(in1.fv, in2->fv[2]) + in2->fv[2][3];
    }
    
    //inverse transform vector
    void model_matrix::inverse_transform( model_vector *v )
    {
        model_vector_f in1, *out;
        model_matrix_f *in2;
        
        out = v->getData();
        in1 = *out;
        in2 = &this->f;
        
        out->fv[0] = in1.fv[0] - in2->fv[0][3];
        out->fv[1] = in1.fv[1] - in2->fv[1][3];
        out->fv[2] = in1.fv[2] - in2->fv[2][3];
        
        this->inverse_rotate( v );
    }

    //rotate vector
    void model_matrix::rotate( model_vector *v )
    {
        model_vector_f in1, *out;
        model_matrix_f *in2;
        
        out = v->getData();
        in1 = *out;
        in2 = &this->f;
        
        out->fv[0] = DotProduct(in1.fv, in2->fv[0]);
        out->fv[1] = DotProduct(in1.fv, in2->fv[1]);
        out->fv[2] = DotProduct(in1.fv, in2->fv[2]);
    }
    
    //inverse rotate vector
    void model_matrix::inverse_rotate( model_vector *v )
    {
        model_vector_f in1, *out;
        model_matrix_f *in2;
        
        out = v->getData();
        in1 = *out;
        in2 = &this->f;

        out->fv[0] = in1.fv[0]*in2->fv[0][0] + in1.fv[1]*in2->fv[1][0] + in1.fv[2]*in2->fv[2][0];
        out->fv[1] = in1.fv[0]*in2->fv[0][1] + in1.fv[1]*in2->fv[1][1] + in1.fv[2]*in2->fv[2][1];
        out->fv[2] = in1.fv[0]*in2->fv[0][2] + in1.fv[1]*in2->fv[1][2] + in1.fv[2]*in2->fv[2][2];
    }
    
    //concatinate
    void model_matrix::concat( model_matrix *a, model_matrix *b )
    {
        model_matrix_f *out, *in1, *in2;
        
        out = &this->f;
        in1 = a->getData();
        in2 = b->getData();
        
        out->fv[0][0] = in1->fv[0][0] * in2->fv[0][0] + in1->fv[0][1] * in2->fv[1][0] +
        in1->fv[0][2] * in2->fv[2][0];
        out->fv[0][1] = in1->fv[0][0] * in2->fv[0][1] + in1->fv[0][1] * in2->fv[1][1] +
        in1->fv[0][2] * in2->fv[2][1];
        out->fv[0][2] = in1->fv[0][0] * in2->fv[0][2] + in1->fv[0][1] * in2->fv[1][2] +
        in1->fv[0][2] * in2->fv[2][2];
        out->fv[0][3] = in1->fv[0][0] * in2->fv[0][3] + in1->fv[0][1] * in2->fv[1][3] +
        in1->fv[0][2] * in2->fv[2][3] + in1->fv[0][3];
        out->fv[1][0] = in1->fv[1][0] * in2->fv[0][0] + in1->fv[1][1] * in2->fv[1][0] +
        in1->fv[1][2] * in2->fv[2][0];
        out->fv[1][1] = in1->fv[1][0] * in2->fv[0][1] + in1->fv[1][1] * in2->fv[1][1] +
        in1->fv[1][2] * in2->fv[2][1];
        out->fv[1][2] = in1->fv[1][0] * in2->fv[0][2] + in1->fv[1][1] * in2->fv[1][2] +
        in1->fv[1][2] * in2->fv[2][2];
        out->fv[1][3] = in1->fv[1][0] * in2->fv[0][3] + in1->fv[1][1] * in2->fv[1][3] +
        in1->fv[1][2] * in2->fv[2][3] + in1->fv[1][3];
        out->fv[2][0] = in1->fv[2][0] * in2->fv[0][0] + in1->fv[2][1] * in2->fv[1][0] +
        in1->fv[2][2] * in2->fv[2][0];
        out->fv[2][1] = in1->fv[2][0] * in2->fv[0][1] + in1->fv[2][1] * in2->fv[1][1] +
        in1->fv[2][2] * in2->fv[2][1];
        out->fv[2][2] = in1->fv[2][0] * in2->fv[0][2] + in1->fv[2][1] * in2->fv[1][2] +
        in1->fv[2][2] * in2->fv[2][2];
        out->fv[2][3] = in1->fv[2][0] * in2->fv[0][3] + in1->fv[2][1] * in2->fv[1][3] +
        in1->fv[2][2] * in2->fv[2][3] + in1->fv[2][3];
    }
    
};
