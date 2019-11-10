#include "ofMain.h"
#include "ofApp.h"
#include "ofxXmlSettings.h"


//========================================================================
int main(int argc, char** argv ){

	int w = 640;
	int h = 480;
	ofxXmlSettings appSettings;

	if (appSettings.loadFile("f1uxu5-settings.xml")) {
		w = appSettings.getValue("f1uxu5:camWidth", 640);
		h = appSettings.getValue("f1uxu5:camHeight", 480);
	}
	else {
		cerr << "f1uxu5 ERROR: Could not find 'f1uxu5-settings.xml' file in 'bin/data' folder. Exiting!" << endl;
		return 1;
	}


	ofSetLogLevel(OF_LOG_SILENT);

#ifdef __arm__
	ofGLESWindowSettings windowSettings;
	windowSettings.setGLESVersion(2);
#else
	ofGLFWWindowSettings windowSettings;
	windowSettings.setGLVersion(3, 2);
#endif
	windowSettings.setPosition(glm::vec2(20, 50));
	windowSettings.setSize(w, h);
	//settings.windowMode = OF_WINDOW;
	windowSettings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(windowSettings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp(appSettings));

}
