#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"

#define M_PI 3.1415926535897932384626433832795

class ofApp : public ofBaseApp{

    public:
        void setup();
        void draw();
    
        ofVbo vbo;
        ofEasyCam camera;
        ofImage image;
};
