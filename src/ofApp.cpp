#include "ofApp.h"
#include "ofxTextLabel.h"


//--------------------------------------------------------------
void ofApp::setup(){
	myfont = new ofTrueTypeFont;
	//myfont->loadFont(OF_TTF_SANS, 20);
	myfont->loadFont("AvalonB.ttf", 40);
	font.loadFont("AvalonB.ttf", 40);
	//gui->setFont("AvalonB.ttf", 20);

	qsiImage = new ofImage("Qsi.png");
	da4fidImage = new ofImage("DA4FID2.png");
	noSmokeImage = new ofImage("no_smoking.png");

	ofSetBackgroundColor(ofColor(0, 0, 0));
	ofSetColor(ofColor::lightGray);

	////// Datasources
	if (!datasourcesXML.load("datasources.xml")) {
		std::cout << "Unable to load datasources file\n";
	};
	datasourcesXML.setTo("properties");

	char Buff[128];
	int numProps = datasourcesXML.getNumChildren("entry");
	numProps = 10;

	sprintf(Buff, "entry");
	//std::cout << Buff;
	datasourcesXML.setTo(Buff);

	while (datasourcesXML.setToSibling())
	{
           // data.url   file:traindata.xml
           // data2.url /traindata?station=SUND&orig=TRAIN&count=4
           // data2.root DataList
           // data2.row schedules
           // data2.columns platform : tcode : scheduledep : destination
           // data2.status_attributes status1

		//entry/key
		std::string value = datasourcesXML.getAttribute("key");
		if (value == "collector.address") {
			std::string data = datasourcesXML.getValue();
			std::cout << value << " " << data << "\r\n";
		}
		
	}
	//ofXml traindataXML;

	//// Playlist
	if (!playlistXML.load("playlist.xml")) {
		std::cout << "Unable to load playlist file\n";
	};
	playlistXML.setTo("loop");


       float frameWidth = 600;
       float frameHeight = 400;
       frameBounds.set((ofGetWidth() - frameWidth) / 2.0f, (ofGetHeight() - frameHeight) / 2.0f, frameWidth, frameHeight);
       lineSpacing = 1.0f;

#ifndef NO_OMX
	
	string videoPath = ofToDataPath("/home/pi/Timecoded_Big_bunny_1.mov", true);
	
	//Somewhat like ofFboSettings we may have a lot of options so this is the current model
	ofxOMXPlayerSettings settings;
	settings.videoPath = videoPath;
	settings.useHDMIForAudio = true;	//default true
	settings.enableTexture = true;		//default true
	settings.enableLooping = true;		//default true
	settings.enableAudio = true;		//default true, save resources by disabling
	//settings.doFlipTexture = true;	//default false
	
	
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

	std::string sampleText = "Here is some sample text:\nThe quick brown fox jumped over the lazy dog.\nThe End";

ofRectangle textBounds;
    vector<string> textLines;


    ofxTextLabel::drawString(font, sampleText, frameBounds, textLines, textBounds, lineSpacing, alignHorz, alignVert);
	
}

//--------------------------------------------------------------

void ofApp::newResponse(ofxHttpResponse & response) {
	string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
	std::cout << responseStr;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   switch (key) {
        case 'h':
            if (alignHorz == OF_ALIGN_HORZ_LEFT) alignHorz = OF_ALIGN_HORZ_CENTER;
            else if (alignHorz == OF_ALIGN_HORZ_CENTER) alignHorz = OF_ALIGN_HORZ_RIGHT;
            else alignHorz = OF_ALIGN_HORZ_LEFT;
            break;
            
        case 'v':
            if (alignVert == OF_ALIGN_VERT_TOP) alignVert = OF_ALIGN_VERT_CENTER;
            else if (alignVert == OF_ALIGN_VERT_CENTER) alignVert = OF_ALIGN_VERT_BOTTOM;
            else alignVert = OF_ALIGN_VERT_TOP;
            break;
            
        case 'l':
            lineSpacing += 0.1f;
            if (lineSpacing > 2.0f) lineSpacing = 0.2f;
            break;
            
        case 'f':
            frameBounds.width += 50;
            if (frameBounds.width > 800) frameBounds.width = 100;
            break;
		case 'r':
		    {
			   ofxHttpForm form;
			   form.action = "http://localhost/of-test/upload.php";
			   form.method = OFX_HTTP_POST;
			   //form.addFormField("number", ofToString(counter));
			   //form.addFile("file", "ofw-logo.gif");
			   httpUtils.addForm(form);
			   //requestStr = "message sent: " + ofToString(counter);
			   //counter++;
		    }
			break;
            
        default:
            break;
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
