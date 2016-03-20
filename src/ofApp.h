#pragma once

#include "ofMain.h"

#ifndef NO_OMX
#include "ofxOMXPlayer.h"
#endif

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
		

		ofTrueTypeFont *myfont;
		ofImage *qsiImage;

		ofImage *noSmokeImage;
		ofImage *da4fidImage;

#ifndef NO_OMX
                ofxOMXPlayer omxPlayer;
#endif

		
};
