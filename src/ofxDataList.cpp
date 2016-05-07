#include "ofxDataList.h"
#include <cctype>
#include <sstream>
#include <streambuf>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timestamp.h>

//--------------------------------------------------------------
ofxDataList::ofxDataList()
{
    cellWidth = 100;
    cellHeight = 18;
}
//--------------------------------------------------------------
void ofxDataList::setup(int x, int y, int w , int h,int numDisplayRows, int numDisplayCols)
{
    
    this->numDisplayRows = numDisplayRows;
    this->numDisplayCols = numDisplayCols;
	width = w;
	height = h;

	cellWidth = width / numDisplayCols;  
	cellHeight = height / (numDisplayRows + 1);

    font.setup("AvalonB.ttf", 1.0, 1024, false, 8, 1.0);
	setPosition(x, y,w,h);
    setTopRow(0);
    setLeftCol(0);
    selectRow(0);
    setActive(true);


    timer.setup( 5000 ) ;
    timer.start( true ) ;
    
    ofAddListener( timer.TIMER_COMPLETE , this, &ofxDataList::timerCompleteHandler ) ;
    
    
  
    ofAddListener(httpUtils.newResponseEvent, this, &ofxDataList::newResponse);
	httpUtils.start();
    
    {
    		ofxHttpForm form;
		form.action = "http://127.0.0.1:3000/airportdata";
		form.method = OFX_HTTP_GET;
		httpUtils.addForm(form);
    }
    

}

void ofxDataList::timerCompleteHandler( int &args )
{
    std::cout << "TIMER" << std::endl;
    std::cout << topRow << std::endl;
    std::cout << getNumberOfEntries() << std::endl;
    
    if (topRow < getNumberOfEntries()) {
        topRow+= numDisplayRows;
        if (topRow>=getNumberOfEntries()) {
           topRow=0; 
           // New data request
	   {
	     ofxHttpForm form;
	     form.action = "http://127.0.0.1:3000/airportdata";
	     form.method = OFX_HTTP_GET;
	     httpUtils.addForm(form);
	   }

        }
        content_updated=true;
    }
    else
    {
        topRow=0; 
    }    
 }


//--------------------------------------------------------------
void ofxDataList::newResponse(ofxHttpResponse & response) {
	string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
	std::cout << responseStr;
	//printf("%s\n", response.responseBody.c_str());
    //return;

	if (XML.loadFromBuffer(response.responseBody))
	{
		clearEntries();
		//XML.setTo("ResponseOfDepartures"); // change relative root to <feed>
		//XML.setTo("ResponseData"); // change relative root to <feed>
		//XML.setTo("Trains");
		//XML.setTo("//ResponseOfDepartures[0]//ResponseData[0]//Trains");
		//XML.setTo("//ResponseOfDepartures//ResponseData//Trains");
		//XML.setTo(root);
		XML.setTo("FlightDataList");

        vector<string> entry;

		//int numEntries = XML.getNumChildren("Train");
		// iterate through <entry> tags
    	//XML.setTo(row);
		XML.setTo("FlightData"); 
   		string flight = XML.getValue("Flight");
		string value = XML.getValue("Destination");
		string time = XML.getValue("CTime");
		string gate = XML.getValue("Gate");

        entry.clear();
   	    entry.push_back(value);
	    entry.push_back(flight);
	    entry.push_back(time);
		entry.push_back(gate);

   	    addEntry(entry);


	while (XML.setToSibling()) {
        XML.setTo("FlightData"); 
		string flight = XML.getValue("Flight");
		string value = XML.getValue("Destination");
		string time = XML.getValue("Ctime");
		string gate = XML.getValue("Gate");

		//string timeFormat = "%H:%M";
		//std::string tmp=Poco::DateTimeFormatter::format(Poco::Timestamp(strtoll(time.c_str(), NULL, 10)), timeFormat);


        entry.clear();
		entry.push_back(value);
		entry.push_back(flight);
		entry.push_back(time);
		entry.push_back(gate);

   	    addEntry(entry);
     }  


      content_updated=true;
	  //string  test = string("Västerhaninge");
	  //linesForDisplay.push_back(test);


	}
}

