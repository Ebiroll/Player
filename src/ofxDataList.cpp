#include "ofxDataList.h"
#include <cctype>
#include <sstream>
#include <streambuf>

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

	setPosition(x, y,w,h);
    setTopRow(0);
    setLeftCol(0);
    selectRow(0);
    setActive(true);
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
void ofxDataList::setPosition(int x, int y,int w, int h)
{
    this->x = x;
    this->y = y;
    width=w; 
    height=h;
    cellWidth = width / numDisplayCols;  
	cellHeight = height / (numDisplayRows + 1);

}

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
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
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

        if (linesForDisplay.size() > j) {
            ofDrawRectRounded(0,  cellHeight * j , width, height, 10);
            ofSetColor(255);
            //ucFont.drawString(linesForDisplay[j], 60, 190 + height * j);
        }
    }
    ofTranslate(0, cellHeight);
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            float cy1 = (r + 1) * cellHeight;
            float cx = c * cellWidth;
            ofSetColor(255);
            ofNoFill();
            ofDrawBitmapString(ofToString(entries[topRow + r][leftCol + c]), cx + 1, cy + cellHeight - 1);
        }
    }

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
    
    ofPopMatrix();
    ofPopStyle();
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
