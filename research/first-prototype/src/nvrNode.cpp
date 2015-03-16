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

    if (deviceId) {
        videoGrabber.setDeviceID(deviceId);
        cout << "************** camera device IN USE: " << deviceId << endl;
    }
    videoGrabber.initGrabber(cameraWidth, cameraHeight);

    outputCorners[0].set(0, 0);
    outputCorners[1].set(cameraWidth, 0);
    outputCorners[2].set(cameraWidth, cameraHeight);
    outputCorners[3].set(0, cameraHeight);

    bufferInput.allocate(cameraWidth, cameraHeight, GL_RGBA);
    bufferOutput.allocate(cameraWidth, cameraHeight, GL_RGBA);
    setupGui();
}

//--------------------------------------------------------------
nvrNode::~nvrNode(){
}

//--------------------------------------------------------------
void nvrNode::update(){
    videoGrabber.update();
    mappingMatrix = ofxHomography::findHomography(inputCorners, outputCorners);

    bufferInput.begin();
        ofClear(0);
        ofBackground(0);
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

    bufferInput.draw(drawX, drawY, drawWidth, drawHeight/2.0);
    if (guipFlipCamsHorizontally)
        bufferOutput.draw(drawX, drawY + drawHeight/2.0, drawWidth, drawHeight/2.0);
    else
        bufferOutput.draw(drawX + drawWidth, drawY + drawHeight/2.0, -drawWidth, drawHeight/2.0);

    reportStream.str(""); reportStream.clear();
    reportStream << "input stream" << endl;
    ofDrawBitmapString(reportStream.str(), drawX +10, drawY +20);

    reportStream.str(""); reportStream.clear();
    reportStream << "output stream" << endl;
    ofDrawBitmapString(reportStream.str(), drawX +10, drawY + drawHeight/2.0 +20);
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
    if (y < drawHeight/2.0) {
        pointSet = &inputCorners[0];
    } else {
        pointSet = &outputCorners[0];
        y -= drawHeight/2.0;
    }

    int dMin = cameraWidth * cameraHeight;
    int jMin = INT_CORNERS_AMOUNT;
    ofPoint mousePoint = ofPoint(x*sx, 2.0*y*sy);
    for (int j = 0; j < INT_CORNERS_AMOUNT; j++) {
        int distance = mousePoint.distance(pointSet[j]);
        if (distance < dMin) {
            dMin = distance;
            jMin = j;
        }
    }
    pointSet[jMin].set(mousePoint.x, mousePoint.y);

    // refresh GUI vars
    guipInputCorners0x = inputCorners[0].x;
    guipInputCorners0y = inputCorners[0].y;
    guipInputCorners1x = inputCorners[1].x;
    guipInputCorners1y = inputCorners[1].y;
    guipInputCorners2x = inputCorners[2].x;
    guipInputCorners2y = inputCorners[2].y;
    guipInputCorners3x = inputCorners[3].x;
    guipInputCorners3y = inputCorners[3].y;
    guipOutputCorners0x = outputCorners[0].x;
    guipOutputCorners0y = outputCorners[0].y;
    guipOutputCorners1x = outputCorners[1].x;
    guipOutputCorners1y = outputCorners[1].y;
    guipOutputCorners2x = outputCorners[2].x;
    guipOutputCorners2y = outputCorners[2].y;
    guipOutputCorners3x = outputCorners[3].x;
    guipOutputCorners3y = outputCorners[3].y;
}

void nvrNode::setupGui() {
    char name[100];
    sprintf(name, "Node %d", number);
	parameters.setName(name);
	parameters.add(guipShowFlow.set("Flow Enabled", false));
	parameters.add(guipFlipCamsHorizontally.set("Flip Cams Horizontally", false));

	inputParameters.setName("Input");
	inputParameters.add(guipInputCorners0x.set("i[0].x", 0, 0, cameraWidth ));
	inputParameters.add(guipInputCorners0y.set("i[0].y", 0, 0, cameraHeight));
	guipInputCorners0x.addListener(this, &nvrNode::setInputCorners0x);
	guipInputCorners0y.addListener(this, &nvrNode::setInputCorners0y);
	inputParameters.add(guipInputCorners1x.set("i[1].x", 0, 0, cameraWidth ));
	inputParameters.add(guipInputCorners1y.set("i[1].y", 0, 0, cameraHeight));
	guipInputCorners1x.addListener(this, &nvrNode::setInputCorners1x);
	guipInputCorners1y.addListener(this, &nvrNode::setInputCorners1y);
	inputParameters.add(guipInputCorners2x.set("i[2].x", 0, 0, cameraWidth ));
	inputParameters.add(guipInputCorners2y.set("i[2].y", 0, 0, cameraHeight));
	guipInputCorners2x.addListener(this, &nvrNode::setInputCorners2x);
	guipInputCorners2y.addListener(this, &nvrNode::setInputCorners2y);
	inputParameters.add(guipInputCorners3x.set("i[3].x", 0, 0, cameraWidth ));
	inputParameters.add(guipInputCorners3y.set("i[3].y", 0, 0, cameraHeight));
	guipInputCorners3x.addListener(this, &nvrNode::setInputCorners3x);
	guipInputCorners3y.addListener(this, &nvrNode::setInputCorners3y);


    parameters.add(inputParameters);

	outputParameters.setName("Output");
	outputParameters.add(guipOutputCorners0x.set("o[0].x", 0, 0, cameraWidth));
	outputParameters.add(guipOutputCorners0y.set("o[0].y", 0, 0, cameraHeight));
	guipOutputCorners0x.addListener(this, &nvrNode::setOutputCorners0x);
	guipOutputCorners0y.addListener(this, &nvrNode::setOutputCorners0y);
	outputParameters.add(guipOutputCorners1x.set("o[1].x", 0, 0, cameraWidth));
	outputParameters.add(guipOutputCorners1y.set("o[1].y", 0, 0, cameraHeight));
	guipOutputCorners1x.addListener(this, &nvrNode::setOutputCorners1x);
	guipOutputCorners1y.addListener(this, &nvrNode::setOutputCorners1y);
	outputParameters.add(guipOutputCorners2x.set("o[2].x", 0, 0, cameraWidth));
	outputParameters.add(guipOutputCorners2y.set("o[2].y", 0, 0, cameraHeight));
	guipOutputCorners2x.addListener(this, &nvrNode::setOutputCorners2x);
	guipOutputCorners2y.addListener(this, &nvrNode::setOutputCorners2y);
	outputParameters.add(guipOutputCorners3x.set("o[3].x", 0, 0, cameraWidth));
	outputParameters.add(guipOutputCorners3y.set("o[3].y", 0, 0, cameraHeight));
	guipOutputCorners3x.addListener(this, &nvrNode::setOutputCorners3x);
	guipOutputCorners3y.addListener(this, &nvrNode::setOutputCorners3y);
    parameters.add(outputParameters);
}
