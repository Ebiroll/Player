#pragma once

#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxTextLabelFbo.h"


//#define NO_OMX


#ifndef NO_OMX
#include "ofxOMXPlayer.h"
#endif

class ofApp : public ofBaseApp 
#ifndef NO_OMX
 , public ofxOMXPlayerListener
#endif
{

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

#ifndef NO_OMX
		void onVideoEnd(ofxOMXPlayerListenerEventData& e);
		void onVideoLoop(ofxOMXPlayerListenerEventData& e) { /*empty*/ };
		void loadNextMovie();

#endif

		// Here we get the response for the data xml.
		void newResponse(ofxHttpResponse & response);
		void videoResponse(ofxHttpResponse & response);

		ofxHttpUtils httpUtils;
		ofxHttpUtils videoHttpUtils;

		ofxTrueTypeFontUC ucFont;
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

		// Video loop
		vector<ofFile> files;
		vector<bool> fullScreen;
		int videoCounter;


        ofXml XML;
	std::string url;
        std::string root;
	std::string row;

        std::vector<std::string> columns;
	std::vector<std::string> linesForDisplay;


        ofRectangle frameBounds;
        float lineSpacing;
		
        ofTrueTypeFont font;
        ofAlignHorz alignHorz;
        ofAlignVert alignVert;
		
	int wWidth;
	int wHeight;
	
	bool textDataUpdated;
		
	// Use FBO to draw textlist
        ofFbo _fbo;
        ofFbo::Settings _fboSettings;
        ofColor _clearColor;

	ofxTextLabelFbo scrollText;
};
