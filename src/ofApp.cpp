#include "ofApp.h"
#include "ofxTextLabel.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myfont = new ofTrueTypeFont;
	ucFont.loadFont("AvalonB.ttf", 20, true, true);
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
		if (value == "data2.url") {
			url = datasourcesXML.getValue();
			std::cout << "url:" << url << "\r\n";
		}
		if (value == "data2.root") {
			root = datasourcesXML.getValue();
			std::cout << "root:" << root << "\r\n";
		}
		if (value == "data2.row") {
			row = datasourcesXML.getValue();
			std::cout << "row:" << row << "\r\n";
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

	   ofAddListener(httpUtils.newResponseEvent, this, &ofApp::newResponse);
	   httpUtils.start();
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
	
	if (strncmp(url.c_str(), "file:",5)==0) {
		ofxHttpResponse fake;
		string filename = url.substr(5);
		std::cout << "Opening " << filename << std::endl;

		std::ifstream file(filename.c_str());
		string content;

		istreambuf_iterator<char> inputIt(file), emptyInputIt;
		back_insert_iterator<string> stringInsert(content);

		copy(inputIt, emptyInputIt, stringInsert);
  	    //std::cout << content;
		fake.responseBody = content;
		newResponse(fake);
    }
	else 
	{
		ofxHttpForm form;
		form.action = url;
		form.method = OFX_HTTP_GET;
		httpUtils.addForm(form);
	}
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

	int width = 1 * ofGetViewportWidth() / 3;
	int height = ofGetViewportHeight() / 8;

	ofSetColor(ofColor::lightGray);
	//string  test = string("Västerhaninge");
	//linesForDisplay.push_back(test);

	for (int j = 0; j < 5; j++) {
		if (j % 2 == 0) {
			ofSetColor(ofColor(40, 40, 40));
		}
		else
		{
			ofSetColor(ofColor::lightGray);
		}

		if (linesForDisplay.size() > j) {
			ofDrawRectRounded(10, 160 + height * j , width, height, 10);
			ofSetColor(255);
			ucFont.drawString(linesForDisplay[j], 60, 190 + height * j);
		}
	}

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
	int scaledHeight	= 2*omxPlayer.getHeight()/3;
	int scaledWidth		= 2*omxPlayer.getWidth()/3;
	omxPlayer.draw(ofGetWidth()-scaledWidth, ofGetHeight()-scaledHeight, scaledWidth, scaledHeight);

#endif

	//std::string sampleText = "Here is some sample text:\nUnicode åäö ÅÄÖ The quick brown fox jumped over the lazy dog.\nThe End";
    //ofRectangle textBounds;
    //vector<string> textLines;
    //ofxTextLabel::drawString(font, sampleText, frameBounds, textLines, textBounds, lineSpacing, alignHorz, alignVert);
	//ucFont.drawString("ÅÄÖ åäö", 100, 200);


}


void ofApp::newResponse(ofxHttpResponse & response) {
	string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
	std::cout << responseStr;
	//printf("%s\n", response.responseBody.c_str());

	if (XML.loadFromBuffer(response.responseBody))
	{
		//XML.setTo("ResponseOfDepartures"); // change relative root to <feed>
		//XML.setTo("ResponseData"); // change relative root to <feed>
		//XML.setTo("Trains");
		//XML.setTo("//ResponseOfDepartures[0]//ResponseData[0]//Trains");
		//XML.setTo("//ResponseOfDepartures//ResponseData//Trains");
		XML.setTo(root);
		//XML.setTo("//ResponseOfDepartures//ResponseData//Buses");


		//int numEntries = XML.getNumChildren("Train");
		// iterate through <entry> tags
		//XML.setTo("Bus"); 
		XML.setTo(row);

		while (XML.setToSibling()) {
			string value = XML.getValue("destination");
			linesForDisplay.push_back(value);
			printf("%s\n", value.c_str());
			//printf("%s\n", XML.getValue("entry:published", "", i).c_str());
		}

	}
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
			   form.action = url;
			   //form.action = "http://localhost/of-test/upload.php";
			   form.method = OFX_HTTP_GET;
			   httpUtils.addForm(form);
			   //requestStr = "message sent: " + ofToString(counter);
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
