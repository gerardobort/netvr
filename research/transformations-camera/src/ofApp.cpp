#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    image.loadImage("bort.jpg");

    camera.setGlobalPosition(ofVec3f(0, 0, 400));
    camera.setTarget(ofVec3f(image.width/2.0, image.height/2.0, 0));
    camera.setVFlip(true);

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

    int w = image.width/2.0;
    int h = image.height/2.0;
    int p = 10;

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


    stringstream reportStream;

    ofFbo fbo1;
    fbo1.allocate(w, h);
    fbo1.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        image.draw(0, 0);

        reportStream.str(""); reportStream.clear();
        reportStream << "original" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo1.end();
    fbo1.draw(p, p, w, h);

    ofFbo fbo2;
    fbo2.allocate(w, h);
    fbo2.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        camera.begin();
        image.draw(0, 0);
        camera.end();

        reportStream.str(""); reportStream.clear();
        reportStream << "camera view transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo2.end();
    fbo2.draw(w + 2*p, p, w, h);


    ofFbo fbo3;
    fbo3.allocate(w, h);
    fbo3.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        ofMatrix4x4 m1 = camera.getModelViewMatrix();
        ofPushMatrix();
            ofLoadMatrix(m1);
            image.draw(0, 0);
        ofPopMatrix();

        reportStream.str(""); reportStream.clear();
        reportStream << "custom transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo3.end();
    fbo3.draw(p, h + 2*p, w, h);

    ofFbo fbo4;
    fbo4.allocate(w, h);
    fbo4.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        ofMatrix4x4 m2 = camera.getModelViewMatrix().getInverse();
        m2.makeOrtho2DMatrix(-w/2.0, w/2.0, -h/2.0, h/2.0);
        ofPushMatrix();
            ofLoadMatrix(m2);
            fbo3.draw(0, 0, w, h);
        ofPopMatrix();

        reportStream.str(""); reportStream.clear();
        reportStream << "custom anti-transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo4.end();
    fbo4.draw(w + 2*p, h + 2*p, w, h);


}

//--------------------------------------------------------------
void ofApp::exit(){
}
