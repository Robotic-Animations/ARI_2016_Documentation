#include "ofApp.h"

void slider(const string & name, ofxGuiGroup * panel) {
	ofParameter<float> fk;
	ofxGuiContainer *buttons;
	panel->add(fk.set(name, 50, 0, 360), ofJson({ { "precision",2 } }));//these will access the position of the stepper motors
	buttons = panel->addContainer("", ofJson({ { "direction","horizontal" } }));//adds buttons to fine tune arm position
	buttons->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } })); //access panel width
	buttons->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
}

void draw_image(ofxGuiContainer * panel, const string & name, const float & x, const float & y, ofImage & picture) {
	panel->add<ofxGuiGraphics>(name, &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*x },{ "height", ofGetWindowHeight()*y } }));
}

float aspect_ratio;

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);

	arm_Panel = gui.addPanel("Arm Controls", ofJson({ { "width",ofGetWidth()*arm_panel_width } }));//add a panel to the gui

	arm_FK = arm_Panel->addGroup("Forward Kinematics");//a collapsable group of sliders controlling forward kinematics
	slider("X-Axis", arm_FK);
	slider("Y-Axis", arm_FK);
	slider("Z-Axis", arm_FK);

	arm_IK = arm_Panel->addGroup("Inverse Kinematics");//a collapsible group of sliders controlling inverse kinematics
	slider("X-Axis", arm_IK);
	slider("Y-Axis", arm_IK);
	slider("Z-Axis", arm_IK);

	arm_Panel->addFpsPlotter();

	//main camera
	picture = ofImage("images/Sun.jpg");
	aspect_ratio = picture.getHeight() / picture.getWidth();
	camera_Panel = gui.addPanel("Cameras", ofJson({ { "width", ofGetWindowWidth()*camera_panel_width }, {"height", ofGetWindowWidth()*camera_panel_width*aspect_ratio} }));//do not set the width *and* height, it distorts the aspect ratio of the image
	camera_Panel->setPosition(arm_Panel->getShape().getTopRight() + ofPoint(20,0));

	main_Camera = camera_Panel->addContainer("");
	draw_image(main_Camera, "Camera 1",  camera_panel_width, camera_panel_width*aspect_ratio, picture);

	other_Cameras = camera_Panel->addContainer("", ofJson({ { "direction","horizontal" } }));
	draw_image(other_Cameras, "Camera 2", .16, .12, picture);
	draw_image(other_Cameras, "Camera 3", .16, .12, picture);
	draw_image(other_Cameras, "Depth Camera", .16, .12, picture);

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
msc.addSlave(control);*/

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
	arm_Panel->setWidth(ofGetWidth()*arm_panel_width);
	camera_Panel->setWidth(ofGetWidth()*camera_panel_width);
	camera_Panel->setHeight(ofGetWidth()*camera_panel_width*aspect_ratio*.75);
	camera_Panel->setPosition(arm_Panel->getShape().getTopRight() + ofPoint(20, 0));
	main_Camera->setWidth(ofGetWidth()*camera_panel_width);
	main_Camera->setHeight(ofGetWidth()*camera_panel_width*aspect_ratio*.75);
	//auto a = main_Camera->children();
	//a[0]->setAttribute("height", ofGetWindowHeight()*ofGetWidth()*camera_panel_width*aspect_ratio*10000);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
