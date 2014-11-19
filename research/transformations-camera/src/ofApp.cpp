#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    image.loadImage("bort.jpg");

    camera.setGlobalPosition(ofVec3f(0, 0, 400));
    camera.setTarget(ofVec3f(image.width/2.0, image.height/2.0, 0));

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    ofEnableDepthTest();
	ofSetColor(255, 255, 255);

    int w = image.width;
    int h = image.height;

    ofPixels pixels1 = image.getPixelsRef();
    unsigned char pixels2[w*h*3];
    ofTexture out1;
    out1.allocate(w, h, GL_RGB);
    ofMesh mesh1;
    ofMesh mesh2;

    for (int u = 0; u < w; u++)
        for (int v = 0; v < h; v++) {
            int i = 3*(v*w + u);
            ofColor color = ofColor(pixels1[i +0], pixels1[i +1], pixels1[i +2], 255);
            ofVec3f vertex = ofVec3f(u, v, 0);

            ofMatrix4x4 m = ofMatrix4x4::newTranslationMatrix(0.001, 0.001, 0) * ofMatrix4x4::newRotationMatrix(45, 1.0, 1.0, 1.0) * ofMatrix4x4::newScaleMatrix(0.5, 0.5, 0.5);
            vertex = m.postMult(vertex);
            mesh1.addColor(color);
            mesh1.addVertex(vertex);

            vertex = m.getInverse().postMult(vertex);
            mesh2.addColor(color);
            mesh2.addVertex(vertex);
        }


    ofFbo fbo1;
    fbo1.allocate(w, h);
    fbo1.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        image.draw(0, 0);
    fbo1.end();
    fbo1.draw(0, 0, w, h);

    ofFbo fbo2;
    fbo2.allocate(w, h);
    fbo2.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        mesh1.drawVertices();
    fbo2.end();
    fbo2.draw(w, 0, w, h);

    /*
    ofFbo fbo3;
    fbo3.allocate(w, h);
    fbo3.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        mesh2.drawVertices();
    fbo3.end();
    fbo3.draw(0, h, w, h);
    */

    ofFbo fbo3;
    fbo3.allocate(w, h);
    fbo3.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        ofMatrix4x4 m = camera.getModelViewMatrix();
        //m.preMultScale(ofVec3f(1.0, 1.0, 1.0));
        ofLoadMatrix(m);
        image.draw(0, 0);
    fbo3.end();
    fbo3.draw(0, h, w, h);

    ofFbo fbo4;
    fbo4.allocate(w, h);
    fbo4.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        camera.begin();
        image.draw(0, 0);
        camera.end();
    fbo4.end();
    fbo4.draw(w, h, w, h);

}

//--------------------------------------------------------------
void ofApp::exit(){
}
