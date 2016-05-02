#include "ofApp.h"
#include "ofxTextLabel.h"
#include "config.h"

bool doLoadNextMovie = false;

unsigned short     gPort=10010;


//ofApp()::~ofApp()
//{
//}

void ofApp::onScrolltextFinnish()
{
	scrollingText.showText(config.r[scroll].text);
}

std::string text = "exploding text";

//--------------------------------------------------------------
void ofApp::setup(){	
    ofSetVerticalSync(true);
    ofHideCursor();
    ofTrueTypeFont::setGlobalDpi(72);
    myfont = new ofTrueTypeFont;
    //ucFont.loadFont("AvalonB.ttf", 20, true, true);
	//myfont->loadFont(OF_TTF_SANS, 20);
	myfont->loadFont("AvalonB.ttf", 32);
	font.setup("AvalonB.ttf", 1.0, 1024, false, 8, 1.0);
	//gui->setFont("AvalonB.ttf", 20);

    //xplodingString = new ofxExplodingString( &font, ofColor(255) );

	ofRectangle bound(0,ofGetViewportHeight()-120,1920*1,32);
	
	//scrollText.setup(myfont,"Hello FBO Font, can you scroll or is all hope lost? Maybe we have a faster scoller here. Just in case there is some misunderstanding I would like to inform you that this line is long like silly",bound);
	scrollingText.setup("AvalonB.ttf",28,config.r[scroll].x, config.r[scroll].y+(config.r[scroll].h/2),config.r[scroll].w,config.r[scroll].h);
	scrollingText.showText(config.r[scroll].text);
	scrollingText.setSpeed(4.0);

	dataList.setup(config.r[datalist].x, config.r[datalist].y, config.r[datalist].w, config.r[datalist].h, 3, 3);

	//dataList.setup(10, 100, 400, 400, 3, 3);

	vector<string> entry;
	entry.push_back("AFL 216");
	entry.push_back("12:10");
	entry.push_back("Jakarta");
    entry.push_back("???");

	dataList.addEntry(entry);
	entry.clear();
	entry.push_back("KLM 666");
	entry.push_back("12:20");
	entry.push_back("Stockholm");
    entry.push_back("???");

	dataList.addEntry(entry);
	entry.clear();
	entry.push_back("SAS 216");
	entry.push_back("12:30");
	entry.push_back("Rome");
    entry.push_back("???");
	
	dataList.addEntry(entry);
	entry.clear();
	entry.push_back("AFL 212");
	entry.push_back("12:40");
	entry.push_back("Sydney");
    entry.push_back("???");	
	dataList.addEntry(entry);

	entry.push_back("SLM 212");
	entry.push_back("12:40");
	entry.push_back("Slumtown");
    entry.push_back("???");	
	dataList.addEntry(entry);





	ofAddListener(scrollingText.completeEvent, this, &ofApp::onScrolltextFinnish);

	qsiImage = new ofImage("Qsi.png");
	da4fidImage = new ofImage("DA4FID2.png");
	noSmokeImage = new ofImage("no_smoking.png");

	ofSetBackgroundColor(ofColor(0, 0, 0));
	//ofSetBackgroundAuto(false);
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

		if (value == "data2.columns") {
			columnames = datasourcesXML.getValue();
			std::cout << "columns:" << columnames << "\r\n";
		}

		


	}
	//ofXml traindataXML;
	dataList.setHeaderByCommalist(columnames);

	//// Playlist
	if (!playlistXML.load("playlist.xml")) {
		std::cout << "Unable to load playlist file\n";
	}
	else
	{
		std::cout << "Loaded playlist file!!!!!!!!!!!!!\n";
	  
		playlistXML.setTo("loop");
		playlistXML.setTo("item");
		playlistXML.setTo("media");
		std::string value = playlistXML.getAttribute("name");
		std::cout << "value" << value << std::endl;
		
		ofFile file(value);
		files.push_back(file);
		std::string full = playlistXML.getAttribute("fullscreen");

		if (full == "true") {
			fullScreen.push_back(true);
		}
		else
		{
			fullScreen.push_back(false);
		}

		playlistXML.setToParent();
		while (playlistXML.setToSibling()) {
	  		playlistXML.setTo("media");
			std::string value = playlistXML.getAttribute("name");
			ofFile file(value);
			//std::cout << value;
			
			files.push_back(file);
			std::string full = playlistXML.getAttribute("fullscreen");

			if (full == "true") {
				fullScreen.push_back(true);
			}
			else
			{
				fullScreen.push_back(false);
			}
			playlistXML.setToParent();
		}
	}

  	   // /da4rid/viewer/adverts/


       float frameWidth = 600;
       float frameHeight = 400;
       frameBounds.set((ofGetWidth() - frameWidth) / 2.0f, (ofGetHeight() - frameHeight) / 2.0f, frameWidth, frameHeight);
       lineSpacing = 1.0f;

	   ofAddListener(httpUtils.newResponseEvent, this, &ofApp::newResponse);
	   httpUtils.start();

	   ofAddListener(httpUtils.newResponseEvent, this, &ofApp::videoResponse);
	   //videoHttpUtils.start();


