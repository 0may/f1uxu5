#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxLibwebsockets.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg); 


		// websocket methods
		void onConnect(ofxLibwebsockets::Event& args);
		void onOpen(ofxLibwebsockets::Event& args);
		void onClose(ofxLibwebsockets::Event& args);
		void onIdle(ofxLibwebsockets::Event& args);
		void onMessage(ofxLibwebsockets::Event& args);
		void onBroadcast(ofxLibwebsockets::Event& args);

	protected:
		void drawOffset();

		void setupCamera();
		void setupGFX();
		void setupGUI(bool show = true);
		void setupWebsocket();





	protected:
		ofVideoGrabber vidGrabber;

		ofShader shader;

		ofFbo* fboDisp;
		ofFbo* fboOut;
		ofFbo* fboBuf1;
		ofFbo* fboBuf2;

		int camWidth;
		int camHeight;

		ofxLibwebsockets::Server websock;


		bool showGui;
		ofxPanel gui;
		ofParameter<bool> vFlip;
		ofParameter<bool> hFlip;
		ofParameter<float> alpha;
		ofParameter<float> offsetX;
		ofParameter<float> offsetY;
		ofParameter<int> repetitions;


		
};
