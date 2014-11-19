#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main() {
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1200, 800, OF_WINDOW);
	ofRunApp(new ofApp());
}
