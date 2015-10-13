
#include "model_instance_writelock.h"
#include "model_instance.h"

namespace dragonpoop
{

    //ctor
    model_instance_writelock::model_instance_writelock( model_instance *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_instance_writelock::~model_instance_writelock( void )
    {
        
    }
    
    //return core
    core *model_instance_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get id
    dpid model_instance_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *model_instance_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_instance_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_instance_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_instance_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find group
    model_instance_group *model_instance_writelock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_instance_writelock::getGroups( std::list<model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //run model from task
    void model_instance_writelock::run( dpthread_lock *thd, model_writelock *m, unsigned int ms_each_frame )
    {
        this->t->run( thd, this, m, ms_each_frame );
    }
    
    //sync
    void model_instance_writelock::sync( void )
    {
        this->t->sync();
    }

    //set renderer model
    void model_instance_writelock::setRenderer( renderer_model_instance *r )
    {
        this->t->setRenderer( r );
    }
    
    //populate vertex buffer for rendering
    void model_instance_writelock::fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb )
    {
        this->t->fillVertexBuffer( group_id, vb );
    }

    //find animation
    model_instance_animation *model_instance_writelock::findAnimation( dpid id )
    {
        return this->t->findAnimation( id );
    }
    
    //find animation
    model_instance_animation *model_instance_writelock::findAnimation( const char *cname )
    {
        return this->t->findAnimation( cname );
    }
    
    //play animation (returns animation instance id)
    dpid model_instance_writelock::playAnimation( dpid id, model_readlock *ml, const char *cname, bool bDoRepeat, float speed )
    {
        return this->t->playAnimation( id, ml, cname, bDoRepeat, speed );
    }
    
    //play animation (returns animation instance id)
    dpid model_instance_writelock::playAnimation( dpid id, model_writelock *ml, const char *cname, bool bDoRepeat, float speed )
    {
        return this->t->playAnimation( id, ml, cname, bDoRepeat, speed );
    }
    
    //stop animation
    void model_instance_writelock::stopAnimation( const char *cname )
    {
        this->t->stopAnimation( cname );
    }
    
    //stop animation
    void model_instance_writelock::stopAnimation( dpid id )
    {
        this->t->stopAnimation( id );
    }
    
    //returns true if animation is playing
    bool model_instance_writelock::isAnimationPlaying( const char *cname )
    {
        return this->t->isAnimationPlaying( cname );
    }
    
    //returns true if animation is playing
    bool model_instance_writelock::isAnimationPlaying( dpid id )
    {
        return this->t->isAnimationPlaying( id );
    }
    
    //change animation repeat
    void model_instance_writelock::setAnimationRepeat( const char *cname, bool bDoRepeat )
    {
        this->t->setAnimationRepeat( cname, bDoRepeat );
    }
    
    //change animation repeat
    void model_instance_writelock::setAnimationRepeat( dpid id, bool bDoRepeat )
    {
        this->t->setAnimationRepeat( id, bDoRepeat );
    }
    
    //change animation speed
    void model_instance_writelock::setAnimationSpeed( const char *cname, float s )
    {
        this->t->setAnimationSpeed( cname, s );
    }
    
    //change animation speed
    void model_instance_writelock::setAnimationSpeed( dpid id, float s )
    {
        this->t->setAnimationSpeed( id, s );
    }
    
    //stop all animations
    void model_instance_writelock::stopAllAnimations( void )
    {
        this->t->stopAllAnimations();
    }
    
    //get animations
    void model_instance_writelock::getAnimations( std::list<model_instance_animation *> *l )
    {
        this->t->getAnimations( l );
    }
    
    //get joints
    void model_instance_writelock::getJoints( std::list<model_instance_joint *> *l )
    {
        this->t->getJoints( l );
    }
    
    //get start time
    uint64_t model_instance_writelock::getStartTime( void )
    {
        return this->t->getStartTime();
    }
    
    //get end time
    uint64_t model_instance_writelock::getEndTime( void )
    {
        return this->t->getEndTime();
    }
    
    //set position
    void model_instance_writelock::setGuiPosition( float x, float y )
    {
        this->t->setGuiPosition( x, y );
    }
    
    //set width and height
    void model_instance_writelock::setGuiSize( float w, float h )
    {
        this->t->setGuiSize( w, h );
    }
    
    //get dimensions
    model_gui_pos *model_instance_writelock::getGuiDimensions( void )
    {
        return this->t->getGuiDimensions();
    }
    
    //set gui mode
    void model_instance_writelock::setGuiMode( bool b )
    {
        this->t->setGuiMode( b );
    }
    
    //returns true if gui
    bool model_instance_writelock::isGui( void )
    {
        return this->t->isGui();
    }

    //get position
    void model_instance_writelock::getPosition( dpposition *p )
    {
        this->t->getPosition( p );
    }
    
    //set position
    void model_instance_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }
  
    //returns true if alive
    bool model_instance_writelock::isAlive( void )
    {
        return this->t->isAlive();
    }
    
    //kill instance
    void model_instance_writelock::kill( void )
    {
        this->t->kill();
    }
    
    //clone animations
    void model_instance_writelock::cloneAnimations( model_instance_ref *mi )
    {
        this->t->cloneAnimations( mi );
    }
    
};
