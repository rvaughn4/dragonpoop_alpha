
#ifndef dragonpoop_menu_gui_writelock_h
#define dragonpoop_menu_gui_writelock_h

#include "../gui_writelock.h"

namespace dragonpoop
{
    class menu_gui;
    
    class menu_gui_writelock : public gui_writelock
    {
        
    private:
        
        menu_gui *t;
        
    protected:
        
        //ctor
        menu_gui_writelock( menu_gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~menu_gui_writelock( void );
        
    public:
        
        //add button
        void addButton( const char *btn_name );
        //remove button
        void removeButton( const char *btn_name );
        //remove all buttons
        void removeButtons( void );
        //returns true if button was clicked
        bool wasClicked( const char *btn_name );
        
        friend class menu_gui;
    };
    
};

#endif