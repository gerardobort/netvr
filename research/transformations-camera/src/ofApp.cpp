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
            ofLoadMatrix(m1);
            image.draw(0, 0);
            ofPopMatrix();
        ofPopMatrix();

        reportStream.str(""); reportStream.clear();
        reportStream << "custom transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo3.end();
    fbo3.draw(p, h + 2*p, w, h);

    // ---- processing anti-transform

    ofTexture out1 = fbo1.getTextureReference();
    ofMesh mesh;

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.addVertex(ofVec3f(-w/2.0, -h/2.0, -200.0)); // A
    mesh.addVertex(ofVec3f( w/2.0, -h/2.0, -200.0)); // B
    mesh.addVertex(ofVec3f(-w/2.0,  h/2.0, -200.0)); // C
    mesh.addVertex(ofVec3f( w/2.0, -h/2.0, -200.0)); // B
    mesh.addVertex(ofVec3f(-w/2.0,  h/2.0, -200.0)); // C
    mesh.addVertex(ofVec3f( w/2.0,  h/2.0, -200.0)); // D

    ofVec3f tmpVec3;
    ofVec2f tmpVec2;

    /*
    tmpVec3 = m1.postMult(ofVec3f(0, 0, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // A
    tmpVec3 = m1.postMult(ofVec3f(w, 0, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // B
    tmpVec3 = m1.postMult(ofVec3f(0, h, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // C
    tmpVec3 = m1.postMult(ofVec3f(w, 0, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // B
    tmpVec3 = m1.postMult(ofVec3f(0, h, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // C
    tmpVec3 = m1.postMult(ofVec3f(w, h, 0));   tmpVec2 = ofVec2f(tmpVec3.x, tmpVec3.y);   mesh.addTexCoord(tmpVec2); // D
    */
    mesh.addTexCoord(ofVec2f(20.0, 20.0)); // A
    mesh.addTexCoord(ofVec2f(100.0, 20.0)); // B
    mesh.addTexCoord(ofVec2f(20.0, 300.0)); // C
    mesh.addTexCoord(ofVec2f(100.0, 20.0)); // B
    mesh.addTexCoord(ofVec2f(20.0, 300.0)); // C
    mesh.addTexCoord(ofVec2f(300.0, 30.0)); // D

    // Problem description
    // http://blender.stackexchange.com/questions/3156/how-to-map-a-texture-to-a-distorted-quad

    // https://github.com/roymacdonald/ofxGLWarper
    // http://forum.openframeworks.cc/t/quad-warping-an-entire-opengl-view-solved/509/46
    // http://openframeworks.cc/documentation/gl/ofVbo.html
    // https://sites.google.com/site/ofauckland/examples/draw-cube-with-ofmesh-and-ofvbo

    // Papers related
    // http://stackoverflow.com/questions/4316829/how-to-transform-a-projected-3d-rectangle-into-a-2d-axis-aligned-rectangle

    const float Verts[] = {
        0.000f,  0.000f,  0.447f,
        1.000f,  0.000f,  0.447f,
        1.000f,  1.000f,  0.447f,
        0.000f,  1.000f,  0.447f};

    ofVec3f v[4];
    ofVec3f n[4];
    ofFloatColor c[4];
    ofVbo vbo;

        int i, j = 0;
        for ( i = 0; i < 4; i++ ) {
            c[i].r = ofRandom(1.0);
            c[i].g = ofRandom(1.0);
            c[i].b = ofRandom(1.0);
     
            v[i][0] = Verts[j++] * 200.f;
            v[i][1] = Verts[j++] * 200.f;
            v[i][2] = Verts[j++] * 200.f;
        }
     
        vbo.setVertexData( &v[0], 12, GL_STATIC_DRAW );
        vbo.setColorData( &c[0], 12, GL_STATIC_DRAW );
        //vbo.setTexCoordData( &Verts[0], 2, 8, GL_STATIC_DRAW );
     
        glEnable(GL_DEPTH_TEST);

        ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 100);
        //ofRotate(ofGetElapsedTimef() * 20.0, 1, 1, 0);
        glPointSize(10.f);

        glEnable(GL_DEPTH_TEST);

        vbo.draw( GL_LINE_LOOP, 0, 4 );

        vbo.draw( GL_QUADS, 0, 4 );
    

    ofFbo fbo4;
    fbo4.allocate(w, h);
    fbo4.begin();
        ofBackground(ofColor(0, 0, 0, 0));
        fbo3.getTextureReference().bind();
        mappingShader.begin();
        //mesh.drawFaces();
        mappingShader.end();
        fbo3.getTextureReference().unbind();

        reportStream.str(""); reportStream.clear();
        reportStream << "custom anti-transformation" << endl;
	    ofDrawBitmapString(reportStream.str(), 20, 20);
    fbo4.end();
    fbo4.draw(w + 2*p, h + 2*p, w, h);

}

//--------------------------------------------------------------
void ofApp::exit(){
}
