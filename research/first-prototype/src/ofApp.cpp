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

    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i]->draw(i*300, 0);

    ofPushView();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++)
        nodes[i]->bufferOutput.draw(0, 400, 600, 400);
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofDisableAlphaBlending();
    ofPopView();
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