//--------------------------------------------------------------
void ofxDataList::sortByCol(int col,bool direction)
{
    if (direction) {
        sort(entries.begin(), entries.end(), [&col](const vector<string> &a, const vector<string> &b){
            if (std::isdigit(ofToChar(a[col])) && std::isdigit(ofToChar(b[col]))) {
                return ofToFloat(a[col]) > ofToFloat(b[col]);
            }
            else {
                return a[col] > b[col];
            }
        });
    }
    else {
        sort(entries.begin(), entries.end(), [&col](const vector<string> &a, const vector<string> &b){
            if (std::isdigit(ofToChar(a[col])) && std::isdigit(ofToChar(b[col]))) {
                return ofToFloat(a[col]) < ofToFloat(b[col]);
            }
            else {
                return a[col] < b[col];
            }
        });
    }
}
//--------------------------------------------------------------
void ofxDataList::setActive(bool active)
{
    this->active = active;
    if (active) {
        ofAddListener(ofEvents().keyPressed, this, &ofxDataList::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &ofxDataList::keyReleased);
        ofAddListener(ofEvents().mousePressed, this, &ofxDataList::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &ofxDataList::mouseReleased);
        ofAddListener(ofEvents().mouseDragged, this, &ofxDataList::mouseDragged);
    }
    else {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxDataList::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxDataList::keyReleased);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxDataList::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxDataList::mouseReleased);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxDataList::mouseDragged);
    }
}
//--------------------------------------------------------------
// This is called by update
void ofxDataList::setPosition(int x, int y,int w, int h)
{
    timer.update( ) ;
    
    this->x = x;
    this->y = y;
    width=w; 
    height=h;
    cellWidth = width / numDisplayCols;  
	cellHeight = height / (numDisplayRows + 1);
    
    
   if (_fboSettings.width!=width || 
    _fboSettings.height!=height) {
        _fboSettings.width=width;
        _fboSettings.height=height;
        _fboSettings.internalformat = GL_RGBA;
        if (_fbo.isAllocated()) {
            _fbo.clear();
        }
        _fbo.allocate(_fboSettings);        
        content_updated=true;
    }
    
};

struct tokens : std::ctype<char>
{
	tokens() : std::ctype<char>(get_table()) {}

	static std::ctype_base::mask const* get_table()
	{
		typedef std::ctype<char> cctype;
		static const cctype::mask *const_rc = cctype::classic_table();

		static cctype::mask rc[cctype::table_size];
		std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

		rc[':'] = std::ctype_base::space;
		rc[' '] = std::ctype_base::space;
		return &rc[0];
	}
};

