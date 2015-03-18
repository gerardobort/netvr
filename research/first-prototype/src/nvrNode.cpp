#include "nvrNode.h"
#include <math.h>

//--------------------------------------------------------------
nvrNode::nvrNode(int cameraWidth, int cameraHeight, int deviceId, int number)
    : cameraWidth(cameraWidth), cameraHeight(cameraHeight), deviceId(deviceId), number(number) {

    mappingShader.load("shaders/mapping");

    inputCorners[0].set(cameraWidth/5.0, cameraHeight/5.0);
    inputCorners[1].set(cameraWidth - cameraWidth/5.0, cameraHeight/5.0);
    inputCorners[2].set(cameraWidth - cameraWidth/5.0, cameraHeight - cameraHeight/5.0);
    inputCorners[3].set(cameraWidth/5.0, cameraHeight - cameraHeight/5.0);

    outputCorners[0].set(0, 0);
    outputCorners[1].set(cameraWidth, 0);
    outputCorners[2].set(cameraWidth, cameraHeight);
    outputCorners[3].set(0, cameraHeight);

    if (deviceId) {
        videoGrabber.setDeviceID(deviceId);
        cout << "************** camera device IN USE: " << deviceId << endl;
    }
    videoGrabber.initGrabber(cameraWidth, cameraHeight);

    opticalFlow.setup(cameraWidth/4.0, cameraHeight/4.0);
    velocityMask.setup(cameraWidth, cameraHeight);
    fluid.setup(cameraWidth/4.0, cameraHeight/4.0, cameraWidth, cameraHeight, false);
    velocityField.allocate(cameraWidth/16.0, cameraHeight/16.0);
    lastTime = ofGetElapsedTimef();

    bufferInput.allocate(cameraWidth, cameraHeight, GL_RGBA);
    bufferOutput.allocate(cameraWidth, cameraHeight, GL_RGBA);
    bufferFlow.allocate(cameraWidth, cameraHeight, GL_RGBA);
    setupGui();

    updateValues();
}

//--------------------------------------------------------------
nvrNode::~nvrNode(){
}

//--------------------------------------------------------------
void nvrNode::update(){

    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();

    videoGrabber.update();
    mappingMatrix = ofxHomography::findHomography(inputCorners, outputCorners);

    bufferInput.begin();
        ofClear(0);
        ofBackground(0);
        if (guipFlipCamsHorizontally.get())
            videoGrabber.draw(cameraWidth, 0, -cameraWidth, cameraHeight);
        else
            videoGrabber.draw(0, 0, cameraWidth, cameraHeight);
    
    bufferInput.end();

    bufferOutput.begin();
        ofClear(0);
        ofBackground(0);
        ofPushMatrix();
            ofMultMatrix(mappingMatrix);
            bufferInput.draw(0, 0);
        ofPopMatrix();
    bufferOutput.end();
}

