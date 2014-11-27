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

    mappingShader.load("shaders/mapping");

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
        ofScale(0.5, 0.5, 0.5);
        ofTranslate(w/2.0, h/2.0, 0);
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
            ofPushMatrix();
            ofScale(0.5, 0.5, 0.5);
            ofTranslate(w/2.0, h/2.0, 0);
            ofMultMatrix(m1);
            image.draw(0, 0);
            ofPopMatrix();
        ofPopMatrix();

        reportStream.str(""); reportStream.clear();
        reportStream << "custom transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo3.end();
    fbo3.draw(p, h + 2*p, w, h);

    // ---- processing anti-transform
    // Problem description
    // http://blender.stackexchange.com/questions/3156/how-to-map-a-texture-to-a-distorted-quad

    // https://github.com/roymacdonald/ofxGLWarper
    // http://forum.openframeworks.cc/t/quad-warping-an-entire-opengl-view-solved/509/46
    // http://openframeworks.cc/documentation/gl/ofVbo.html
    // https://sites.google.com/site/ofauckland/examples/draw-cube-with-ofmesh-and-ofvbo

    // Papers related
    // http://stackoverflow.com/questions/4316829/how-to-transform-a-projected-3d-rectangle-into-a-2d-axis-aligned-rectangle



    distortedCorners[0].set(10, 10);
    distortedCorners[1].set(70, 10);
    distortedCorners[2].set(400, 400);
    distortedCorners[3].set(10, 400);

    originalCorners[0].set(0, 0);
    originalCorners[1].set(w, 0);
    originalCorners[2].set(w, h);
    originalCorners[3].set(0, h);
	
    ofMatrix4x4 homography = ofxHomography::findHomography(distortedCorners, originalCorners);

    ofFbo fbo4;
    fbo4.allocate(w, h);
    fbo4.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        ofPushMatrix();
            ofMultMatrix(homography);
            fbo3.draw(0, 0);
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
