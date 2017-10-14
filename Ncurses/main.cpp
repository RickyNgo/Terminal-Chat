#include "layout.hpp"
#include <unistd.h>

int main()
{
    UI main_win;
    main_win.windows_init();
    
    while(1)
    {   
        
        //main_win.content_update();
        main_win.resize();
        main_win.get_input();
        sleep(1);
    }

    main_win.delete_windows();
    return 0;

}