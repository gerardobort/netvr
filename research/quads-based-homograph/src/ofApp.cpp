#include "ofApp.h"
#include <math.h>

// Taken from https://sites.google.com/site/ofauckland/examples/draw-cube-with-ofmesh-and-ofvbo

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0);
    glEnable(GL_DEPTH_TEST);

    ofMesh mesh;
        
    char c[] = "00 01 11 10";
    
    for (int i=0,x,y,z; i<12; i+=3) {
        
        x = c[i+0]-'0';
        y = c[i+1]-'0'; 
        z = 0; 
        
        mesh.addColor(ofFloatColor(x,y,z));
        mesh.addVertex(ofVec3f(x*2-1,y*2-1,z*2-1));
        //mesh.addTexCoord(ofVec2f(400.0*x,400.0*y));
    }

    mesh.addTexCoord(ofVec2f(240.0,200.0));
    mesh.addTexCoord(ofVec2f(240.0,40.0));
    mesh.addTexCoord(ofVec2f(10.0,5.0));
    mesh.addTexCoord(ofVec2f(30.0,220.0));
    
    vbo.setMesh(mesh, GL_STATIC_DRAW);
    image.loadImage("homograph.png");
 }

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    ofScale(100,100,100);
    image.getTextureReference().bind();
    vbo.draw(GL_TRIANGLE_FAN,0,4);
}
