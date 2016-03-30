#pragma once

#include "ofMain.h"
#include "ofxHttpUtils.h"

#define NO_OMX


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
		
		// Here we get the response
		void newResponse(ofxHttpResponse & response);

		ofxHttpUtils httpUtils;


		ofTrueTypeFont *myfont;
		ofImage *qsiImage;

		ofImage *noSmokeImage;
		ofImage *da4fidImage;

		ofXml datasourcesXML;
		ofXml traindataXML;
		ofXml playlistXML;

#ifndef NO_OMX
                ofxOMXPlayer omxPlayer;
#endif

		std::string root;
		std::vector<std::string> columns;
		ofRectangle frameBounds;
                float lineSpacing;
		
		ofTrueTypeFont font;
                ofAlignHorz alignHorz;
                ofAlignVert alignVert;
};
