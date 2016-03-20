#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	myfont = new ofTrueTypeFont;
	//myfont->loadFont(OF_TTF_SANS, 20);
	myfont->loadFont("AvalonB.ttf", 40);
	//gui->setFont("AvalonB.ttf", 20);

	qsiImage = new ofImage("Qsi.png");
	da4fidImage = new ofImage("DA4FID.png");
	noSmokeImage = new ofImage("no_smoking.png");

	ofSetBackgroundColor(ofColor(0, 0, 0));
	ofSetColor(ofColor::lightGray);

#ifndef NO_OMX
	
	string videoPath = ofToDataPath("/home/pi/Timecoded_Big_bunny_1.mov", true);
	
	//Somewhat like ofFboSettings we may have a lot of options so this is the current model
	ofxOMXPlayerSettings settings;
	settings.videoPath = videoPath;
	settings.useHDMIForAudio = true;	//default true
	settings.enableTexture = true;		//default true
	settings.enableLooping = true;		//default true
	settings.enableAudio = true;		//default true, save resources by disabling
	//settings.doFlipTexture = true;		//default false
	
	
	//so either pass in the settings
	omxPlayer.setup(settings);

#endif
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofDrawBitmapString("Hello World", 10, 10);
	myfont->drawString("Hang Nadim Batam Airport", 250, 50);

	ofSetColor(255);


	da4fidImage->draw(10, 10, 100, 100); //scale

	ofSetColor(ofColor::lightGray);

	ofDrawRectRounded(10, 160, 300, 100, 10);

	ofSetColor(ofColor(40, 40, 40));

	ofDrawRectRounded(10, 280, 300, 100, 10);

	ofSetColor(ofColor::lightGray);

	ofDrawRectRounded(10, 400, 300, 100, 10);


	ofSetColor(ofColor::lightGray);
	//int position = (ofGetFrameNum()  30);
	int position = ofGetViewportWidth() - ofGetFrameNum();

	myfont->drawString("Test of scrolling text for drawing with new positions every 10 frames", position, ofGetViewportHeight() - 60);

	ofSetColor(255);
	noSmokeImage->draw(10, ofGetViewportHeight() - 120, 100, 80); //scale

#ifndef NO_OMX


	if(!omxPlayer.isTextureEnabled())
	{
		return;
	}
	
	//omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	//draw a smaller version in the lower right
	int scaledHeight	= omxPlayer.getHeight()/3;
	int scaledWidth		= omxPlayer.getWidth()/3;
	omxPlayer.draw(ofGetWidth()-scaledWidth, ofGetHeight()-scaledHeight, scaledWidth, scaledHeight);

#endif
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
