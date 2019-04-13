#include "mouthus_core.h"
#include "mouthus_main_window.h"

#include <iostream>

#ifdef _MSC_VER
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);
    
    MouthusCore core;
    MouthusMainWindow window(&core);
    
    window.show();
    
    return app.exec();
}