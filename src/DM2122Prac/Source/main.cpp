//! Framework file, no modifications have been made by me

#include "Application.h"

int main( void )
{
	music::player.init();
	Application app;
	app.Init();
	app.Run();
	app.Exit();
}