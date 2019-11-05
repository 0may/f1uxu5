#include "ofApp.h"

ofApp::ofApp(int w, int h) {
	camWidth = w;
	camHeight = h;
}



//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("f1uxu5");
	ofHideCursor();

	cout << "* Trying to setup camera with size " << camWidth << "x" << camHeight << endl;
	setupCamera();
	cout << "* Initializing graphics elements" << endl;
	setupGFX();
	cout << "* Initializing GUI" << endl;
	setupGUI(false);
	cout << "* Initializing Websockets" << endl;
	setupWebsocket();

	updateDisplayOffsets();
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(0);
#ifndef __arm__
	vidGrabber.update();
#endif

}



void ofApp::draw() {
	if (displayOffsetX < 0) {
		updateDisplayOffsets();
	}

	drawOffset();

	if (!hFlip && !vFlip)
		fboDisp->draw(displayOffsetX, displayOffsetY, ofGetWidth()-2*displayOffsetX, ofGetHeight()-2*displayOffsetY);
	else if (hFlip && !vFlip)
		fboDisp->draw(ofGetWidth()-displayOffsetX, displayOffsetY, -ofGetWidth()+2*displayOffsetX, ofGetHeight()-2*displayOffsetY);
	else if (!hFlip && vFlip)
		fboDisp->draw(displayOffsetX, ofGetHeight()-displayOffsetY, ofGetWidth()-2*displayOffsetX, -ofGetHeight()+2*displayOffsetY);
	else
		fboDisp->draw(ofGetWidth()-displayOffsetX, ofGetHeight()-displayOffsetY, -ofGetWidth()+2*displayOffsetX, -ofGetHeight()+2*displayOffsetY);

	if (showGui) {
		gui.draw();

		char strFps[32];
		sprintf(strFps, "FPS: %f", ofGetFrameRate());
		ofDrawBitmapString(ofGetFrameRate(), 20, ofGetHeight() - 20);
	}
}



//--------------------------------------------------------------
void ofApp::drawOffset() {

	float oX, oY, offX, offY;
	float a;
	ofFbo* fboTmp;

	ofSetColor(255);

	if (hFlip)
		oX = -offsetX;
	else
		oX = offsetX;

	if (vFlip)
		oY = offsetY;
	else
		oY = -offsetY;


	fboOut->begin();

		ofClear(0, 255);

//		vidGrabber.getTextureReference().bind();

		shader.begin();

			shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

			shader.setUniform1f("alpha", alpha);
			shader.setUniform2f("offset", glm::vec2(oX*camWidth*0.15, oY*camHeight*0.15));

			shader.setUniformTexture("tex0", vidGrabber.getTextureReference(), vidGrabber.getTextureID());
			shader.setUniformTexture("texFbo", fboDisp->getTextureReference(), vidGrabber.getTextureID()+1);

#ifdef __arm__
			vidGrabber.draw(0, 0, camWidth, camHeight);
#else
			ofDrawRectangle(0, 0, camWidth, camHeight);
#endif
		shader.end();

//		vidGrabber.getTextureReference().unbind();


	fboOut->end();



	for (int i = 1; i < repetitions; i++) {

		a = i / (float)repetitions * 2 * PI;

		offX = oX * cos(a) - oY * sin(a);
		offY = oX * sin(a) + oY * cos(a);


		fboBuf1->begin();

			ofClear(0, 255);
		//	vidGrabber.getTextureReference().bind();

			shader.begin();

				shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

				shader.setUniform1f("alpha", alpha);
				shader.setUniform2f("offset", glm::vec2(offX*camWidth*0.15, offY*camHeight*0.15));

				shader.setUniformTexture("tex0", vidGrabber.getTextureReference(), vidGrabber.getTextureID());
				shader.setUniformTexture("texFbo", fboDisp->getTextureReference(), vidGrabber.getTextureID()+1);

#ifdef __arm__
				vidGrabber.draw(0, 0, camWidth, camHeight);
#else
				ofDrawRectangle(0, 0, camWidth, camHeight);
#endif
			shader.end();

		//	vidGrabber.getTextureReference().unbind();

		fboBuf1->end();


		fboBuf2->begin();

			ofClear(0, 255);
		//	fboBuf1->getTextureReference().bind();

			shader.begin();

				shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

				shader.setUniform1f("alpha", 1.0);
				shader.setUniform2f("offset", glm::vec2(0, 0));

				shader.setUniformTexture("tex0", fboBuf1->getTextureReference(), 0);
				shader.setUniformTexture("texFbo", fboOut->getTextureReference(), 1);

#ifdef __arm__
				fboBuf1->draw(0, 0, camWidth, camHeight);
#else
				ofDrawRectangle(0, 0, camWidth, camHeight);
#endif
			shader.end();

		//	fboBuf1->getTextureReference().unbind();

		fboBuf2->end();

		fboTmp = fboBuf2;
		fboBuf2 = fboOut;
		fboOut = fboTmp;

	}


	fboTmp = fboDisp;
	fboDisp = fboOut;
	fboOut = fboTmp;

}


