#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

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

		void GalaxyBrush(float y, float speed);
		void BurstBalls(float x, float y, float intensity);

		ofxOscSender sender;
		ofxOscReceiver receiver;

		float oscfreq = 0.0f;
		float oscseq = 0.0f;
		float osckey = 0.0f;
		float oscmode = 0.0f;
		float oscwave = 0.0f;
		float oscvol = 0.0f;
		float oscnvol = 0.0f;
		float oscmetro = 0.0f;
		int oscpat [64];

		ofPolyline line [10];
		float lineDelay = 0.0f;
		
};
