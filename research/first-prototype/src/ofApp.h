#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "nvrNode.h"

#define M_PI 3.1415926535897932384626433832795
#define INT_NODES_AMOUNT 2
#define INT_ROOM_WIDTH 2000
#define INT_ROOM_DEPH 1000

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void drawProjections();
        void drawRoom();
        void exit();
		void mouseDragged(int x, int y, int button);

        void listCameraDevices();

        nvrNode *nodes[INT_NODES_AMOUNT];
        ofFbo bufferProjections;

        ofFbo bufferRoom;
        ofEasyCam roomCamera;
        ofShader roomMapping;
};
