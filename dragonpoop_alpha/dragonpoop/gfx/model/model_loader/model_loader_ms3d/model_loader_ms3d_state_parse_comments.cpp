
#include "model_loader_ms3d_state_parse_comments.h"
#include "model_loader_ms3d_state_parse_vertex_extras.h"
#include "model_loader_ms3d_state_make_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d_comment.h"
#include "model_loader_ms3d_group.h"
#include "model_loader_ms3d_material.h"
#include "model_loader_ms3d_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_comments::model_loader_ms3d_state_parse_comments( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_parse_comments::~model_loader_ms3d_state_parse_comments( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_comments::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        if( !this->readGroupComments( b, ml ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        if( !this->readMaterialComments( b, ml ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        if( !this->readJointComments( b, ml ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        if( !this->readModelComments( b, ml ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );

        return new model_loader_ms3d_state_parse_vertex_extras( this->b, this->m );
    }
  
    //read comment into string
    bool model_loader_ms3d_state_parse_comments::readComment( dpbuffer *b, std::string *s )
    {
        ms3d_model_comment_prefix pr;
        union
        {
            ms3d_model_comment *c;
            char *buff;
        };
        
        if( b->readBytes( (uint8_t *)&pr, sizeof( pr ) ) < sizeof( pr ) || pr.size < 0 )
            return 0;
        
        buff = new char[ pr.size + sizeof( pr ) ];
        if( !buff )
            return 0;
        
        if( b->readBytes( (uint8_t *)&c->data, pr.size ) < pr.size )
        {
            delete[] buff;
            return 0;
        }
        
        s->assign( &c->data, pr.size );
        delete[] buff;
        return 1;
    }
    
    //read group comments
    bool model_loader_ms3d_state_parse_comments::readGroupComments( dpbuffer *b, model_loader_writelock *ml )
    {
        unsigned int i, e, ge;
        ms3d_model_comment_section hs;
        bool g;
        std::string s;
        model_loader_ms3d *m;
        ms3d_model_group_m *grp;
        std::vector<ms3d_model_group_m> *v;
        
        m = (model_loader_ms3d *)ml->getLoader();
        v = m->groups;
        ge = (unsigned int)v->size();
        
        if( b->readBytes( (uint8_t *)&hs, sizeof( hs ) ) < sizeof( hs ) || hs.cnt < 0 )
            return 0;
        
        e = (unsigned int)hs.cnt;
        g = 1;
        for( i = 0; i < e && g; i++ )
        {
            g = this->readComment( b, &s );
            if( i >= ge )
                continue;
            grp = &( *v )[ i ];
            grp->cmt.append( s );
        }
     
        return g;
    }
    
    //read material comments
    bool model_loader_ms3d_state_parse_comments::readMaterialComments( dpbuffer *b, model_loader_writelock *ml )
    {
        unsigned int i, e, ge;
        ms3d_model_comment_section hs;
        bool g;
        std::string s;
        model_loader_ms3d *m;
        ms3d_model_material_m *grp;
        std::vector<ms3d_model_material_m> *v;
        
        m = (model_loader_ms3d *)ml->getLoader();
        v = m->materials;
        ge = (unsigned int)v->size();
        
        if( b->readBytes( (uint8_t *)&hs, sizeof( hs ) ) < sizeof( hs ) || hs.cnt < 0 )
            return 0;
        
        e = (unsigned int)hs.cnt;
        g = 1;
        for( i = 0; i < e && g; i++ )
        {
            g = this->readComment( b, &s );
            if( i >= ge )
                continue;
            grp = &( *v )[ i ];
            grp->cmt.append( s );
        }
        
        return g;
    }
    
    //read joint comments
    bool model_loader_ms3d_state_parse_comments::readJointComments( dpbuffer *b, model_loader_writelock *ml )
    {
        unsigned int i, e, ge;
        ms3d_model_comment_section hs;
        bool g;
        std::string s;
        model_loader_ms3d *m;
        ms3d_model_joint_m *grp;
        std::vector<ms3d_model_joint_m> *v;
        
        m = (model_loader_ms3d *)ml->getLoader();
        v = m->joints;
        ge = (unsigned int)v->size();
        
        if( b->readBytes( (uint8_t *)&hs, sizeof( hs ) ) < sizeof( hs ) || hs.cnt < 0 )
            return 0;
        
        e = (unsigned int)hs.cnt;
        g = 1;
        for( i = 0; i < e && g; i++ )
        {
            g = this->readComment( b, &s );
            if( i >= ge )
                continue;
            grp = &( *v )[ i ];
            grp->cmt.append( s );
        }
        
        return g;
    }
    
    //read model comments
    bool model_loader_ms3d_state_parse_comments::readModelComments( dpbuffer *b, model_loader_writelock *ml )
    {
        unsigned int i, e;
        ms3d_model_comment_section hs;
        bool g;
        std::string s;
        model_loader_ms3d *m;
        
        m = (model_loader_ms3d *)ml->getLoader();
        
        if( b->readBytes( (uint8_t *)&hs, sizeof( hs ) ) < sizeof( hs ) || hs.cnt < 0 )
            return 0;
        
        e = (unsigned int)hs.cnt;
        g = 1;
        for( i = 0; i < e && g; i++ )
        {
            g = this->readComment( b, &s );
            m->cmt.append( s );
        }
        
        return g;
    }
    
};
