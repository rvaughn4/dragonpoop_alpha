
#ifndef dragonpoop_window_h
#define dragonpoop_window_h

namespace dragonpoop
{
    
    class window
    {
        
    private:
        
    protected:
        
    public:
        

        //ctor
        window( float w, float h, const char *ctitle );
        //dtor
        virtual ~window( void );
        //run window
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        //returns window width
        virtual float getWidth( void );
        //returns window height
        virtual float getHeight( void );
        
    };
    
};

#endif