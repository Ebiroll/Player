#pragma once

#include "ofMain.h"


class ofxDataList
{
public:
	ofxDataList();
    void setup(int x, int y, int w, int h, int numDisplayRows, int numDisplayCols);

	//void setURL(std::string uri);
	void setHeaderByCommalist(std::string commaseparatedlist);
    
    void setPosition(int x, int y,int w, int h);

    void setActive(bool active);
    bool getActive() {return active;}

    // direction true = ascending : false = descending
    void sortByCol(int col,bool direction);
    
    void setHeaders(vector<string> headers);
    void addEntry(vector<string> entry);
    void addEntry(vector<float> entry);
    
    void clearEntries();
    
    int getNumberOfEntries() { return entries.size(); }
    vector<vector<float> > & getEntries() { return entriesF; }
    vector<vector<string> > & getEntriesS() { return entries; }
    vector<string> & getHeaders() { return headers; }
    
    void draw();
    
    template<typename ListenerClass, typename ListenerMethod>
    void addSpreadsheetChangedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(changeEvent, listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void removeSpreadsheetChangedListener(ListenerClass *listener, ListenerMethod method) {
        ofRemoveListener(changeEvent, listener, method);
    }

private:
    void deleteSelectedRow();

    void setTopRow(int topRow) {this->topRow = topRow;}
    void setLeftCol(int leftCol) {this->leftCol = leftCol;}
    void selectRow(int selectedRow) {this->selectedRow = selectedRow;}
    void selectCol(int selectedCol) {this->selectedCol = selectedCol;}
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();

    void keyPressed(ofKeyEventArgs &evt);
    void keyReleased(ofKeyEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    bool active;
    bool shift;
    int x, y, width, height;
    int numDisplayRows;
    int numDisplayCols;
    int cellWidth;
    int cellHeight;
    int topRow;
    int leftCol;
    int selectedRow;
    int selectedCol;
    
    vector<string> headers;
    vector<vector<string> > entries;
    vector<vector<float> > entriesF;
    
    //ofFbo _fbo;
    //ofFbo::Settings _fboSettings;
    //ofColor _clearColor;
    
    ofEvent<void> changeEvent;
};

