#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"
#include "ofxMasterSlaveControl.h"

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

		ofxGuiPanel* arm_Panel;
		ofxGuiGroup *arm_FK, *arm_IK;
		ofParameter<float> FK_x, FK_y, FK_z, IK_x, IK_y, IK_z;
		ofParameter<int> test;
		ofxGuiContainer *FK_x_btns, *FK_y_btns, *FK_z_btns, *IK_x_btns, *IK_y_btns, *IK_z_btns;
		ofParameter<bool> FK_x_up, FK_y_up, FK_z_up, IK_x_up, IK_y_up, IK_z_up, FK_x_down, FK_y_down, FK_z_down, IK_x_down, IK_y_down, IK_z_down;  //meant for the more and less buttons
		ofxMasterSlaveControl msc;

		ofxGuiPanel* camera_Panel;
		ofxGuiContainer *main_Camera, *other_Cameras;
		//ofxGuiGroup *camera_1, *camera_2, *camera_3, *camera_4;
		ofImage picture;

		ofxGuiPanel* hand_Panel;
		ofxGuiGroup *hand_control, *hand_image;
		ofParameter<int> rotation, clamp;
		ofImage hand;

		ofxGuiPanel* motor_Panel;
		ofxGuiContainer *motor_Container, *rpm, *battery_Level;
		ofParameter<float> rpm_Right_Front, rpm_Right_Rear, rpm_Left_Front, rpm_Left_Rear, battery;
};
