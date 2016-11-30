#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"
//#include "ofxMasterSlaveControl.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

		ofxGui gui;

		map<string, ofxGuiPanel*> panels;
		ofxGuiGroup* arm_FK, *arm_IK;
		ofxGuiContainer* main_Camera, *other_Cameras;
		ofxGuiGroup* hand_control, *hand_image;
		ofxGuiContainer* motor_Container;

		ofImage hand;
		ofImage picture;
};
