#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"

#define M_PI 3.1415926535897932384626433832795

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        ofImage image;
        ofEasyCam camera;
};
