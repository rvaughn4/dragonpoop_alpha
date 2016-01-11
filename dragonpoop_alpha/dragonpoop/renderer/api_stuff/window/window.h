
#ifndef dragonpoop_window_h
#define dragonpoop_window_h

#include <string>

namespace dragonpoop
{

    class input_passer_ref;
    class input_passer_writelock;

    class window
    {

    private:

        input_passer_ref *ip;

    protected:

    public:


        //ctor
        window( float w, float h, const char *ctitle, input_passer_writelock *ip );
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
        //adds mouse input
        void addMouseInput( uint64_t t, int x, int y, bool bLb, bool bRb, bool bDown );
        //adds kb input
        void addKBInput( uint64_t t, std::string *skey, bool bDown );

    };

};

#endif
