#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"

#define M_PI 3.1415926535897932384626433832795

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();
		void mousePressed(int x, int y, int button);
		void mouseMoved(int x, int y );
    
        ofImage image;
        ofEasyCam camera;
        ofShader mappingShader;

        int clickCount;
        ofPoint distortedCorners[4];
        ofPoint originalCorners[4];
};
