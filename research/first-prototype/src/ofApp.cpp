#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    node1 = new nvrNode(300, 200);
    node2 = new nvrNode(300, 200);
    node3 = new nvrNode(300, 200);

}

//--------------------------------------------------------------
void ofApp::update(){
    node1->update();
    node2->update();
    node3->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    node1->draw(0, 0);
    node2->draw(300, 0);
    node3->draw(600, 0);

    ofPushView();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    node1->bufferOutput.draw(300, 440);
    node2->bufferOutput.draw(300, 440);
    node3->bufferOutput.draw(300, 440);
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableAlphaBlending();
    ofPopView();
}

//--------------------------------------------------------------
void ofApp::exit(){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (x < 300)
        node1->mousePressed(x, y, button);
    else if (x < 600)
        node2->mousePressed(x -300, y, button);
    else
        node3->mousePressed(x -600, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}
