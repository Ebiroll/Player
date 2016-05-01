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
///////////////////////////////////////////////////

#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

std::string listAllIF() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    std::string ret;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
	    if (std::string(addressBuffer)!=std::string("127.0.0.1")) {
  	        ret = ret + " " + std::string(addressBuffer);
	    }
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return ret;
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
	ofSetupOpenGL(1280,1024, OF_FULLSCREEN);			// <-------- setup the GL context  1920,1080
	// TV
	system("/opt/vc/bin/tvservice -j -m CEA > data/modes_CEA.json");
	// Monitor
	system("/opt/vc/bin/tvservice -j -m DMT > data/modes_DMT.json");

	// Current setting
	system("/opt/vc/bin/tvservice -s > data/current.txt");
	
	load_current_config();
	std::string add=listAllIF();
	strcpy(config.r[scroll].text,add.c_str());

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
