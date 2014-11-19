#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    for (int i = 0; i < NUM_CAMERAS; i++) {
        cameras[i].setGlobalPosition(ofVec3f(ROOM_W/2 + i*(600/NUM_CAMERAS) - 600/2, ROOM_H*2, ROOM_D*3));
        ofVec3f orientation = cameras[i].getGlobalPosition() - ofVec3f(ROOM_W/2, ROOM_H/2, ROOM_D/2);
        cameras[i].setOrientation(orientation.getNormalized());
    }
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

    float row = 0, column = 0;
    int w = ofGetWindowWidth()/VIEW_COLS;
    int h = ofGetWindowWidth()/VIEW_COLS * 2.0/3.0;
    for (int i = 0; i < NUM_CAMERAS; i++) {
        row = i/VIEW_COLS;
        column = i%VIEW_COLS;
        drawView(&cameras[i], column*w, row*h, w, h);
	    stringstream reportStream;
        reportStream << row << ", " << column << endl;
	    ofDrawBitmapString(reportStream.str(), column*w + 20, row*h + 20);
    }

}

//--------------------------------------------------------------
void ofApp::drawView(ofEasyCam* camera, int x, int y, int w, int h) {
    ofFbo fbo;
    fbo.allocate(w, h);
    fbo.begin();
        ofBackground(ofColor(0, 255, 0, 160));
        camera->begin();
            drawScene();
        camera->end();
    fbo.end();
    fbo.draw(x, y, w, h);

    ofTexture map1 = fbo.getTextureReference(0);
    ofTexture map2;
    ofPixels pixels1;
    unsigned char pixels2[w*h*4];
    map1.readToPixels(pixels1);
    map2.allocate(w, h, GL_RGBA);
    ofFbo fboi;
    ofMesh mesh;

    for (int u = 0; u < w; u++)
        for (int v = 0; v < h; v++) {
            int i = 4*(v*w + u);
            ofColor color = ofColor(pixels1[i +0], pixels1[i +1], pixels1[i +2], 255);
            if (color.g != 255) {
                pixels2[i +0] = color.r;
                pixels2[i +1] = color.g;
                pixels2[i +2] = color.b;
                pixels2[i +3] = color.a;
                ofVec3f worldCoord = camera->worldToScreen(ofVec3f(u, v, 0), ofRectangle(x, y, w, h));
                mesh.addColor(color);
                mesh.addVertex(worldCoord);
            } else {
                pixels2[i +0] = 255;
                pixels2[i +1] = 0;
                pixels2[i +2] = 0;
                pixels2[i +3] = 255;
            }
        }

    fboi.allocate(w, h);
    fboi.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        map2.loadData(pixels2, w, h, GL_RGBA);
        map2.draw(0, 0, w, h);
    fboi.end();
    fboi.draw(x, y+h+10, w, h);

    glPointSize(4);
    mesh.drawVertices();
}

//--------------------------------------------------------------
void ofApp::drawScene() {

    ofMesh plane;
    plane.setMode(OF_PRIMITIVE_TRIANGLES);

    plane.addColor(ofColor(255, 0, 0, 255));
    plane.addColor(ofColor(0, 255, 0, 255));
    plane.addColor(ofColor(0, 0, 255, 255));

    plane.addColor(ofColor(255, 0, 255, 255));
    plane.addColor(ofColor(0, 255, 0, 255));
    plane.addColor(ofColor(0, 0, 255, 255));

    plane.addVertex(ofVec3f(0, 0, 0));
    plane.addVertex(ofVec3f(ROOM_W, 0, 0));
    plane.addVertex(ofVec3f(0, 0, ROOM_D));

    plane.addVertex(ofVec3f(ROOM_W, 0, ROOM_D));
    plane.addVertex(ofVec3f(ROOM_W, 0, 0));
    plane.addVertex(ofVec3f(0, 0, ROOM_D));

    plane.drawFaces();


    ofMesh normal;
    normal.setMode(OF_PRIMITIVE_LINES);

    normal.addColor(ofColor(255, 255, 0, 255));
    normal.addColor(ofColor(255, 255, 0, 255));

    normal.addVertex(ofVec3f(ROOM_W/2, 0, ROOM_D/2));
    normal.addVertex(ofVec3f(ROOM_W/2, ROOM_H, ROOM_D/2));

    normal.drawWireframe();
    glPointSize(4);
    normal.drawVertices();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case OF_KEY_LEFT:
            //for (int i = 0; i < NUM_CAMERAS; i++)
            //    cameras[i].setDistance(cameras[i].getDistance() + 20);
			break;

		case OF_KEY_RIGHT:
            //for (int i = 0; i < NUM_CAMERAS; i++)
            //    cameras[i].setDistance(cameras[i].getDistance() - 20);
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
}