//--------------------------------------------------------------
void nvrNode::draw(int x, int y, int width, int height){

    drawX = x;
    drawY = y;
    drawWidth = width;
    drawHeight = height;

    bufferInput.begin();
        ofEnableAlphaBlending();
        for (int j = 0; j < INT_CORNERS_AMOUNT; j++) {
            ofSetColor(0, 255, 0, 80);
            ofCircle(inputCorners[j].x, inputCorners[j].y, 6);

            ofSetColor(0, 255, 0, 255);
            reportStream.str(""); reportStream.clear();
            reportStream << j << endl;
            ofDrawBitmapString(reportStream.str(), inputCorners[j].x, inputCorners[j].y);
        }

        ofSetColor(255, 0, 0, 127);
        ofLine(inputCorners[3].x, inputCorners[3].y, 0, inputCorners[0].x, inputCorners[0].y, 0);
        for (int j = 1; j < INT_CORNERS_AMOUNT; j++) {
            ofSetColor(0, 0, 255, 127);
            ofLine(inputCorners[j-1].x, inputCorners[j-1].y, 0, inputCorners[j].x, inputCorners[j].y, 0);
        }

        ofDisableAlphaBlending();
    bufferInput.end();

    bufferOutput.begin();
        ofEnableAlphaBlending();
        for (int j = 0; j < INT_CORNERS_AMOUNT; j++) {
            ofSetColor(0, 0, 255, 80);
            ofCircle(outputCorners[j].x, outputCorners[j].y, 6);

            ofSetColor(0, 0, 255, 255);
            reportStream.str(""); reportStream.clear();
            reportStream << j << "'" << endl;
            ofDrawBitmapString(reportStream.str(), outputCorners[j].x, outputCorners[j].y);
        }

        ofSetColor(255, 0, 0, 127);
        ofLine(outputCorners[3].x, outputCorners[3].y, 0, outputCorners[0].x, outputCorners[0].y, 0);
        for (int j = 1; j < INT_CORNERS_AMOUNT; j++) {
            ofSetColor(0, 0, 255, 127);
            ofLine(outputCorners[j-1].x, outputCorners[j-1].y, 0, outputCorners[j].x, outputCorners[j].y, 0);
        }
        ofDisableAlphaBlending();
    bufferOutput.end();

    ofSetColor(255); // color fix

    bufferInput.draw(drawX, drawY, drawWidth, drawHeight/3.0);
    bufferOutput.draw(drawX, drawY + drawHeight/3.0, drawWidth, drawHeight/3.0);

    reportStream.str(""); reportStream.clear();
    reportStream << "input stream" << endl;
    ofDrawBitmapString(reportStream.str(), drawX +10, drawY +20);

    reportStream.str(""); reportStream.clear();
    reportStream << "output stream" << endl;
    ofDrawBitmapString(reportStream.str(), drawX +10, drawY + drawHeight/3.0 +20);

    drawOpticalFlow();
    reportStream.str(""); reportStream.clear();
    reportStream << "flow stream" << endl;
    ofDrawBitmapString(reportStream.str(), drawX +10, drawY + 2.0*drawHeight/3.0 +20);
}

