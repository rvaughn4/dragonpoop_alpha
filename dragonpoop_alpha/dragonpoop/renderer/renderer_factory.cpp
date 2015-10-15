
#include "renderer_factory.h"

namespace dragonpoop
{
    
    //ctor
    renderer_factory::renderer_factory( const char *cname, int score )
    {
        this->score = score;
        this->bWastested = 0;
        this->bWorks = 0;
        this->sname.assign( cname );
    }
    
    //dtor
    renderer_factory::~renderer_factory( void )
    {
        
    }
    
    //returns true if works
    bool renderer_factory::works( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        if( !this->bWastested )
        {
            this->bWorks = this->testRenderer( c, g, tp );
            this->bWastested = 1;
        }
        return this->bWorks;
    }
    
    //get name
    std::string *renderer_factory::getName( void )
    {
        return &this->sname;
    }
    
    //compare name
    bool renderer_factory::compareName( std::string *s )
    {
        return this->sname.compare( *s ) == 0;
    }
    
    //generate renderer
    renderer *renderer_factory::makeRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        if( !this->bWorks )
            return 0;
        return this->genRenderer( c, g, tp );
    }

    //return score
    int renderer_factory::getScore( void )
    {
        return this->score;
    }
    
};

