#include "GUI.h"
using namespace std;

int main(int argc, char* argv[])
{
    GUI* gui = new GUI(argc,argv);
 
    while (gui->work_flow())
        SDL_Delay(20);

    return 0;
}








