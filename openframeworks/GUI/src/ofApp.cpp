#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	float val = 50;
	ofSetFrameRate(60);

	//arm panel
	arm_Panel = gui.addPanel("Arm Controls", ofJson({ { "width",ofGetWidth()*.2 } }));//add a panel to the gui

	arm_FK = arm_Panel->addGroup("Forward Kinematics");//a collapsable group of sliders controlling forward kinematics
	arm_FK->add(FK_x.set("X-Axis", val, 0, 360), ofJson({ {"precision",2} }));//these will access the position of the stepper motors
	
	
	
	
	FK_x_btns = arm_FK->addContainer("FK-X", ofJson({ {"direction","horizontal"} }));//adds buttons to fine tune arm position
	FK_x_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } })); //access panel width
	FK_x_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	
	arm_FK->add(FK_y.set("Y-Axis", 100, 0, 360), ofJson({ { "precision",2 } }));
	FK_y_btns = arm_FK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));
	FK_y_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	FK_y_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));

	arm_FK->add(FK_z.set("Z-Axis", 150, 0, 360), ofJson({ { "precision",2 } }));
	FK_z_btns = arm_FK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));
	FK_z_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	FK_z_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));

	arm_IK = arm_Panel->addGroup("Inverse Kinematics");//a collapsible group of sliders controlling inverse kinematics
	arm_IK->add(IK_x.set("X-Axis", 50, 0, 360), ofJson({ { "precision",2 } }));
	IK_x_btns = arm_IK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));
	IK_x_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	IK_x_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));

	arm_IK->add(IK_y.set("Y-Axis", 100, 0, 360), ofJson({ { "precision",2 } }));
	IK_y_btns = arm_IK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));
	IK_y_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	IK_y_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));

	arm_IK->add(IK_z.set("Z-Axis", 150, 0, 360), ofJson({ { "precision",2 } }));
	IK_z_btns = arm_IK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));
	IK_z_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	IK_z_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));

	arm_Panel->addFpsPlotter();

	

	//main camera
	picture.load("images/Sun.jpg");
	camera_Panel = gui.addPanel("Cameras", ofJson({ { "width", ofGetWindowWidth()*.5 }}));//do not set the width *and* height
	camera_Panel->setPosition(arm_Panel->getShape().getTopRight() + ofPoint(20,0));

	main_Camera = camera_Panel->addContainer("Main Camera");
	other_Cameras = camera_Panel->addContainer("Other Cameras", ofJson({ { "direction","horizontal" } }));
	main_Camera->add<ofxGuiGraphics>("Camera 1", &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.51 }, { "height", ofGetWindowHeight()*.39 } }));
	other_Cameras->add<ofxGuiGraphics>("Camera 2", &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.16 }, { "height", ofGetWindowHeight()*.12 } }));
	other_Cameras->add<ofxGuiGraphics>("Camera 3", &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.16 }, { "height", ofGetWindowHeight()*.12 } }));
	other_Cameras->add<ofxGuiGraphics>("Depth Camera", &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.16 }, { "height", ofGetWindowHeight()*.12 } }));

	//motor feedback
	motor_Panel = gui.addPanel("Motor Feedback", ofJson({ {"width", ofGetWindowWidth()*.55} }));//make the panel horizontal
	motor_Panel->setPosition(camera_Panel->getShape().getBottomLeft() + ofPoint(0,10));

	motor_Container = motor_Panel->addContainer("motors", ofJson({ {"direction","horizontal"} }));

	rpm = motor_Container->addContainer("RPMs", ofJson({ {"direction","horizontal"} }));
	rpm->add(rpm_Left_Front.set("Left Front", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 }, {"type", "circular"}, { "width",140 }, { "height",100 } }));//replace max with actual motor max value
	rpm->add(rpm_Right_Front.set("Right Front", ofGetFrameRate() - 10, 0, 85), ofJson({ { "precision",2 }, {"type","circular"}, { "width",140 }, { "height",100 } }));
	rpm->add(rpm_Left_Rear.set("Left Rear", ofGetFrameRate() + 20, 0, 85), ofJson({ { "precision",2 }, { "type","circular" }, { "width",140 }, { "height",100 } }));
	rpm->add(rpm_Right_Rear.set("Right Rear", ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 }, { "type","circular" }, { "width",140 }, { "height",100 } }));

	battery_Level = motor_Container->addContainer("Battery Level", ofJson({ {"direction","horizontal"} }));
	battery_Level->add(battery.set("Battery Level", 80, 0, 100), ofJson({ { "precision",2 }, {"width",40}, {"height",100} }));//access the battery levels in volts + keep track of amperages?

	//hand control
	hand.load("images/Hand Image.jpg");
	hand_Panel = gui.addPanel("Hand Controls", ofJson({ { "width", ofGetWindowWidth()*.2 } }));
	hand_Panel->setPosition(camera_Panel->getShape().getTopRight() + ofPoint(20, 0));

	hand_image = hand_Panel->addGroup("3D Model");
	hand_image->add<ofxGuiGraphics>("Hand Model", &hand.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.18 },{ "height", ofGetWindowHeight()*.13 } }));

	hand_control = hand_Panel->addGroup("Hand Controls");
	hand_control->add(rotation.set("Rotation", 180, 0, 360), ofJson({ { "type","circular" },{ "width",100 },{ "height",100 } }));
	hand_control->add(clamp.set("Clamp", 70, 0, 100));
}


/*ofxGuiElement* control = arm_Panel->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
msc.addMaster(control);

control = arm_Panel->add(FK_x.set("slave1", 50, 0, 100));
msc.addSlave(control);


for (int i = 1; i <= 6; i++)
{
switch (i)
{
string ltr
}
FK_x_btns = arm_FK->addContainer("FK-X", ofJson({ { "direction","horizontal" } }));//adds buttons to fine tune arm position
FK_x_btns->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } })); //access panel width
FK_x_btns->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
}*/

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case 'f': {
		ofToggleFullscreen();
		break;
	}
	default: break;
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
