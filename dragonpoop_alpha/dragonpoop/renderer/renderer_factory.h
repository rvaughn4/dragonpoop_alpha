
#ifndef dragonpoop_renderer_factory_h
#define dragonpoop_renderer_factory_h

#include <string>

namespace dragonpoop
{
    
    class renderer;
    class core;
    class gfx_writelock;
    class dptaskpool_writelock;
    
    class renderer_factory
    {
        
    private:
        
        bool bWastested, bWorks;
        std::string sname;
        int score;
        
    protected:
        
        //generate renderer
        virtual renderer *genRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) = 0;
        //test renderer
        virtual bool testRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) = 0;
        
    public:
        
        //ctor
        renderer_factory( const char *cname, int score );
        //dtor
        virtual ~renderer_factory( void );
        //returns true if works
        bool works( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //get name
        std::string *getName( void );
        //compare name
        bool compareName( std::string *s );
        //generate renderer
        renderer *makeRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //return score
        int getScore( void );
        
    };
    
};

#endif