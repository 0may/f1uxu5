#include "ofApp.h"

ofApp::ofApp(ofxXmlSettings& settings) {
	this->settings = settings;
	camWidth = settings.getValue("f1uxu5:camWidth", 640);
	camHeight = settings.getValue("f1uxu5:camHeight", 480);
}



//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("f1uxu5");
	ofHideCursor();

	setupCamera();
	setupGFX();
	setupGUI(false);
	setupWebsocket();
	cout << endl;

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

		shader.begin();

			shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

			shader.setUniform1f("alpha", alpha);
			shader.setUniform2f("offset", glm::vec2(oX*camWidth*0.15, oY*camHeight*0.15));

#ifdef __arm__
			shader.setUniformTexture("tex0", vidGrabber.getTextureReference(), vidGrabber.getTextureID());
			shader.setUniformTexture("texFbo", fboDisp->getTexture(), vidGrabber.getTextureID()+1);
#else
			shader.setUniformTexture("tex0", vidGrabber.getTexture(), 0);
			shader.setUniformTexture("texFbo", fboDisp->getTexture(), 1);
#endif
			vidGrabber.draw(0, 0, camWidth, camHeight);

		shader.end();


	fboOut->end();



	for (int i = 1; i < repetitions; i++) {

		a = i / (float)repetitions * 2 * PI;

		offX = oX * cos(a) - oY * sin(a);
		offY = oX * sin(a) + oY * cos(a);


		fboBuf1->begin();

			ofClear(0, 255);

			shader.begin();

				shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

				shader.setUniform1f("alpha", alpha);
				shader.setUniform2f("offset", glm::vec2(offX*camWidth*0.15, offY*camHeight*0.15));

#ifdef __arm__
				shader.setUniformTexture("tex0", vidGrabber.getTextureReference(), vidGrabber.getTextureID());
				shader.setUniformTexture("texFbo", fboDisp->getTexture(), vidGrabber.getTextureID()+1);
#else
				shader.setUniformTexture("tex0", vidGrabber.getTexture(), 0);
				shader.setUniformTexture("texFbo", fboDisp->getTexture(), 1);
#endif

				vidGrabber.draw(0, 0, camWidth, camHeight);

			shader.end();

		fboBuf1->end();



		fboBuf2->begin();

			ofClear(0, 255);

			shader.begin();

				shader.setUniform2f("resolution", glm::vec2(camWidth, camHeight));

				shader.setUniform1f("alpha", 1.0);
				shader.setUniform2f("offset", glm::vec2(0, 0));

				shader.setUniformTexture("tex0", fboBuf1->getTexture(), 0);
				shader.setUniformTexture("texFbo", fboOut->getTexture(), 1);

				fboBuf1->draw(0, 0, camWidth, camHeight);

			shader.end();

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

	cout << "\nSetting up camera" << endl;
	cout << "- Specified camera resolution: " << camWidth << "x" << camHeight << endl;

#ifdef __arm__
    cout << "- Available options for RPi camera:" << endl;
	cout << "  - White Balance names:   ";
	vector<string> names = OMX_Maps::getInstance().getWhiteBalanceNames();
	for (int i = 0; i < names.size(); i++) {
		cout << names[i] << " ";
	}
	cout << endl;

	cout << "  - Metering Type names:   ";
	names = OMX_Maps::getInstance().meteringNames;
	for (int i = 0; i < names.size(); i++) {
		cout << names[i] << " ";
	}
	cout << endl;

	cout << "  - Exposure Preset names: ";
	names = OMX_Maps::getInstance().getExposurePresetNames();
	for (int i = 0; i < names.size(); i++) {
		cout << names[i] << " ";
	}
	cout << endl << endl;

	camSettings.sensorWidth = camWidth;
	camSettings.sensorHeight = camHeight;
	camSettings.framerate = 30;
	camSettings.enableTexture = true;
	camSettings.whiteBalance = settings.getValue("f1uxu5:whiteBalance", "Flash");
	camSettings.meteringType = settings.getValue("f1uxu5:meteringType", "Average");
	camSettings.exposurePreset = settings.getValue("f1uxu5:exposurePreset", "FixedFps");
	camSettings.autoShutter = true;
	camSettings.shutterSpeed = 0;
	camSettings.autoISO = false;
	camSettings.ISO = settings.getValue("f1uxu5:iso", 200);

	vidGrabber.setup(camSettings);

	cout << "- Waiting for camera to be ready..." << endl;

	while (!vidGrabber.isReady());

	cout << "- Selected options for RPi camera: " << endl;
	cout << "  - White Balance:   " << vidGrabber.getWhiteBalance() << endl;
	cout << "  - Exposure Preset: " << vidGrabber.getExposurePreset() << endl;
	cout << "  - Metering Type:   " << vidGrabber.getMeteringType() << endl;
	cout << "  - ISO:             " << vidGrabber.getISO() << endl;
//	cout << "  - Shutter Speed:   " << vidGrabber.getShutterSpeed() << endl;
#else
	cout << "- Available camera devices:" << endl;

	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			cout << "  - " << devices[i].id << ": " << devices[i].deviceName << endl;
		}
		else {
			cout << "  - " << devices[i].id << ": " << devices[i].deviceName << " - unavailable" << endl;
		}
	}

	cout << "- Selected camera device: " << settings.getValue("f1uxu5:camDeviceID", 0) << endl;
	vidGrabber.setDeviceID(settings.getValue("f1uxu5:camDeviceID", 0));
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(camWidth, camHeight);
#endif

	camHeight = vidGrabber.getHeight();
	camWidth = vidGrabber.getWidth();

	cout << "- Final camera resolution: " << camWidth << "x" << camHeight << endl;

}


