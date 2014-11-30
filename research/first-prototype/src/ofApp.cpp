#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    listCameraDevices();
    
    node1 = new nvrNode(300, 200);
    node2 = new nvrNode(300, 200);
    node3 = new nvrNode(300, 200);
    node4 = new nvrNode(300, 200);

    ofSetWindowTitle("netvr - prototype dashboard");
}

//--------------------------------------------------------------
void ofApp::update(){
    node1->update();
    node2->update();
    node3->update();
    node4->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    node1->draw(0, 0);
    node2->draw(300, 0);
    node3->draw(600, 0);
    node4->draw(900, 0);

    ofPushView();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    node1->bufferOutput.draw(0, 400, 600, 400);
    node2->bufferOutput.draw(0, 400, 600, 400);
    node3->bufferOutput.draw(0, 400, 600, 400);
    node4->bufferOutput.draw(0, 400, 600, 400);
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableAlphaBlending();
    ofPopView();
}

//--------------------------------------------------------------
void ofApp::exit(){
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (x < 300)
        node1->mouseDragged(x, y, button);
    else if (x < 600)
        node2->mouseDragged(x -300, y, button);
    else if (x < 900)
        node3->mouseDragged(x -600, y, button);
    else
        node4->mouseDragged(x -900, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}



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
