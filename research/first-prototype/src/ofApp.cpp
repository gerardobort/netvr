#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    listCameraDevices();
    
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i] = new nvrNode(640, 480);

    ofSetWindowTitle("netvr - prototype dashboard");

    bufferProjections.allocate(INT_ROOM_WIDTH, INT_ROOM_DEPTH, GL_RGBA);
    bufferFlow.allocate(INT_ROOM_WIDTH, INT_ROOM_DEPTH, GL_RGBA);
    bufferRoom.allocate(600, 400, GL_RGBA);
    roomMapping.load("shaders/mapping");
    roomCamera.setPosition(ofVec3f(0, INT_ROOM_HEIGHT, INT_ROOM_WIDTH+INT_ROOM_HEIGHT));
    roomCamera.setTarget(ofVec3f(0, INT_ROOM_HEIGHT/2.0, 0));
    roomCamera.setDistance(INT_ROOM_WIDTH+INT_ROOM_HEIGHT);

    opticalFlow.setup(INT_ROOM_WIDTH/4.0, INT_ROOM_DEPTH/4.0);
    velocityField.allocate(INT_ROOM_WIDTH, INT_ROOM_DEPTH);

    lastTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
    didCamsUpdate = false;
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++) {
        nodes[i]->update();
        didCamsUpdate = didCamsUpdate || nodes[i]->videoGrabber.isFrameNew();
    }

    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    drawProjections();
    drawOpticalFlow();
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
            nodes[i]->bufferOutput.draw(0, 0, INT_ROOM_WIDTH, INT_ROOM_DEPTH);
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        ofDisableAlphaBlending();
    bufferProjections.end();
    bufferProjections.draw(600, 400, 600, 400);
}

void ofApp::drawRoom(){


    ofMesh room;
    room.addVertex(ofVec3f(0, 0, 0));
    room.addVertex(ofVec3f(0, 0, INT_ROOM_DEPTH));
    room.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, INT_ROOM_DEPTH));
    room.addVertex(ofVec3f(0, 0, 0));
    room.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, 0));
    room.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, INT_ROOM_DEPTH));

    room.addTexCoord(ofVec2f(0, 0));
    room.addTexCoord(ofVec2f(0, INT_ROOM_DEPTH));
    room.addTexCoord(ofVec2f(INT_ROOM_WIDTH, INT_ROOM_DEPTH));
    room.addTexCoord(ofVec2f(0, 0));
    room.addTexCoord(ofVec2f(INT_ROOM_WIDTH, 0));
    room.addTexCoord(ofVec2f(INT_ROOM_WIDTH, INT_ROOM_DEPTH));

    ofMesh roomWireframe;
    roomWireframe.addVertex(ofVec3f(0, -10, 0));
    roomWireframe.addVertex(ofVec3f(0, -10, INT_ROOM_DEPTH));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, -10, INT_ROOM_DEPTH));
    roomWireframe.addVertex(ofVec3f(0, -10, 0));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, -10, 0));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, -10, INT_ROOM_DEPTH));

    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));

    roomWireframe.addVertex(ofVec3f(0, INT_ROOM_HEIGHT, 0));
    roomWireframe.addVertex(ofVec3f(0, INT_ROOM_HEIGHT, INT_ROOM_DEPTH));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, INT_ROOM_HEIGHT, INT_ROOM_DEPTH));
    roomWireframe.addVertex(ofVec3f(0, INT_ROOM_HEIGHT, 0));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, INT_ROOM_HEIGHT, 0));
    roomWireframe.addVertex(ofVec3f(INT_ROOM_WIDTH, INT_ROOM_HEIGHT, INT_ROOM_DEPTH));

    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));
    roomWireframe.addColor(ofColor(0, 100, 0));

    bufferFlow.getTextureReference().bind();
    bufferRoom.begin();
        ofBackground(0);
        roomCamera.begin(ofRectangle(0, 0, 600, 400));
            ofEnableDepthTest();
            ofPushMatrix();
            ofTranslate(-INT_ROOM_WIDTH/2.0, 0, -INT_ROOM_DEPTH/2.0);
            roomMapping.begin();
            room.drawFaces();
            roomMapping.end();
            roomWireframe.drawWireframe();
            glPointSize(4);
            roomWireframe.drawVertices();
            ofPopMatrix();
            ofDisableDepthTest();
        roomCamera.end();
    bufferRoom.end();
    bufferRoom.draw(0, 0);
}

void ofApp::drawOpticalFlow(){
    if (didCamsUpdate) {
        opticalFlow.setSource(bufferProjections.getTextureReference());
        opticalFlow.update(deltaTime);
    }
    velocityField.setSource(opticalFlow.getOpticalFlowDecay());
    bufferFlow.begin();
        ofBackground(0);
        velocityField.draw(0, 0, INT_ROOM_WIDTH, INT_ROOM_DEPTH);
    bufferFlow.end();
    bufferFlow.draw(0, 400, 600, 400);
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
