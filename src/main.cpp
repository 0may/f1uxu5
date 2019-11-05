#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char** argv ){

	int w = 640;
	int h = 480;

	if (argc == 3) {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}


	ofSetLogLevel(OF_LOG_SILENT);

#ifdef __arm__
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
#else
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
#endif
	settings.setPosition(glm::vec2(20, 50));
	settings.setSize(w, h);
	//settings.windowMode = OF_WINDOW;
	settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp(w, h));

}
