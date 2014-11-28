#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"

#define M_PI 3.1415926535897932384626433832795

class nvrNode {

    public:
        nvrNode(int width, int height);
        virtual ~nvrNode();
        void update();
        void draw(int x, int y);
		void mousePressed(int x, int y, int button);

        int width;
        int height;

        stringstream reportStream;
    
        ofFbo bufferInput;
        ofFbo bufferOutput;
        ofShader mappingShader;

        int clickCount;
        ofPoint inputCorners[5];
        ofPoint outputCorners[5];
        ofVideoGrabber videoGrabber;
        ofMatrix4x4 mappingMatrix;
};
