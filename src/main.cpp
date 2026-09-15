#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(1100, 750);
	settings.resizable = false;
	settings.doubleBuffering = true;
	settings.highResDisplay = false;
	ofCreateWindow(settings);

	ofRunApp(new ofApp());
}