void ofApp::setupCamera() {

#ifdef __arm__
	camSettings.sensorWidth = camWidth;
	camSettings.sensorHeight = camHeight;
	camSettings.framerate = 30;
	camSettings.enableTexture = true;
	vidGrabber.setup(camSettings);
#else
	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(camWidth, camHeight);
#endif

	camHeight = vidGrabber.getHeight();
	camWidth = vidGrabber.getWidth();

	cout << "** Camera resolution is: " << camWidth << "x" << camHeight << endl;

}


void ofApp::setupGFX() {
	fboDisp = new ofFbo();
	fboDisp->allocate(camWidth, camHeight);
	fboDisp->begin();
	ofClear(0, 255);
	fboDisp->end();

	fboOut = new ofFbo();
	fboOut->allocate(camWidth, camHeight);
	fboOut->begin();
	ofClear(0, 255);
	fboOut->end();

	fboBuf1 = new ofFbo();
	fboBuf1->allocate(camWidth, camHeight);
	fboBuf1->begin();
	ofClear(0, 255);
	fboBuf1->end();

	fboBuf2 = new ofFbo();
	fboBuf2->allocate(camWidth, camHeight);
	fboBuf2->begin();
	ofClear(0, 255);
	fboBuf2->end();


	cout << "** Loading shader... " << (shader.load("shader.vert", "shader.frag") ? "SUCCESS" : "FAILED") << endl;

	ofSetVerticalSync(true);

	ofEnableArbTex();
	ofDisableNormalizedTexCoords();

}


void ofApp::setupGUI(bool show) {
	gui.setup();

	vFlip.set(false);
	vFlip.setName("vertical flip");
	gui.add(vFlip);

	hFlip.set(true);
	hFlip.setName("horizonatl flip");
	gui.add(hFlip);

	alpha.set(0.95);
	alpha.setMin(0.9);
	alpha.setMax(1.0);
	alpha.setName("alpha");
	gui.add(alpha);

	offsetX.set(0.0);
	offsetX.setMin(-1.0);
	offsetX.setMax(1.0);
	offsetX.setName("offset x");
	gui.add(offsetX);

	offsetY.set(0.0);
	offsetY.setMin(-1.0);
	offsetY.setMax(1.0);
	offsetY.setName("offset y");
	gui.add(offsetY);

	repetitions.set(1);
	repetitions.setMin(1);
	repetitions.setMax(21);
	repetitions.setName("repetitions");
	gui.add(repetitions);

	showGui = show;
}


void ofApp::setupWebsocket() {

	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
	options.port = 9092;
	options.bUseSSL = false; 

	cout << "** Websocket setup... " << (websock.setup(options) ? "SUCCESS" : "FAILED") << endl;

	websock.addListener(this);
}


void ofApp::updateDisplayOffsets() {

	int w = ofGetWidth();
	int h = ofGetHeight();

	float aspectDisp = w/(float)h;
	float aspectCam = camWidth/(float)camHeight;

	if (aspectDisp == aspectCam) {
		displayOffsetX = 0;
		displayOffsetY = 0;
	}
	else if (aspectDisp > aspectCam) {
		displayOffsetY = 0;
		displayOffsetX = (int)((w - h*aspectCam)*0.5 + 0.5);
	}
	else {
		displayOffsetX = 0;
		displayOffsetY = (int)((h - w/aspectCam)*0.5 + 0.5);
	}
}


//--------------------------------------------------------------
void ofApp::onMessage(ofxLibwebsockets::Event& args) {

	if (args.message.find("aa:") != string::npos) {
		alpha = (float)atof(args.message.substr(3).c_str());
	}
	else if (args.message.find("ox:") != string::npos) {
		offsetX = (float)atof(args.message.substr(3).c_str());
	}
	else if (args.message.find("oy:") != string::npos) {
		offsetY = (float)atof(args.message.substr(3).c_str());
	}
	else if (args.message.find("rp:") != string::npos) {
		repetitions = (int)atoi(args.message.substr(3).c_str());
	}
	else if (args.message.find("fh:") != string::npos) {
		hFlip = (bool)atoi(args.message.substr(3).c_str());
	}
	else if (args.message.find("fv:") != string::npos) {
		vFlip = (bool)atoi(args.message.substr(3).c_str());
	}
}

//--------------------------------------------------------------
void ofApp::onConnect(ofxLibwebsockets::Event& args) {}
void ofApp::onOpen(ofxLibwebsockets::Event& args) {}
void ofApp::onClose(ofxLibwebsockets::Event& args) {}
void ofApp::onIdle(ofxLibwebsockets::Event& args) {}
void ofApp::onBroadcast(ofxLibwebsockets::Event& args) {}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#ifndef __arm__
	if (key == 's' || key == 'S') {
		vidGrabber.videoSettings();
	}
#endif

	if (key == 'u' || key == 'U') {
		showGui = !showGui;
		if (showGui)
			ofShowCursor();
		else
			ofHideCursor();
	}

	if (key == 'v' || key == 'V') {
		vFlip = !vFlip;
	}

	if (key == 'h' || key == 'H') {
		vFlip = !vFlip;
	}

	if (key == 'f' || key == 'F') {
		ofToggleFullscreen();

		displayOffsetX = -1;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


