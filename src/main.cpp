#include "ofMain.h"
#include "ofApp.h"
#include <stdlib.h>
//#include <linux/fb.h>


std::string newMode;

void setNewMode(const char *mode) {
	//system("/opt/vc/bin/tvservice -e ");
	char Buffer[512];
	
	sprintf(Buffer,"%s",mode);
	newMode=std::string(Buffer);
	ofExit();
}

void run() {


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:


}


//========================================================================
int main( ){

  /*
    This was a shot grom the hip... :-P
  int zero = 0;
  int fbdev = open("/dev/fb0", O_RDWR);  
  ioctl(fbdev, FBIO_WAITFORVSYNC, &zero);
  */
  

  {
	ofSetupOpenGL(1280,1024, OF_WINDOW);			// <-------- setup the GL context  1920,1080
	// TV
	system("/opt/vc/bin/tvservice -j -m CEA > data/modes_CEA.json");
	// Monitor
	system("/opt/vc/bin/tvservice -j -m DMT > data/modes_DMT.json");

	// Current setting
	system("/opt/vc/bin/tvservice -s > data/current.txt");
	
	load_current_config();

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
		system(Buffer);
	}
    }
		
}
