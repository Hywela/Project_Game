#pragma once
#include "Window.h"


 
int main(int argc, char** argv) {
	//Open the main window
	Window *mainWindow = new Window();

	//Start window activity
	mainWindow->login();

	//Grab state
	int mainWindowState = mainWindow->getWindowState();

	//Delete main window
	delete mainWindow;

	//Return state of the main window
	return mainWindowState;
}