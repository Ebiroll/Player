#include "ofMain.h"
#include "ofApp.h"
#include <stdlib.h> 
std::string newMode;

void setNewMode(const char *mode) {
	//system("/opt/vc/bin/tvservice -e ");
	char Buffer[512];
	
	sprintf(Buffer,"%s",mode);
	newMode=std::string(Buffer);
}

void run() {


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:


}


//========================================================================
int main( ){
	ofSetupOpenGL(1280,1024, OF_FULLSCREEN);			// <-------- setup the GL context  1920,1080
	// TV
	system("/opt/vc/bin/tvservice -j -m CEA > data/modes_CEA.json");
	// Monitor
	system("/opt/vc/bin/tvservice -j -m DMT > data/modes_DMT.json");

	// Current setting
	system("/opt/vc/bin/tvservice -s > data/current.txt");

        ofRunApp(new ofApp());

	// To set mode,
	// tvservice -e "CEA 4"
	//run();
	// To exit call.... ofExit().
	// To cleanup add methis,
	// void ofBaseApp::exit()
	//system("/opt/vc/bin/tvservice -e ");
	if (newMode.length()>0) {
		char Buffer[512];
		
		sprintf(Buffer,"/opt/vc/bin/tvservice -e \"%s\"",newMode.c_str());
		printf("Calling %s",Buffer);
	} 
		
}
