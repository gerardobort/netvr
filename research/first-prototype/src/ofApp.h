#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "ofxFlowTools.h"
#include "nvrNode.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

#define M_PI 3.1415926535897932384626433832795
#define INT_NODES_AMOUNT 6
#define INT_ROOM_WIDTH 800
#define INT_ROOM_HEIGHT 200
#define INT_ROOM_DEPTH 600

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void drawCameras();
        void drawProjections();
        void drawRoom();
        void exit();
		void mouseDragged(int x, int y, int button);

        void listCameraDevices();
        void keyPressed(int key);

        stringstream reportStream;
        ofShader mappingShader;

        nvrNode *nodes[INT_NODES_AMOUNT];
        ofFbo bufferRoom;
        ofEasyCam roomCamera;
        ofShader roomMapping;

        float lastTime;
        float deltaTime;

        int activeCamera;

        // GUI
        ofxPanel			gui;
        void				setupGui();
        ofParameter<int>	guipNodesAmount;
	    ofParameterGroup	prototypeParameters;


        ofxAssimpModelLoader model;
};