void ofApp::setupGFX() {

	cout << "\nSetting up graphics elements" << endl;

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

#ifdef __arm__
	cout << "- Loading shader: " 
	     << (shader.load("shader.rpi.vert", "shader.rpi.frag") ? "SUCCESS" : "FAILED") 
    	<< endl;
#else
	cout << "- Loading shader: " 
	     << (shader.load("shader.i386.vert", "shader.i386.frag") ? "SUCCESS" : "FAILED") 
    	<< endl;
#endif

	ofSetVerticalSync(true);

	ofEnableArbTex();
	ofDisableNormalizedTexCoords();

}


void ofApp::setupGUI(bool show) {

	cout << "\nSetting up GUI" << endl;

	gui.setup();

	vFlip.set(settings.getValue("f1uxu5:flipV", 0) != 0);
	vFlip.setName("vertical flip");
	gui.add(vFlip);

	hFlip.set(settings.getValue("f1uxu5:flipH", 1) != 0);
	hFlip.setName("horizontal flip");
	gui.add(hFlip);

	alpha.set(settings.getValue("f1uxu5:alpha", 0.95));
	alpha.setMin(0.9);
	alpha.setMax(1.0);
	alpha.setName("alpha");
	gui.add(alpha);

	offsetX.set(settings.getValue("f1uxu5:offsetX", 0.0));
	offsetX.setMin(-1.0);
	offsetX.setMax(1.0);
	offsetX.setName("offset x");
	gui.add(offsetX);

	offsetY.set(settings.getValue("f1uxu5:offsetY", 0.0));
	offsetY.setMin(-1.0);
	offsetY.setMax(1.0);
	offsetY.setName("offset y");
	gui.add(offsetY);

	repetitions.set(settings.getValue("f1uxu5:repetitions", 1));
	repetitions.setMin(1);
	repetitions.setMax(15);
	repetitions.setName("repetitions");
	gui.add(repetitions);

	showGui = show;
}


void ofApp::setupWebsocket() {

	cout << "\nSetting up Websockets" << endl;

	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
	options.port = settings.getValue("f1uxu5:websocketsPort", 9092);
	options.bUseSSL = false; 

	cout << "- Websocket setup at port " << options.port << ": " << (websock.setup(options) ? "SUCCESS" : "FAILED") << endl;

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