#ifndef NO_OMX
	
	string videoPath = ofToDataPath("/home/pi/Timecoded_Big_bunny_1.mov", true);
	
	//Somewhat like ofFboSettings we may have a lot of options so this is the current model
	ofxOMXPlayerSettings settings;
	settings.videoPath = files[videoCounter].path(); //videoPath;
        cout << "Load--------" << settings.videoPath << std::endl;
	
	settings.listener = this;
	settings.useHDMIForAudio = true;	//default true
	settings.enableTexture = true;		//default true
	settings.enableLooping = false;		//default true	
	settings.enableAudio = false;		//default true, save resources by disabling
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
	
	// Setup list box
    _fboSettings.width = ofGetWidth()/3;
    _fboSettings.height = 2*ofGetHeight()/3;
    _fboSettings.internalformat = GL_RGBA;
    _fbo.allocate(_fboSettings);

   config.width=ofGetWidth();
   config.height=ofGetHeight();

	
    textDataUpdated=false;

      
    fServerSocket=new  ServerSocket(gPort);
    // set-up a HTTPServer instance
    fServer= new HTTPServer(new RequestHandlerFactory, *fServerSocket, new HTTPServerParams);
    
    if (fServer) fServer->start();

    if (fServer) printf("Server Start ...................\n");
    
}

//--------------------------------------------------------------
void ofApp::update(){
	if (config.config_updated==1) {
		if (!scrollingText.isPlaying) {
    		scrollingText.showText(config.r[scroll].text);
		}
		config.config_updated=0;
	}
	
    scrollingText.update(config.r[scroll].x, config.r[scroll].y+(config.r[scroll].h/2),config.r[scroll].w,config.r[scroll].h);
	//scrollText.update();
    dataList.setPosition(config.r[datalist].x, config.r[datalist].y, config.r[datalist].w, config.r[datalist].h);

    //xplodingString->update(0.01666);
	
	
#ifndef NO_OMX
	if (doLoadNextMovie) 
	{
		ofLogVerbose(__func__) << "doing reload";
		
		if(omxPlayer.isTextureEnabled())
		{
			//clear the texture if you want
			//omxPlayer.getTextureReference().clear();
		}
		//with the texture based player this must be done here - especially if the videos are different resolutions
		loadNextMovie();
	}
#endif
	   if (textDataUpdated) {
	     	int width = 0.9 * ofGetViewportWidth() / 3;
         	int height = ofGetViewportHeight() / 8;
    _fbo.begin();
    {
        //ofClear(_clearColor.r, _clearColor.g, _clearColor.b, 0);
		//ofClear(0);
		ofClear(40,0);
		        
        ofPushStyle();
        ofSetColor(255);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		//ofEnableSmoothing();

 	    ofSetColor(ofColor::lightGray);
		
		for (int j = 0; j < 5; j++) {
			if (j % 2 == 0) {
  			  ofSetColor(ofColor(40, 40, 40));
			  //ofClear(40,0);
  			  //ofBackground(ofColor(40, 40, 40));
			}
			else
			{
			  ofSetColor(ofColor::darkGrey);
   			  //ofClear(255,0);
   			  //ofBackground(ofColor::lightGray);
			}
	
			if (linesForDisplay.size() > j) {
			  ofDrawRectRounded(10, 160 + height * j , width, height, 10);
			  ofSetColor(255);
			 // ucFont.drawString(linesForDisplay[j], 60, 190 + height * j);
			}
		}
        
        ofDisableAlphaBlending();
        ofPopStyle();
    }
    _fbo.end();

	textDataUpdated=false;   
   }
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofDrawBitmapString("Hello World", 10, 10);
        //static int times=0;
    float fontsize=40.0;
	float lx=config.r[label].x;
	float ly=config.r[label].y + (config.r[label].h/2);
    font.draw(config.r[label].text,fontsize,lx , ly);
	ofSetColor(255);

	da4fidImage->draw(config.r[logo].x, config.r[logo].y, config.r[logo].w, config.r[logo].h); //scale

	int width = 0.9 * ofGetViewportWidth() / 3;
	int height = ofGetViewportHeight() / 8;
	wWidth=ofGetViewportWidth();
	wHeight=ofGetViewportHeight();	

	float fsize=40.0;
        font.draw(config.r[explode].text,fsize, config.r[explode].x, config.r[explode].y);
	//xplodingString->draw();



	if (!fullScreen[videoCounter])
	{
		ofSetColor(255, 255, 255);
		dataList.draw();
	 	//_fbo.draw(0,0);
#if 0
 	    ofSetColor(ofColor::lightGray);
		
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
			  //ucFont.drawString(linesForDisplay[j], 60, 190 + height * j);
			}
		}
