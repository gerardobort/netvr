#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"

#define M_PI 3.1415926535897932384626433832795

#define INT_CORNERS_AMOUNT 5

class nvrNode {

    public:
        nvrNode(int cameraWidth = 640, int cameraHeight = 480, int deviceId = 0);
        virtual ~nvrNode();
        void update();
        void draw(int x = 0, int y = 0, int width = 300, int height = 400);
		void mouseDragged(int x, int y, int button);

        int cameraWidth;
        int cameraHeight;

        int drawX;
        int drawY;
        int drawWidth;
        int drawHeight;

        stringstream reportStream;
    
        ofFbo bufferInput;
        ofFbo bufferOutput;
        ofShader mappingShader;

        ofPoint inputCorners[INT_CORNERS_AMOUNT];
        ofPoint outputCorners[INT_CORNERS_AMOUNT];
        ofVideoGrabber videoGrabber;
        ofMatrix4x4 mappingMatrix;
};
