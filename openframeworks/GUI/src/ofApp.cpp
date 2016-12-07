#include "ofApp.h"

void slider(const string & name, ofxGuiGroup * panel) {
	ofParameter<float> parameter;
	panel->add(parameter.set(name, 50, 0, 360), ofJson({ { "precision",2 } }));//these will access the position of the stepper motors
	ofxGuiContainer *buttons = panel->addContainer("", ofJson({ { "direction","horizontal" } }));//adds buttons to fine tune arm position
	buttons->add<ofxGuiButton>("Less", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
	buttons->add<ofxGuiButton>("More", ofJson({ { "type","fullsize" },{ "width",ofGetWidth()*.1 },{ "text-align","center" } }));
}

template <typename T>
void draw_image(T * panel, const string & name, const float & x, const float & y, ofImage & picture) {
	panel->add<ofxGuiGraphics>(name, &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*x },{ "height", ofGetWindowHeight()*y } }));
}

void gauge(ofxGuiContainer * container, const string & name) {
	ofParameter<float> temp;
	container->add(temp.set(name, ofGetFrameRate() + 10, 0, 85), ofJson({ { "precision",2 },{ "type", "circular" },{ "width",140 },{ "height",100 } }));
}

void power_gauge(ofxGuiContainer * container, const string & name, const int & width=0) {
	ofParameter<float> temp;
	container->add(temp.set(name, 80, 0, 100), ofJson({ { "precision",2 },{ "width",width } }));
}

ofxGuiPanel * gui_panel(ofxGui & gui, const string & name, const float & width) {
	return gui.addPanel(name, ofJson({ { "width",ofGetWidth()*width } }));
}

struct panel_dimensions {
	panel_dimensions() {}
	panel_dimensions(const float & width) : width(width), height(0) {}
	panel_dimensions(const float & width, const float & height) : width(width), height(height) {}
	float width;
	float height; //unused
};
std::pair<string, panel_dimensions> panel_pairer(const string & name, const float & width, const float & height=0) {
	return std::pair<string, panel_dimensions>(name, panel_dimensions(width, height));
}

//Globals, used in setup and update or callbacks. Maybe moved into the class?
map<string, panel_dimensions> panels_info;
float aspect_ratio;

//--------------------------------------------------------------
void ofApp::setup(){
	picture = ofImage("images/Sun.jpg");
	hand.load("images/Hand Image.jpg");
	aspect_ratio = picture.getHeight() / picture.getWidth();
	ofSetFrameRate(60);

	panels_info = map<string, panel_dimensions>{
		panel_pairer("Arm Controls", .2),
		panel_pairer("Cameras", .5, .5*aspect_ratio),
		panel_pairer("Motor Feedback", .55),
		panel_pairer("Hand Controls", .2),
	};
	for (auto panel : panels_info) { //mapping the panels info to the panels in the class, ofxGuiPanels
		panels[panel.first] = gui_panel(gui, panel.first, panel.second.width);
	}

	//Panel Initialization
	arm_FK = panels["Arm Controls"]->addGroup("Forward Kinematics");
	arm_IK = panels["Arm Controls"]->addGroup("Inverse Kinematics");
	main_Camera = panels["Cameras"]->addContainer("Main Camera");
	other_Cameras = panels["Cameras"]->addContainer("Other Cameras", ofJson({ { "direction","horizontal" } }));
	motor_Container = panels["Motor Feedback"]->addContainer("Motors", ofJson({ { "direction","horizontal" } }));
	hand_image = panels["Hand Controls"]->addGroup("3D Model");
	hand_control = panels["Hand Controls"]->addGroup("Hand Controls");

	//Positioning
	panels["Cameras"]->setPosition(panels["Arm Controls"]->getShape().getTopRight() + ofPoint(20, 0));
	panels["Motor Feedback"]->setPosition(panels["Cameras"]->getShape().getBottomLeft() + ofPoint(0, 10));
	panels["Hand Controls"]->setPosition(panels["Cameras"]->getShape().getTopRight() + ofPoint(20, 0));

	//Arms
	slider("X-Axis", arm_FK);
	slider("Y-Axis", arm_FK);
	slider("Z-Axis", arm_FK);
	slider("X-Axis", arm_IK);
	slider("Y-Axis", arm_IK);
	slider("Z-Axis", arm_IK);
	panels["Arm Controls"]->addFpsPlotter();

	//Cameras
	draw_image(main_Camera, "Camera 1",  panels_info["Cameras"].width, panels_info["Cameras"].width*aspect_ratio, picture);
	draw_image(other_Cameras, "Camera 2", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);
	draw_image(other_Cameras, "Camera 3", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);
	draw_image(other_Cameras, "Depth Camera", panels_info["Cameras"].width/3, panels_info["Cameras"].width*aspect_ratio/3, picture);

	//Motors
	gauge(motor_Container, "Left Front");
	gauge(motor_Container, "Right Front");
	gauge(motor_Container, "Left Rear");
	gauge(motor_Container, "Right Rear");
	power_gauge(motor_Container, "Battery Level", 40);

	//Hand control
	draw_image(hand_image, "Hand Model", .18f, .13f, hand);
	gauge(hand_control, "Rotation");
	power_gauge(hand_control, "Clamp", 200);
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
	panels["Arm Controls"]->setWidth(w*panels_info["Arm Controls"].width);
	panels["Motor Feedback"]->setWidth(w*panels_info["Motor Feedback"].width);
	panels["Hand Controls"]->setWidth(w*panels_info["Hand Controls"].width);

	panels["Cameras"]->setWidth(w*panels_info["Cameras"].width);
	panels["Cameras"]->setHeight(w*panels_info["Cameras"].width*aspect_ratio*.75);
	
	main_Camera->setWidth(w*panels_info["Cameras"].width);
	main_Camera->setHeight(w*panels_info["Cameras"].width*aspect_ratio*.75);
	
	//figure out how ot resize image, first drawn in "draw_image"
	//panel->add<ofxGuiGraphics>(name, &picture.getTexture(), ofJson({ { "width", ofGetWindowWidth()*x },{ "height", ofGetWindowHeight()*y } }));
	//ofxGuiGraphics temp = panels["Cameras"]->findChildrenById("Main Camera")[0]->findChildrenById("Camera 1")[0]->setWidth(w*panels_info["Cameras"].width);
	//docs don't help... good luck

	panels["Cameras"]->setPosition(panels["Arm Controls"]->getShape().getTopRight() + ofPoint(20, 0));
	panels["Motor Feedback"]->setPosition(panels["Cameras"]->getShape().getBottomLeft() + ofPoint(0, 10));
	panels["Hand Controls"]->setPosition(panels["Cameras"]->getShape().getTopRight() + ofPoint(20, 0));
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
