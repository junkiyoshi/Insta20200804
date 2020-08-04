#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	int anime_rate = 90;
	auto param = ofGetFrameNum() % anime_rate;

	int radius = 300;
	int iterations = (ofGetFrameNum() % (anime_rate * 4)) / anime_rate;
	this->sphere = ofIcoSpherePrimitive(radius, iterations);

	this->mesh.clear();
	this->frame.clear();

	auto anime_param = 0.f;
	if (param > anime_rate * 0.4 && param <= anime_rate * 0.5) {

		anime_param = ofMap(param, anime_rate * 0.4, anime_rate * 0.5, 0, 1);
	}
	else if (param > anime_rate * 0.5 && param <= anime_rate * 0.9) {

		anime_param = 1;
	}
	else if (param > anime_rate * 0.9 && param <= anime_rate) {

		anime_param = ofMap(param, anime_rate * 0.9, anime_rate, 1, 0);
	}

	int radius_start = 65;
	int radius_max = 180;
	for (auto& triangle : this->sphere.getMesh().getUniqueFaces()) {

		auto mesh_index = this->mesh.getNumVertices();
		auto frame_index = this->frame.getNumVertices();

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		auto noise_value = ofNoise(avg);
		auto noise_location = avg + (avg * noise_value * anime_param);

		this->mesh.addVertex(noise_location + triangle.getVertex(0) - avg);
		this->mesh.addVertex(noise_location + triangle.getVertex(1) - avg);
		this->mesh.addVertex(noise_location + triangle.getVertex(2) - avg);

		this->mesh.addIndex(mesh_index + 0); this->mesh.addIndex(mesh_index + 1); this->mesh.addIndex(mesh_index + 2);

		this->frame.addVertex(glm::vec3());
		this->frame.addVertex(noise_location);

		this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index + 1);

	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.5);
	ofRotateY(ofGetFrameNum() * 0.32);

	ofSetColor(39);
	ofSetLineWidth(3);
	this->mesh.drawWireframe();
	
	ofSetColor(89);
	ofSetLineWidth(1.5);
	this->frame.drawWireframe();

	ofSetColor(239);
	this->mesh.draw();

	this->cam.end();

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}