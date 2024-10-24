#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;

		float getMarblePos(float p);
		float getMarbleDist(float x, float z);
		float volumeCurve(float dist);
		void myDrawCylinder(float x, float z);
		void speedLimit(float& speed);
		void decreaser(float& speed);
		void colorLimit(float& c);
		void StarBrush(float x, float y, float z, float intensity);

		//// GUI ////

		ofEasyCam cam;
  		ofLight light;
		ofColor brightness;
  		bool bHide = false;
		ofxPanel gui;
		ofParameter <ofVec3f> uiMarbleRotation;
		ofParameter<float> layer1;
		ofParameter<float> layer2;
		ofParameter<float> layer3;
		ofParameter<float> visualdecay;
		ofParameter<float> speakerXPos;
		ofParameter<float> speakerZPos;

		//// ROTATION & MOVEMENT ////

		float zMov;
		float xMov;
		float maxSpeed;
		float curXSpeed;
		float curZSpeed;

		bool goingRight;
		bool goingLeft;
		bool goingFor;
		bool goingBack;
		float zRot;

		bool gridDrawn;

		//// SOUND STUFF ////

		ofSoundPlayer sound;
		ofSoundPlayer sound2;
		ofSoundPlayer sound3;

		float * fft;
		float * soundSpectrum;
		int bands;

		//// MISC ////

		//float speakerXPos;
		//float speakerZPos;

		float bgR;
		float bgB;
		float bgG;
		
};
