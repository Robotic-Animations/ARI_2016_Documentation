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

void gauge(ofxGuiContainer * container, const string & name) {
	ofParameter<float> temp;
	container->add(temp.set(name, ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "width",140 },{ "height",100 } }));//replace max with actual motor max value
}

void power_gauge(ofxGuiContainer * container, const string & name) {
	ofParameter<float> temp;
	container->add(temp.set(name, 80, 0, 100), ofJson({ { "precision",2 },{ "width",40 },{ "height",100 } }));//access the battery levels
}

void alt_power_gauge(ofxGuiContainer * container, const string & name) {
	ofParameter<float> temp;
	container->add(temp.set(name, 80, 0, 100), ofJson({ { "precision",2 }/*,{ "width",40 },{ "height",100 }*/ }));//access the battery levels

	//ofParameter<int> temp;
	//container->add(temp.set(name, 70, 0, 100));
}

ofxGuiPanel * gui_panel(ofxGui & gui, const string & name, const float & width) {
	return gui.addPanel(name, ofJson({ { "width",ofGetWidth()*width } }));
}

ofxGuiPanel * gui_panel(ofxGui & gui, const string & name, const float & width, const float & height) {
	return gui.addPanel(name, ofJson({ {"width",ofGetWidth()*width}, {"height",ofGetWidth()*height} }));
}

ofxGuiPanel * gui_panel_debug(ofxGui & gui, const string & name) {
	return gui.addPanel(name, ofJson({ { "width",100 } }));
}

struct panel_info {
	panel_info() {}
	panel_info(const float & width) : width(width), height(0) {}
	panel_info(const float & width, const float & height) : width(width), height(height) {}
	float width;
	float height;
};
map<string, panel_info> panels_info;

float aspect_ratio;

//--------------------------------------------------------------
void ofApp::setup(){
	picture = ofImage("images/Sun.jpg");
	aspect_ratio = picture.getHeight() / picture.getWidth();

	ofSetFrameRate(60);

	panels_info = map<string, panel_info> {
		make_pair<string, panel_info>("Arm Controls", panel_info(.2)),
		make_pair<string, panel_info>("Cameras", panel_info(.5, .5*aspect_ratio)),
		make_pair<string, panel_info>("Motor Feedback", panel_info(.55)),
		make_pair<string, panel_info>("Hand Controls", panel_info(.2))
	};
	for (auto panel : panels_info) {
		if (panel.second.height) panels[panel.first] = gui_panel(gui, panel.first, panel.second.width, panel.second.height);
		else panels[panel.first] = gui_panel(gui, panel.first, panel.second.width);
	}

	//arm
	arm_FK = panels["Arm Controls"]->addGroup("Forward Kinematics");//a collapsable group of sliders controlling forward kinematics
	arm_IK = panels["Arm Controls"]->addGroup("Inverse Kinematics");//a collapsible group of sliders controlling inverse kinematics
	slider("X-Axis", arm_FK);
	slider("Y-Axis", arm_FK);
	slider("Z-Axis", arm_FK);
	slider("X-Axis", arm_IK);
	slider("Y-Axis", arm_IK);
	slider("Z-Axis", arm_IK);
	panels["Arm Controls"]->addFpsPlotter();

	//main camera
	panels["Cameras"]->setPosition(panels["Arm Controls"]->getShape().getTopRight() + ofPoint(20,0));
	main_Camera = panels["Cameras"]->addContainer("");
	draw_image(main_Camera, "Camera 1",  panels_info["Cameras"].width, panels_info["Cameras"].width*aspect_ratio, picture);

	other_Cameras = panels["Cameras"]->addContainer("", ofJson({ { "direction","horizontal" } }));
	draw_image(other_Cameras, "Camera 2", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);
	draw_image(other_Cameras, "Camera 3", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);
	draw_image(other_Cameras, "Depth Camera", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);

	//motor feedback
	panels["Motor Feedback"]->setPosition(panels["Cameras"]->getShape().getBottomLeft() + ofPoint(0,10));
	motor_Container = panels["Motor Feedback"]->addContainer("motors", ofJson({ {"direction","horizontal"} }));

	gauge(motor_Container, "Left Front");
	gauge(motor_Container, "Right Front");
	gauge(motor_Container, "Left Rear");
	gauge(motor_Container, "Right Rear");
	power_gauge(motor_Container, "Battery Level");

	//hand control
	hand.load("images/Hand Image.jpg");

	panels["Hand Controls"]->setPosition(panels["Cameras"]->getShape().getTopRight() + ofPoint(20, 0));

	hand_image = panels["Hand Controls"]->addGroup("3D Model");
	hand_image->add<ofxGuiGraphics>("Hand Model", &hand.getTexture(), ofJson({ { "width", ofGetWindowWidth()*.18 },{ "height", ofGetWindowHeight()*.13 } }));
	hand_control = panels["Hand Controls"]->addGroup("Hand Controls");
	gauge(hand_control, "Rotation");
	alt_power_gauge(hand_control, "Clamp");
}


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
	panels["Arm Controls"]->setWidth(ofGetWidth()*panels_info["Arm Controls"].width);

	panels["Cameras"]->setWidth(ofGetWidth()*panels_info["Cameras"].width);
	panels["Cameras"]->setHeight(ofGetWidth()*panels_info["Cameras"].width*aspect_ratio*.75);
	panels["Cameras"]->setPosition(panels["Arm Controls"]->getShape().getTopRight() + ofPoint(20, 0));
	main_Camera->setWidth(ofGetWidth()*panels_info["Cameras"].width);
	main_Camera->setHeight(ofGetWidth()*panels_info["Cameras"].width*aspect_ratio*.75);
	//figfure out how ot resize image, first drawn in "draw_image"

	panels["Motor Feedback"]->setPosition(panels["Cameras"]->getShape().getBottomLeft() + ofPoint(0, 10));
	panels["Hand Controls"]->setPosition(panels["Cameras"]->getShape().getTopRight() + ofPoint(20, 0));
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
