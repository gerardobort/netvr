#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    listCameraDevices();
    
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i] = new nvrNode(300, 200);

    ofSetWindowTitle("netvr - prototype dashboard");

    bufferProjections.allocate(600, 400, GL_RGBA);
    bufferRoom.allocate(600, 400);
    roomMapping.load("shaders/mapping");
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    drawProjections();
    drawRoom();
}

void ofApp::drawProjections(){
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i]->draw(600 + i*300, 0);

    bufferProjections.begin();
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofBackground(0);
        for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
            nodes[i]->bufferOutput.draw(0, 0, 600, 400);
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        ofDisableAlphaBlending();
    bufferProjections.end();
    bufferProjections.draw(600, 400, 600, 400);
}

void ofApp::drawRoom(){
    ofMesh room;
    room.addVertex(ofVec3f(0, 0, 0));
    room.addVertex(ofVec3f(0, 400, 0));
    room.addVertex(ofVec3f(600, 400, 0));
    room.addVertex(ofVec3f(0, 0, 0));
    room.addVertex(ofVec3f(600, 0, 0));
    room.addVertex(ofVec3f(600, 400, 0));

    room.addTexCoord(ofVec2f(0, 0));
    room.addTexCoord(ofVec2f(0, 400));
    room.addTexCoord(ofVec2f(600, 400));
    room.addTexCoord(ofVec2f(0, 0));
    room.addTexCoord(ofVec2f(600, 0));
    room.addTexCoord(ofVec2f(600, 400));

    room.addColor(ofColor(0, 200, 0));
    room.addColor(ofColor(0, 200, 0));
    room.addColor(ofColor(0, 200, 0));
    room.addColor(ofColor(0, 200, 0));
    room.addColor(ofColor(0, 200, 0));
    room.addColor(ofColor(0, 200, 0));

    bufferProjections.getTextureReference().bind();
    bufferRoom.begin();
        ofBackground(0);
        roomCamera.begin(ofRectangle(0, 0, 600, 400));
            ofPushMatrix();
            ofTranslate(0, 0, -400);
            roomMapping.begin();
            room.drawFaces();
            roomMapping.end();
            ofPopMatrix();
        roomCamera.end();
    bufferRoom.end();
    bufferRoom.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::exit(){
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i]->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::listCameraDevices(){

    ofVideoGrabber tvid;
    tvid.setVerbose(true);

    vector<ofVideoDevice> allCameras;
    allCameras = tvid.listDevices();
    cout << endl << "***** Listing camera devices:" << endl;
    for(int i = 0; i < allCameras.size(); i++){
        cout << "***** " << allCameras[i].id << ": " << allCameras[i].deviceName;
        if (allCameras[i].bAvailable) {
            cout << endl;
        } else {
            cout << "***** - unavailable " << endl;
        }
    }
    cout << "***** Total number of detected cameras: " << allCameras.size() << endl << endl;

}
