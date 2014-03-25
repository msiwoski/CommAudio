#include "iostream"
#include "main_menu.h"
using namespace std;
using namespace TCPIP;

[STAThread] 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
        Application::EnableVisualStyles();
        Application::Run(gcnew main_menu());
}