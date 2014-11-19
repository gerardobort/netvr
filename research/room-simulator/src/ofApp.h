#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"

#define M_PI 3.1415926535897932384626433832795
#define NUM_CAMERAS 1
#define ROOM_W 300
#define ROOM_H 100
#define ROOM_D 200

#define VIEW_COLS 3

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();

	    void drawView(ofCamera* camera, int x, int y, int w, int h);
	    void drawScene();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        ofCamera cameras[NUM_CAMERAS];
};
