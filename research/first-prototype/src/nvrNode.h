#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "ofxFlowTools.h"

#define M_PI 3.1415926535897932384626433832795

#define INT_CORNERS_AMOUNT 5

#define INT_ROOM_WIDTH 800
#define INT_ROOM_HEIGHT 200
#define INT_ROOM_DEPTH 600

class nvrNode {

    public:
        nvrNode(int cameraWidth = 640, int cameraHeight = 480, int deviceId = 0, int number = 0);
        virtual ~nvrNode();
        void update();
        void draw(int x = 0, int y = 0, int width = 300, int height = 400);
		void mouseDragged(int x, int y, int button);

        int cameraWidth;
        int cameraHeight;
        int deviceId;
        int number;

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

        // FLOW
        ofFbo bufferFlow;
        float lastTime;
        float deltaTime;
        flowTools::ftOpticalFlow opticalFlow;
        flowTools::ftVelocityMask velocityMask;
        flowTools::ftFluidSimulation fluid;
        flowTools::ftVelocityField velocityField;
        void drawOpticalFlow();

        // GUI
        void				setupGui();
        void				updateValues();
	    ofParameterGroup	parameters;
	    ofParameterGroup	inputParameters;
	    ofParameterGroup	outputParameters;
        ofParameter<bool>	guipShowFlow;
        ofParameter<bool>	guipFlipCamsHorizontally;

        ofParameter<ofVec2f> guipInputCorners0;
	    void setInputCorners0(ofVec2f& _value) { inputCorners[0] = ofPoint(_value); }
        ofParameter<ofVec2f> guipInputCorners1;
	    void setInputCorners1(ofVec2f& _value) { inputCorners[1] = ofPoint(_value); }
        ofParameter<ofVec2f> guipInputCorners2;
	    void setInputCorners2(ofVec2f& _value) { inputCorners[2] = ofPoint(_value); }
        ofParameter<ofVec2f> guipInputCorners3;
	    void setInputCorners3(ofVec2f& _value) { inputCorners[3] = ofPoint(_value); }

        ofParameter<ofVec2f> guipOutputCorners0;
	    void setOutputCorners0(ofVec2f& _value) { outputCorners[0] = ofPoint(_value); }
        ofParameter<ofVec2f> guipOutputCorners1;
	    void setOutputCorners1(ofVec2f& _value) { outputCorners[1] = ofPoint(_value); }
        ofParameter<ofVec2f> guipOutputCorners2;
	    void setOutputCorners2(ofVec2f& _value) { outputCorners[2] = ofPoint(_value); }
        ofParameter<ofVec2f> guipOutputCorners3;
	    void setOutputCorners3(ofVec2f& _value) { outputCorners[3] = ofPoint(_value); }

        ofParameter<ofVec3f>	guipTargetPosition;
        ofParameter<ofVec3f>	guipTargetNormal;
        ofParameter<ofVec3f>	guipCameraPosition;
        ofParameter<ofVec3f>	guipCameraNormal;
};
