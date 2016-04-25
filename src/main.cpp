#include "ofMain.h"
#include "ofApp.h"
#include <stdlib.h> 
//========================================================================
int main( ){
	ofSetupOpenGL(1280,1024, OF_FULLSCREEN);			// <-------- setup the GL context  1920,1080

	system("/opt/vc/bin/tvservice -j -m CEA > data/modes.json");

	// To set mode,
	// tvservice -e "CEA 4"
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
