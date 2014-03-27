/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Source.cpp
--
-- PROGRAM: WinAsgn2
--
-- FUNCTIONS:
-- int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
--
-- DATE: February 03, 2014
--
-- REVISIONS:   
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- NOTES:
-- This Program acts as either a Server or a Client. 
-- When the program executes, the user is able to choose either client or server mode and select the port.
-- Receive Mode:
--		The Server can be placed into either TCP or UDP mode. When in Receive mode
-- Send Mode:
--		The Client can be placed into either TCP or UDP mode. The user is able to choose the packet size (multiples of 1 KB)
--		and the number of packets to send.  The user will be able to enter the ip address and the name of the server.
--
-- Remember to add "WS2_32.Lib" to the project source list.
--
----------------------------------------------------------------------------------------------------------------------*/
#include "MyForm.h"
#include <stdlib.h>
#include "Variables.h"

[STAThreadAttribute]

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MyForm_Load
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
--				HINSTANCE hInstance: Instance of the program 
--				HINSTANCE hPrevInstance: Previous instance of the program
--				LPSTR lpCmdLine: String of cmd lines
--				int nShowCmd: command number
--
-- RETURNS: -
--
-- NOTES:
-- This function starts the Window Forms.
------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application::EnableVisualStyles();
	Application::Run(gcnew MyForm());

}