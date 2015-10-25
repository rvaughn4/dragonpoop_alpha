
#ifndef dragonpoop_window_h
#define dragonpoop_window_h

#include <queue>
#include <string>

namespace dragonpoop
{
    
    struct window_mouse_input
    {
        float x, y;
        bool lb, rb;
    };
    struct window_kb_input
    {
        std::string sname;
        bool bDown;
    };
    
    class window
    {
        
    private:
        
        std::queue<window_mouse_input> mse;
        std::queue<window_kb_input> kbe;
        
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
        //returns true if mouse input is waiting
        bool hasMouseInput( void );
        //fetches mouse input
        bool getMouseInput( window_mouse_input *m );
        //adds mouse input
        void addMouseInput( window_mouse_input *m );
        //returns true if kb input is waiting
        bool hasKBInput( void );
        //fetches kb input
        bool getKBInput( window_kb_input *m );
        //adds kb input
        void addKBInput( window_kb_input *m );
        
    };
    
};

#endif