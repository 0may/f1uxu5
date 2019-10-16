#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetLogLevel(OF_LOG_SILENT);

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setPosition(glm::vec2(20, 50));
	settings.setSize(1280, 720);
	//settings.windowMode = OF_WINDOW;
	settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
