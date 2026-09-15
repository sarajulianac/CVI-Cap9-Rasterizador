#include "ofApp.h"
#include <algorithm>
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("CVI Cap 9 - Rasterizador manual");
	ofBackground(25, 25, 30);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetCircleResolution(40);

	verts[0] = ofVec2f(ofGetWidth()*0.5f, ofGetHeight()*0.25f);
	verts[1] = ofVec2f(ofGetWidth()*0.25f, ofGetHeight()*0.75f);
	verts[2] = ofVec2f(ofGetWidth()*0.75f, ofGetHeight()*0.75f);

	vertColors[0] = ofColor(255, 80, 80);
	vertColors[1] = ofColor(80, 255, 120);
	vertColors[2] = ofColor(90, 140, 255);

	draggedVertex = -1;
	antialiasingOn = true;
	cullingOn = false;
	signedArea = 0;
	wasCulled = false;
	pixelsRasterized = 0;

	pixelMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
float ofApp::edgeFunction(const ofVec2f& a, const ofVec2f& b, const ofVec2f& p){
	return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

//--------------------------------------------------------------
void ofApp::rasterize(){

	int w = ofGetWidth();
	int h = ofGetHeight();

	pixelMesh.clear();
	pixelMesh.setMode(OF_PRIMITIVE_TRIANGLES);

	pixelsRasterized = 0;

	signedArea = edgeFunction(verts[0], verts[1], verts[2]);

	wasCulled = (cullingOn && signedArea <= 0);
	if (wasCulled || fabs(signedArea) < 0.0001f){
		return;
	}

	float rawMinX = std::min(std::min(verts[0].x, verts[1].x), verts[2].x);
	float rawMaxX = std::max(std::max(verts[0].x, verts[1].x), verts[2].x);
	float rawMinY = std::min(std::min(verts[0].y, verts[1].y), verts[2].y);
	float rawMaxY = std::max(std::max(verts[0].y, verts[1].y), verts[2].y);

	int minX = (int) ofClamp(rawMinX, 0.0f, (float)(w-1));
	int maxX = (int) ofClamp(rawMaxX, 0.0f, (float)(w-1));
	int minY = (int) ofClamp(rawMinY, 0.0f, (float)(h-1));
	int maxY = (int) ofClamp(rawMaxY, 0.0f, (float)(h-1));

	const int numSamples = 4;
	const ofVec2f sampleOffsets[4] = {
		ofVec2f(0.25f, 0.25f), ofVec2f(0.75f, 0.25f),
		ofVec2f(0.25f, 0.75f), ofVec2f(0.75f, 0.75f)
	};

	for (int y = minY; y <= maxY; y++){
		for (int x = minX; x <= maxX; x++){

			int samplesToTest = antialiasingOn ? numSamples : 1;
			int insideCount = 0;
			float accR = 0, accG = 0, accB = 0;

			for (int s = 0; s < samplesToTest; s++){
				ofVec2f p;
				if (antialiasingOn){
					p = ofVec2f(x + sampleOffsets[s].x, y + sampleOffsets[s].y);
				} else {
					p = ofVec2f(x + 0.5f, y + 0.5f);
				}

				float w0 = edgeFunction(verts[1], verts[2], p);
				float w1 = edgeFunction(verts[2], verts[0], p);
				float w2 = edgeFunction(verts[0], verts[1], p);

				bool inside = (w0 >= 0 && w1 >= 0 && w2 >= 0) ||
				              (w0 <= 0 && w1 <= 0 && w2 <= 0);

				if (inside){
					insideCount++;
					float bw0 = w0 / signedArea;
					float bw1 = w1 / signedArea;
					float bw2 = w2 / signedArea;

					accR += bw0*vertColors[0].r + bw1*vertColors[1].r + bw2*vertColors[2].r;
					accG += bw0*vertColors[0].g + bw1*vertColors[1].g + bw2*vertColors[2].g;
					accB += bw0*vertColors[0].b + bw1*vertColors[1].b + bw2*vertColors[2].b;
				}
			}

			if (insideCount > 0){
				pixelsRasterized++;
				float coverage = (float)insideCount / samplesToTest;
				ofColor fragColor(accR/insideCount, accG/insideCount, accB/insideCount);

				ofColor finalColor = fragColor;
				finalColor.a = (unsigned char)(255 * coverage);

				float x0 = (float)x, y0 = (float)y, x1 = x0+1.0f, y1 = y0+1.0f;
				pixelMesh.addVertex(glm::vec3(x0,y0,0)); pixelMesh.addColor(finalColor);
				pixelMesh.addVertex(glm::vec3(x1,y0,0)); pixelMesh.addColor(finalColor);
				pixelMesh.addVertex(glm::vec3(x1,y1,0)); pixelMesh.addColor(finalColor);

				pixelMesh.addVertex(glm::vec3(x0,y0,0)); pixelMesh.addColor(finalColor);
				pixelMesh.addVertex(glm::vec3(x1,y1,0)); pixelMesh.addColor(finalColor);
				pixelMesh.addVertex(glm::vec3(x0,y1,0)); pixelMesh.addColor(finalColor);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	rasterize();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(25, 25, 30);
	ofFill();
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	pixelMesh.draw();

	for (int i = 0; i < 3; i++){
		ofSetColor(255);
		ofNoFill();
		ofDrawCircle(verts[i], 9);
		ofFill();
		ofSetColor(vertColors[i]);
		ofDrawCircle(verts[i], 5);
	}

	ofSetColor(255);
	stringstream ss;
	ss << "CVI - Capitulo 9: The Graphics Pipeline" << endl;
	ss << "----------------------------------------" << endl;
	ss << "[a] Antialiasing (9.3): " << (antialiasingOn ? "ON  (supersampling 4x)" : "OFF (bordes con escalones)") << endl;
	ss << "[c] Culling (9.4):      " << (cullingOn ? "ON" : "OFF") << endl;
	ss << "[w] Invertir orden de vertices (winding)" << endl;
	ss << "----------------------------------------" << endl;
	ss << "Area con signo (edge function): " << ofToString(signedArea, 1) << endl;
	ss << "Orientacion: " << (signedArea > 0 ? "horaria (front-facing)" : "antihoraria (back-facing)") << endl;
	if (wasCulled){
		ss << ">>> TRIANGULO DESCARTADO por culling (9.4) <<<" << endl;
	}
	ss << "Pixeles rasterizados: " << pixelsRasterized << endl;
	ss << endl << "Arrastra los vertices con el mouse";

	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'a') antialiasingOn = !antialiasingOn;
	if (key == 'c') cullingOn = !cullingOn;
	if (key == 'w') std::swap(verts[1], verts[2]);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	draggedVertex = -1;
	for (int i = 0; i < 3; i++){
		if (verts[i].distance(ofVec2f(x, y)) < 12){
			draggedVertex = i;
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (draggedVertex != -1){
		verts[draggedVertex].set(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	draggedVertex = -1;
}