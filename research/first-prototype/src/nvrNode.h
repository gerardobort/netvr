#pragma once

#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxHomography.h"
#include "ofxFlowTools.h"

#define M_PI 3.1415926535897932384626433832795

#define INT_CORNERS_AMOUNT 5

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

        ofParameter<int>	guipInputCorners0x;
        ofParameter<int>	guipInputCorners0y;
	    void setInputCorners0x(int& _value) { inputCorners[0].x = _value; }
	    void setInputCorners0y(int& _value) { inputCorners[0].y = _value; }
        ofParameter<int>	guipInputCorners1x;
        ofParameter<int>	guipInputCorners1y;
	    void setInputCorners1x(int& _value) { inputCorners[1].x = _value; }
	    void setInputCorners1y(int& _value) { inputCorners[1].y = _value; }
        ofParameter<int>	guipInputCorners2x;
        ofParameter<int>	guipInputCorners2y;
	    void setInputCorners2x(int& _value) { inputCorners[2].x = _value; }
	    void setInputCorners2y(int& _value) { inputCorners[2].y = _value; }
        ofParameter<int>	guipInputCorners3x;
        ofParameter<int>	guipInputCorners3y;
	    void setInputCorners3x(int& _value) { inputCorners[3].x = _value; }
	    void setInputCorners3y(int& _value) { inputCorners[3].y = _value; }

        ofParameter<int>	guipOutputCorners0x;
        ofParameter<int>	guipOutputCorners0y;
	    void setOutputCorners0x(int& _value) { outputCorners[0].x = _value; }
	    void setOutputCorners0y(int& _value) { outputCorners[0].y = _value; }
        ofParameter<int>	guipOutputCorners1x;
        ofParameter<int>	guipOutputCorners1y;
	    void setOutputCorners1x(int& _value) { outputCorners[1].x = _value; }
	    void setOutputCorners1y(int& _value) { outputCorners[1].y = _value; }
        ofParameter<int>	guipOutputCorners2x;
        ofParameter<int>	guipOutputCorners2y;
	    void setOutputCorners2x(int& _value) { outputCorners[2].x = _value; }
	    void setOutputCorners2y(int& _value) { outputCorners[2].y = _value; }
        ofParameter<int>	guipOutputCorners3x;
        ofParameter<int>	guipOutputCorners3y;
	    void setOutputCorners3x(int& _value) { outputCorners[3].x = _value; }
	    void setOutputCorners3y(int& _value) { outputCorners[3].y = _value; }
};