//--------------------------------------------------------------
void nvrNode::drawOpticalFlow(){
    if (videoGrabber.isFrameNew()) {
        opticalFlow.setSource(bufferOutput.getTextureReference());
        opticalFlow.update(deltaTime);
        velocityMask.setDensity(bufferOutput.getTextureReference());
        velocityMask.setVelocity(opticalFlow.getOpticalFlow());
        velocityMask.update();
        fluid.addVelocity(opticalFlow.getOpticalFlowDecay());
        fluid.addDensity(velocityMask.getColorMask());
        fluid.addTemperature(velocityMask.getLuminanceMask());
        fluid.update();
    }
    //velocityField.setSource(opticalFlow.getOpticalFlowDecay());
    velocityField.setSource(fluid.getVelocity());
    ofEnableAlphaBlending();
    bufferFlow.begin();
        ofClear(0, 0, 0, 0);
        ofSetBackgroundColor(255, 0, 0, 0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        velocityField.draw(0, 0, cameraWidth, cameraHeight);
        //velocityMask.draw(0, 0, INT_ROOM_WIDTH, INT_ROOM_DEPTH);
        ofDisableBlendMode();
    bufferFlow.end();
    bufferFlow.draw(drawX, drawY + 2.0*drawHeight/3.0, drawWidth, 2.0*drawHeight/3.0);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void nvrNode::mouseDragged(int x, int y, int button){

    float sx = cameraWidth/(float)drawWidth;
    float sy = cameraHeight/(float)drawHeight;

    if (x < drawX || x > drawX + drawWidth) return;
    if (y < drawY || y > drawY + drawHeight) return;

    x -= drawX;
    y -= drawY;

    ofPoint *pointSet;
    if (y < drawHeight/3.0) {
        pointSet = &inputCorners[0];
    } else if (y < 2.0*drawHeight/3.0) {
        pointSet = &outputCorners[0];
        y -= drawHeight/3.0;
    } else {
        return;
    }

    int dMin = cameraWidth * cameraHeight;
    int jMin = INT_CORNERS_AMOUNT;
    ofPoint mousePoint = ofPoint(x*sx, 3.0*y*sy);
    for (int j = 0; j < INT_CORNERS_AMOUNT; j++) {
        int distance = mousePoint.distance(pointSet[j]);
        if (distance < dMin) {
            dMin = distance;
            jMin = j;
        }
    }
    pointSet[jMin].set(mousePoint.x, mousePoint.y);
    updateValues();
}

void nvrNode::updateValues(){
    // refresh GUI vars
    guipInputCorners0 = inputCorners[0];
    guipInputCorners1 = inputCorners[1];
    guipInputCorners2 = inputCorners[2];
    guipInputCorners3 = inputCorners[3];
    guipOutputCorners0 = outputCorners[0];
    guipOutputCorners1 = outputCorners[1];
    guipOutputCorners2 = outputCorners[2];
    guipOutputCorners3 = outputCorners[3];
}

void nvrNode::setupGui() {
    char name[100];
    sprintf(name, "Node %d", number);
	parameters.setName(name);
	parameters.add(guipShowFlow.set("Flow Enabled", false));
	parameters.add(guipFlipCamsHorizontally.set("Flip Cams Horizontally", false));

	parameters.add(guipTargetPosition.set("target position", ofVec3f(0,0,0), ofVec3f(-INT_ROOM_WIDTH, -INT_ROOM_DEPTH, -INT_ROOM_HEIGHT), ofVec3f(INT_ROOM_WIDTH, INT_ROOM_DEPTH, INT_ROOM_HEIGHT)));
	parameters.add(guipTargetNormal.set("target normal", ofVec3f(0,0,0), ofVec3f(-INT_ROOM_WIDTH, -INT_ROOM_DEPTH, -INT_ROOM_HEIGHT), ofVec3f(INT_ROOM_WIDTH, INT_ROOM_DEPTH, INT_ROOM_HEIGHT)));
	parameters.add(guipCameraPosition.set("camera position", ofVec3f(0,0,0), ofVec3f(-INT_ROOM_WIDTH, -INT_ROOM_DEPTH, -INT_ROOM_HEIGHT), ofVec3f(INT_ROOM_WIDTH, INT_ROOM_DEPTH, INT_ROOM_HEIGHT)));
	parameters.add(guipCameraNormal.set("camera normal", ofVec3f(0,0,0), ofVec3f(-INT_ROOM_WIDTH, -INT_ROOM_DEPTH, -INT_ROOM_HEIGHT), ofVec3f(INT_ROOM_WIDTH, INT_ROOM_DEPTH, INT_ROOM_HEIGHT)));

	inputParameters.setName("input");
	inputParameters.add(guipInputCorners0.set("i[0]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipInputCorners0.addListener(this, &nvrNode::setInputCorners0);
	inputParameters.add(guipInputCorners1.set("i[1]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipInputCorners1.addListener(this, &nvrNode::setInputCorners1);
	inputParameters.add(guipInputCorners2.set("i[2]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipInputCorners2.addListener(this, &nvrNode::setInputCorners2);
	inputParameters.add(guipInputCorners3.set("i[3]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipInputCorners3.addListener(this, &nvrNode::setInputCorners3);
    parameters.add(inputParameters);

	outputParameters.setName("output");
	outputParameters.add(guipOutputCorners0.set("o[0]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipOutputCorners0.addListener(this, &nvrNode::setOutputCorners0);
	outputParameters.add(guipOutputCorners1.set("o[1]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipOutputCorners1.addListener(this, &nvrNode::setOutputCorners1);
	outputParameters.add(guipOutputCorners2.set("o[2]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipOutputCorners2.addListener(this, &nvrNode::setOutputCorners2);
	outputParameters.add(guipOutputCorners3.set("o[3]", ofPoint(0), ofPoint(0), ofPoint(cameraWidth, cameraHeight)));
	guipOutputCorners3.addListener(this, &nvrNode::setOutputCorners3);
    parameters.add(outputParameters);

	parameters.add(opticalFlow.parameters);
	//parameters.add(velocityMask.parameters);
	//parameters.add(fluid.parameters);
}
