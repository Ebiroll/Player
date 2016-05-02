//
//  ofxScrollingText.h
//
//  Created by Teruaki Tsubokura on 2014/03/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxFontStash.h"

class ofxScrollingText{
public:
    static ofxScrollingText* instance();
    void setup(string _fontPath, int _fontSize, int x, int y, int w, int h);
    void update(int x,int y,int w,int h);
    void draw();
    
    void showText(string _message);
    void hideText();
    bool getVisible();
    
    void setFontColor(ofColor _color);
    ofColor getFontColor();
    
    void setBackgroundColor(ofColor _color);
    ofColor getBackgroundColor();

    void setSpeed(float _speed);
    float getSpeed();
    
    // complete event.
    static ofEvent<void> completeEvent;

    ofxScrollingText();
    ~ofxScrollingText();

    bool isPlaying;
    
private:
    // private instance
    //static ofxScrollingText* mIns;
    
    ofxFontStash font;
    int fontsize;
    //ofxTrueTypeFontUC font;
    bool isVisible;
    bool isScroll;
    float speed;
    float alpha;
    ofColor fontColor;
    ofColor bgColor;
    string textString;
    ofPoint textPosition;
	int x;
	int y;

    int width;
    int height;
};
