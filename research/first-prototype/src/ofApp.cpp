#include "ofApp.h"
#include <math.h>
#include <ctype.h>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    listCameraDevices();
    
    nodes[0] = new nvrNode(720, 480, 0, 0);
    //nodes[1] = new nvrNode(320, 200, 0, 1);
    nodes[1] = new nvrNode(720, 480, 0, 1);
    for (int i= 2 ; i < INT_NODES_AMOUNT; i++)
        nodes[i] = new nvrNode(320, 240, 0, i);

    ofSetWindowTitle("netvr - prototype dashboard");

    bufferRoom.allocate(900, 400, GL_RGBA);
    roomMapping.load("shaders/mapping");
    roomCamera.setPosition(ofVec3f(0, INT_ROOM_HEIGHT, INT_ROOM_WIDTH+INT_ROOM_HEIGHT));
    roomCamera.setTarget(ofVec3f(0, INT_ROOM_HEIGHT/2.0, 0));
    roomCamera.setDistance(INT_ROOM_WIDTH+INT_ROOM_HEIGHT);

    lastTime = ofGetElapsedTimef();
    activeCamera = 0;

	setupGui();


    model.enableColors();
    model.enableMaterials();
    model.enableNormals();
    model.enableTextures();
    //model.loadModel("models/OBJ/spider.obj", true);
    //model.loadModel("models/Collada/duck.dae", true);
    model.loadModel("models/man/Man N070315.3DS", true);

    mappingShader.load("shaders/mapping");

}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++) {
        nodes[i]->update();
    }

    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor(100, 100, 100);
    ofColor edgeColor(40, 40, 40);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    drawCameras();
    drawRoom();
    gui.draw();
    
}

void ofApp::drawCameras(){
    for (int i= 0 ; i < guipNodesAmount; i++) {
        int w = 1200/guipNodesAmount, h = 400, x = 0 + i*w, y = 400;
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofNoFill();
        ofSetColor(255, 0, 0, 100);
        nodes[i]->draw(x, y, w, h);
        if (i == activeCamera - 1 || !activeCamera) {
            ofRect(x +2, y +2, w -4, h -4);
        }
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        ofDisableAlphaBlending();
    }
}

void ofApp::drawRoom(){

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


    bufferRoom.begin();
        ofBackground(0);
        ofEnableAlphaBlending();
        roomCamera.begin(ofRectangle(0, 0, 900, 400));
            ofEnableDepthTest();
            glShadeModel(GL_SMOOTH); 
            ofPushMatrix();
            ofTranslate(-INT_ROOM_WIDTH/2.0, 0, -INT_ROOM_DEPTH/2.0);
            roomWireframe.drawWireframe();
            glPointSize(4);
            roomWireframe.drawVertices();
            ofPopMatrix();

            ofEnableSeparateSpecularLight();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            ofPushMatrix();
            model.setPosition(0, 0, 0);
            model.setScale(0.5, -0.5, 0.5);
            ofPopMatrix();
            model.drawFaces();


            ofSpherePrimitive sphere;
            ofMesh cameraProjection;
            cameraProjection.addVertex(ofVec3f(0, 0, 0));
            cameraProjection.addVertex(ofVec3f(0, 0, INT_ROOM_DEPTH));
            cameraProjection.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, INT_ROOM_DEPTH));
            cameraProjection.addVertex(ofVec3f(0, 0, 0));
            cameraProjection.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, 0));
            cameraProjection.addVertex(ofVec3f(INT_ROOM_WIDTH, 0, INT_ROOM_DEPTH));

            cameraProjection.addTexCoord(ofVec2f(0, 0));
            cameraProjection.addTexCoord(ofVec2f(0, INT_ROOM_DEPTH));
            cameraProjection.addTexCoord(ofVec2f(INT_ROOM_WIDTH, INT_ROOM_DEPTH));
            cameraProjection.addTexCoord(ofVec2f(0, 0));
            cameraProjection.addTexCoord(ofVec2f(INT_ROOM_WIDTH, 0));
            cameraProjection.addTexCoord(ofVec2f(INT_ROOM_WIDTH, INT_ROOM_DEPTH));
            for (int i= 0 ; i < guipNodesAmount; i++) {

                ofPushMatrix();
                if (nodes[i]->guipShowFlow.get())
                    nodes[i]->bufferFlow.getTextureReference().bind();
                else
                    nodes[i]->bufferOutput.getTextureReference().bind();

                ofTranslate(nodes[i]->guipTargetPosition.get());
                ofRotateX(nodes[i]->guipTargetRotation.get().x);
                ofRotateY(nodes[i]->guipTargetRotation.get().y);
                ofRotateZ(nodes[i]->guipTargetRotation.get().z);
                ofTranslate(ofVec3f(-INT_ROOM_WIDTH/2.0, 0, -INT_ROOM_DEPTH/2.0));

                ofPushStyle();
                ofSetColor(0, 200, 200);
                ofLine(ofVec3f(INT_ROOM_WIDTH/2.0, 0, INT_ROOM_DEPTH/2.0), ofVec3f(INT_ROOM_WIDTH/2.0, 0, INT_ROOM_DEPTH/2.0) + 60*ofVec3f(0,1,0));
                ofPopStyle();


                mappingShader.begin();
                cameraProjection.draw();
                mappingShader.end();
                ofPopMatrix();

                ofPushStyle();
                ofSetColor(200, 0, 0);
                sphere.setRadius(10);
                sphere.setPosition(nodes[i]->guipTargetPosition);
                sphere.draw();
                ofPopStyle();
                ofPushStyle();
                ofSetColor(0, 200, 0);
                sphere.setRadius(20);
                sphere.setPosition(nodes[i]->guipCameraPosition);
                sphere.draw();
                ofPopStyle();
                ofPushStyle();
                ofSetColor(0, 0, 200);
                ofLine(nodes[i]->guipCameraPosition, nodes[i]->guipTargetPosition);
                ofPopStyle();
            }

            ofDisableDepthTest();

        roomCamera.end();
        ofDisableAlphaBlending();
    bufferRoom.end();
    bufferRoom.draw(0, 0);

    reportStream.str(""); reportStream.clear();
    reportStream << "3d room" << endl;
    ofDrawBitmapString(reportStream.str(), 0 +10, 0 +20);
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (isdigit(key)) {
        activeCamera = key -48;
    }
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	
	gui.setup("settings", "settings.xml", 920 + 10, 10);
	gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
	gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
	
	int guiColorSwitch = 0;
	ofColor guiHeaderColor[2];
	guiHeaderColor[0].set(160, 160, 80, 200);
	guiHeaderColor[1].set(80, 160, 160, 200);
	ofColor guiFillColor[2];
	guiFillColor[0].set(160, 160, 80, 200);
	guiFillColor[1].set(80, 160, 160, 200);

	prototypeParameters.setName("Prototype Params");
	prototypeParameters.add(guipNodesAmount.set("Amount of Nodes", 2, 1, INT_NODES_AMOUNT));
	gui.add(prototypeParameters);
	
    for (int i= 0 ; i < INT_NODES_AMOUNT; i++) {
        gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
        gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
        guiColorSwitch = 1 - guiColorSwitch;
        gui.add(nodes[i]->parameters);
    }
	
	gui.loadFromFile("settings.xml");
	gui.minimizeAll();
}
