#include "nvrNode.h"
#include <math.h>

//--------------------------------------------------------------
nvrNode::nvrNode(int cameraWidth, int cameraHeight, int deviceId) : cameraWidth(cameraWidth), cameraHeight(cameraHeight){

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
    bufferOutput.draw(drawX, drawY + drawHeight/2.0, drawWidth, drawHeight/2.0);

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
}