#endif
	}

	
	//int position = (ofGetFrameNum()  30);
	//int position = ofGetViewportWidth() - ofGetFrameNum();

    //scrollText.draw(position,0);
	scrollingText.draw();

	//myfont->drawString("Test of scrolling text for drawing with new positions every 10 frames", position, ofGetViewportHeight() - 60);

	ofSetColor(255);
	noSmokeImage->draw(config.r[no_smoke].x, config.r[no_smoke].y, config.r[no_smoke].w, config.r[no_smoke].h); //scale

#ifndef NO_OMX


	if(!omxPlayer.isTextureEnabled())
	{
		return;
	}
	
	//omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	//draw a smaller version in the lower right
	int scaledHeight	= 2*omxPlayer.getHeight()/3;
	int scaledWidth		= 2*omxPlayer.getWidth()/3;

	if (fullScreen[videoCounter])
        {
            omxPlayer.draw(0,0,ofGetWidth(), ofGetHeight());
	} else
        {
            omxPlayer.draw(config.r[video].x, config.r[video].y, config.r[video].w, config.r[video].h);	  	  
	}


#endif

	//std::string sampleText = "Here is some sample text:\nUnicode åäö ÅÄÖ The quick brown fox jumped over the lazy dog.\nThe End";
    //ofRectangle textBounds;
    //vector<string> textLines;
    //ofxTextLabel::drawString(font, sampleText, frameBounds, textLines, textBounds, lineSpacing, alignHorz, alignVert);
	//ucFont.drawString("ÅÄÖ åäö", 100, 200);


}

#ifndef NO_OMX



void ofApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
	ofLogVerbose(__func__) << " RECEIVED";
	doLoadNextMovie = true;
}


void ofApp::loadNextMovie()
{
	if (videoCounter + 1<files.size())
	{
		videoCounter++;
	}
	else
	{
		videoCounter = 0;
	}
	omxPlayer.loadMovie(files[videoCounter].path());
	doLoadNextMovie = false;
}

#endif

void ofApp::videoResponse(ofxHttpResponse & response) {
	// Video response
	// One file per line???
}


void ofApp::newResponse(ofxHttpResponse & response) {
	string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
	//std::cout << responseStr;
	//printf("%s\n", response.responseBody.c_str());

	if (XML.loadFromBuffer(response.responseBody))
	{
		linesForDisplay.clear();
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
		string value = XML.getValue("Destination");
		string dt = XML.getValue("DisplayTime");
		string total = value + string("\n    ") + dt;
		linesForDisplay.push_back(total);


		while (XML.setToSibling()) {
			value = XML.getValue("Destination");
			 dt = XML.getValue("DisplayTime");
			 total = value + string("\n   ") + dt;

			linesForDisplay.push_back(total);
			printf("%s\n", value.c_str());
			//printf("%s\n", XML.getValue("entry:published", "", i).c_str());
		}


      textDataUpdated=true;
	  //string  test = string("Västerhaninge");
	  //linesForDisplay.push_back(test);


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
			   //form.action = url;
			   form.action = "http://api.sl.se/api2/realtimedepartures.xml?key=9e99975c83864b6f8dab6ab4baddaba8&siteid=9702&timewindow=20";
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
