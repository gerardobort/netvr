#include "nvrNode.h"
#include <math.h>

//--------------------------------------------------------------
nvrNode::nvrNode(int width, int height) : width(width), height(height){

    mappingShader.load("shaders/mapping");

    clickCount = 0;
    inputCorners[0].set(10, 10);
    inputCorners[1].set(70, 10);
    inputCorners[2].set(150, 140);
    inputCorners[3].set(10, 160);

    videoGrabber.initGrabber(640, 480);

    outputCorners[0].set(0, 0);
    outputCorners[1].set(width, 0);
    outputCorners[2].set(width, height);
    outputCorners[3].set(0, height);

    bufferInput.allocate(width, height, GL_RGBA);
    bufferOutput.allocate(width, height, GL_RGBA);
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
        videoGrabber.draw(0, 0, width, height);
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
void nvrNode::draw(int x, int y){

    bufferInput.begin();
        for (int j = 0; j < 5; j++) {
            ofSetColor(0, 255, 0, 30);
            ofCircle(inputCorners[j].x, inputCorners[j].y, 6);

            ofSetColor(0, 255, 0, 255);
            reportStream.str(""); reportStream.clear();
            reportStream << j << endl;
            ofDrawBitmapString(reportStream.str(), inputCorners[j].x, inputCorners[j].y);
        }
        ofSetColor(255);

        ofSetColor(255, 0, 0, 127);
        ofLine(inputCorners[3].x, inputCorners[3].y, 0, inputCorners[0].x, inputCorners[0].y, 0);
        for (int j = 1; j < 5; j++) {
            ofSetColor(0, 0, 255, 127);
            ofLine(inputCorners[j-1].x, inputCorners[j-1].y, 0, inputCorners[j].x, inputCorners[j].y, 0);
        }
        ofSetColor(255);

        reportStream.str(""); reportStream.clear();
        reportStream << "input stream" << endl;
        ofDrawBitmapString(reportStream.str(), 20, height -20);
    bufferInput.end();

    bufferOutput.begin();
        ofSetColor(255, 255, 255);
        reportStream.str(""); reportStream.clear();
        reportStream << "output stream" << endl;
        ofDrawBitmapString(reportStream.str(), 20, height -20);
    bufferOutput.end();

    bufferInput.draw(x, y, width, height);
    bufferOutput.draw(x, y + height, width, height);

}


//--------------------------------------------------------------
void nvrNode::mousePressed(int x, int y, int button){
    if (clickCount < 5) inputCorners[clickCount].set(x, y);
    if (++clickCount >= 5) clickCount = 0;
}