void ofxDataList::setHeaderByCommalist(std::string commaseparatedlist) {
	std::string s = commaseparatedlist;
	std::stringstream ss(s);
	ss.imbue(std::locale(std::locale(), new tokens()));
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	setHeaders(vstrings);

	numDisplayCols = vstrings.size();
	cellWidth = width / numDisplayCols;


	//std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}


//--------------------------------------------------------------
void ofxDataList::setHeaders(vector<string> headers)
{
    this->headers = headers;
}
//--------------------------------------------------------------
void ofxDataList::addEntry(vector<string> entry)
{
    entries.push_back(entry);
    vector<float> entryF;
    for (auto e : entry) {
        entryF.push_back(ofToFloat(e));
    }
    entriesF.push_back(entryF);
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxDataList::addEntry(vector<float> entry)
{
    entriesF.push_back(entry);
    vector<string> entryS;
    for (auto e : entry) {
        entryS.push_back(ofToString(e));
    }
    entries.push_back(entryS);
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxDataList::deleteSelectedRow()
{
    if (entries.size() == 0) return;
    entries.erase(entries.begin() + min((int) entries.size()-1, selectedRow));
    entriesF.erase(entriesF.begin() + min((int) entriesF.size()-1, selectedRow));
    selectRow(min(selectedRow, (int) entries.size()-1));
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxDataList::clearEntries()
{
    entries.clear();
    entriesF.clear();
    ofNotifyEvent(changeEvent);
}
//--------------------------------------------------------------
void ofxDataList::draw()
{
    int numRows = min(numDisplayRows, (int) entries.size() - topRow);
    int numCols = min(numDisplayCols, (int) headers.size() - leftCol);
    
    //ofPushStyle();
    //ofPushMatrix();
    //ofTranslate(x, y);
    if (content_updated) {
        _fbo.clear();
        _fbo.allocate(_fboSettings);        

        _fbo.begin();
        {
        
            for (int j = 0; j < numRows; j++) {
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

                if (numRows+1 > j) {
                    ofDrawRectRounded(0,  cellHeight * j , width, cellHeight, 10);
                    ofSetColor(255);
                    //ucFont.drawString(linesForDisplay[j], 60, 190 + height * j);
                }
            }
            //ofTranslate(0, cellHeight);
            for (int r = 0; r < numRows; r++) {
                for (int c = 0; c < numCols; c++) {
                    float cy1 = (r + 1) * cellHeight;
                    float cx = c * cellWidth;
                    float cy = r * cellHeight;
                    
		    if (r % 2 == 0) {
		      //ofSetColor(ofColor(40, 40, 40));
		      ofSetColor(255);
		     } else {
		      ofSetColor(0);
		     }
                    //ofNoFill();
                    float px=cx + 8;
                    float py= cy + cellHeight/2 - 1;
                    float fsize=20;
                    const std::string textString=ofToString(entries[topRow + r][leftCol + c]);
                    //ofSetColor(55);

                    font.draw(textString,fsize,px,py);
                    //ofDrawBitmapString(ofToString(entries[topRow + r][leftCol + c]), cx + 1, cy + cellHeight - 1);
                }
            }
        }
        _fbo.end();
        content_updated=false;
    }
    
    _fbo.draw(x,y);
    // draw header
    /*
    for (int c = 0; c < numCols; c++) {
        float cx = c * cellWidth;
        ofFill();
        ofSetColor(55);
        ofDrawRectangle(cx, 0, cellWidth, cellHeight);
        ofSetColor(110);
        ofNoFill();
        ofDrawRectangle(cx, 0, cellWidth, cellHeight);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(headers[leftCol + c]), cx + 1, cellHeight - 1);
    }
    */
    // draw cells
    /*
    ofTranslate(0, cellHeight);
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            float cy = r * cellHeight;
            float cy1 = (r + 1) * cellHeight;
            float cx = c * cellWidth;
            ofFill();
            ofSetColor(topRow + r == selectedRow ? 210 : 255);
            ofDrawRectangle(cx, cy, cellWidth, cellHeight);
            ofSetColor(170);
            ofNoFill();
            ofDrawRectangle(cx, cy, cellWidth, cellHeight);
            ofSetColor(0);
            ofDrawBitmapString(ofToString(entries[topRow + r][leftCol + c]), cx + 1, cy + cellHeight - 1);

            ofFill();
            ofSetColor(55);
            ofDrawRectangle(-20, cy, 20, cellHeight);
            ofSetColor(110);
            ofNoFill();
            ofDrawRectangle(-20, cy, 20, cellHeight);
            ofSetColor(255);
            ofDrawBitmapString(ofToString(topRow+ r+1), -20, cy1-3);
        }
    }
    */
    
    //ofPopMatrix();
    //ofPopStyle();
}
//--------------------------------------------------------------
void ofxDataList::keyPressed(ofKeyEventArgs &evt)
{
    if (evt.key == OF_KEY_UP) {
        scrollUp();
    }
    else if (evt.key == OF_KEY_DOWN) {
        scrollDown();
    }
    else if (evt.key == OF_KEY_LEFT) {
        scrollLeft();
    }
    else if (evt.key == OF_KEY_RIGHT) {
        scrollRight();
    }
    else if (evt.key == OF_KEY_BACKSPACE) {
        deleteSelectedRow();
    }
    else if (evt.key == OF_KEY_SHIFT) {
        shift = true;
    }
}
//--------------------------------------------------------------
void ofxDataList::keyReleased(ofKeyEventArgs &evt)
{
    if (evt.key == OF_KEY_SHIFT) {
        shift = false;
    }
}
//--------------------------------------------------------------
void ofxDataList::mousePressed(ofMouseEventArgs &evt)
{
    if (ofRectangle(x, y, width, height).inside(evt.x, evt.y)) {
        int idxY = floor((evt.y - y - cellHeight) / cellHeight);
        int idyX = floor((evt.x - x - cellWidth) / cellWidth)+1;
        selectRow(topRow + idxY);
        
        // Left Click : Right Click
        sortByCol(leftCol+ idyX, evt.button);
    }
}
//--------------------------------------------------------------
void ofxDataList::mouseDragged(ofMouseEventArgs &evt)
{
    
}
//--------------------------------------------------------------
void ofxDataList::mouseReleased(ofMouseEventArgs &evt)
{
    
}
//--------------------------------------------------------------
void ofxDataList::scrollUp()
{
    selectRow(max(0, selectedRow - (shift ? numDisplayRows-1 : 1)));
    if (selectedRow < topRow) {
        setTopRow(selectedRow);
    }
}
//--------------------------------------------------------------
void ofxDataList::scrollDown()
{
    selectRow(min((int) entries.size()-1, selectedRow + (shift ? numDisplayRows-1: 1)));
    
    if (selectedRow >= topRow + numDisplayRows) {
        setTopRow(topRow + (shift ? numDisplayRows-1: 1));
    }
}
//--------------------------------------------------------------
void ofxDataList::scrollLeft()
{
    if (leftCol > 0) {
        setLeftCol(max(0, leftCol - (shift ? numDisplayCols-1 : 1)));
    }
}
//--------------------------------------------------------------
void ofxDataList::scrollRight()
{
    setLeftCol(min((int)headers.size() - numDisplayCols, leftCol + (shift ? numDisplayRows-1 : 1)));
}
