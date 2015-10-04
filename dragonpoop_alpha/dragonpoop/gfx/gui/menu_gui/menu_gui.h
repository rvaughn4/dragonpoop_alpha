
#ifndef dragonpoop_menu_gui_h
#define dragonpoop_menu_gui_h

#include "../gui.h"
#include "../button_gui/button_gui.h"
#include <vector>

namespace dragonpoop
{
    
    struct menu_gui_btn
    {
        std::string s;
        button_gui *b;
    };
    
    class menu_gui : public gui
    {
        
    private:
        
        float last_y, sz, top_y;
        uint64_t t_last_poll;
        button_gui *btitle;
        std::vector<menu_gui_btn> btns;
        gfx_ref *g;
        
    protected:
        
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //add button
        void addButton( const char *btn_name );
        //remove button
        void removeButton( const char *btn_name );
        //remove all buttons
        void removeButtons( void );
        //recreate all buttons
        void redoButtons( void );
        
    public:
        
        //ctor
        menu_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, float bh, const char *txt );
        //dtor
        virtual ~menu_gui( void );
        
    };
    
};

#endif