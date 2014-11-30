#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "ofxFlowTools.h"
#include "nvrNode.h"

#define M_PI 3.1415926535897932384626433832795
#define INT_NODES_AMOUNT 2
#define INT_ROOM_WIDTH 800
#define INT_ROOM_HEIGHT 200
#define INT_ROOM_DEPTH 600

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void drawProjections();
        void drawRoom();
        void drawOpticalFlow();
        void exit();
		void mouseDragged(int x, int y, int button);

        void listCameraDevices();

        nvrNode *nodes[INT_NODES_AMOUNT];
        ofFbo bufferProjections;
        ofFbo bufferFlow;
        ofFbo bufferRoom;
        ofEasyCam roomCamera;
        ofShader roomMapping;

        float lastTime;
        float deltaTime;

        flowTools::ftOpticalFlow opticalFlow;
        flowTools::ftDisplayScalar displayScalar;
        flowTools::ftVelocityField velocityField;

        bool didCamsUpdate;
};
