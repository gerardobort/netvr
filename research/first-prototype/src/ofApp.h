#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "nvrNode.h"

#define M_PI 3.1415926535897932384626433832795

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();
		void mouseDragged(int x, int y, int button);
		void mouseMoved(int x, int y );

        void listCameraDevices();
    
        nvrNode *node1;
        nvrNode *node2;
        nvrNode *node3;
        nvrNode *node4;
};
