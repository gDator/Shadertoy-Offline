#include "Global.hpp"
#include "ProgramStateMain.hpp"
#include <iostream>


int main()
{
    // initLog();  
    
    //DEB(lg, trace) << "A trace severity message";
    //DEB(lg, debug) << "A debug severity message";
    //DEB(lg, info) << "An informational severity message";
    //DEB(lg, warning) << "A warning severity message";
    //DEB(lg, error) << "An error severity message";
    //DEB(lg, fatal) << "A fatal severity message";
    app.init();
    app.pushState(new xgl::program::ProgramStateTest());
    app.loop();
    return 0;
}
