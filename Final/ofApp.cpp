#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetBackgroundAuto(false);
  ofSetFrameRate(60);
  sender.setup("localhost", 1337);
  receiver.setup(1337);
  lineDelay = ofGetWidth()/2;
}

//--------------------------------------------------------------
void ofApp::update(){
  while(receiver.hasWaitingMessages()){
    ofxOscMessage msg;
    receiver.getNextMessage(msg);
    if (msg.getAddress() == "/freq"){
      oscfreq = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/seq"){
      oscseq = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/key"){
      osckey = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/mode"){
      oscmode = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/ws"){
      oscwave = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/vol"){
      oscvol = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/nvol"){
      oscnvol = msg.getArgAsFloat(0);
    }
    if (msg.getAddress() == "/metro"){
      oscmetro = msg.getArgAsFloat(0);
    }
    for(int i = 0; i<64; i++) {
      if (msg.getAddress() == "/pat"){
        oscpat[i] = msg.getArgAsInt(i);
      }
    }
  }
  lineDelay = ofLerp(lineDelay, oscfreq, 0.2f);
  for(int i=0; i<10; i++) {
    line[i].addVertex(lineDelay*2-350, oscvol*0.8f+20, oscnvol*5);
    if (line[i].size() > 100) {
      line[i].getVertices().erase(line[i].getVertices().begin());
    }
    for (auto &vert : line[i].getVertices()){
      vert.x += (oscwave+1)*oscnvol-i;
      vert.y += (oscwave*2+1)*ofRandom(-1,1);
    }
  }
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofFill();
  ofBackgroundGradient(ofColor(180-ofMap(oscmode, 0, 7, 0, 50),0,ofMap(oscmode, 0, 7, 0, 130)),
                      ofColor(0,ofMap(oscmode, 0, 7, 0, 150),
                      ofMap(oscmode, 0, 7, 150, 0)),OF_GRADIENT_CIRCULAR);
  GalaxyBrush(200, 7);
  GalaxyBrush(300, -29);
  GalaxyBrush(400, 17);
  GalaxyBrush(500, -11);
  GalaxyBrush(600, 23);
  ofPushMatrix();
  ofNoFill();
  ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
  ofRotateDeg(ofGetElapsedTimef()*ofMap(oscmetro, 200, 1000, 100, 0.01), 0, 1, 0);
  ofSetColor(255,255,0,90);
  ofDrawIcoSphere(0, 0, 800);
  ofPopMatrix();
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      ofNoFill();
      ofSetColor(0);
      float xPos = j*80+ofGetWidth()/2-300;
      float yPos = i*80+100;
      ofDrawRectangle(xPos,yPos,41,41);
      ofFill();
      if((oscseq-1) == j) {
        ofSetColor(255, oscpat[(i*8)+j]*(oscvol*0.5f));
        ofDrawRectangle(xPos,yPos,40,40);
      }
    }
  }
  BurstBalls(oscseq*120, 350-oscfreq*0.5f, oscwave);
  for(int i=0; i<10; i++) {
    ofSetColor(255,180);
    line[i].draw();
  }
  
}

//--------------------------------------------------------------
void ofApp::exit(){

}

void ofApp::GalaxyBrush(float y, float speed) {
    float randomColor = ofRandom(210,250)*(ofMap(oscwave, 0, 5, 1, 0.1667f));
    int mistAlpha = 10 + oscnvol*5;
    ofSetRectMode(OF_RECTMODE_CENTER);
    for (int r=400; r>0; r-=3) {
        float width = ofRandom(15,50);
        float height = ofRandom(15,50);
        float distance = ofRandom(15,80);
        float angle = ofRandom(ofDegToRad(360.0));

        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        ofSetColor(randomColor,mistAlpha);
        ofDrawCircle(int(ofGetElapsedTimef()*speed)%200+xOffset*5+500, y+yOffset, width*4, height);
      }
}

void ofApp::BurstBalls(float x, float y, float intensity) {
    //ofLog(OF_LOG_NOTICE, ofToString(intensity)+", "+ofToString(255-intensity*1400)+", "+ofToString(120+intensity*1200));
    int numLines = int(ofMap(intensity, 0, 5, 5, 15));
    int minRadius = 20;
    int maxRadius = 80;
    for (int k=0; k<1; k++) {
      for (int j=0; j<8; j++) {
        x+=(ofRandom(0,60)-(oscseq-2)*10)*(2*k+1);
        y+=(ofMap(oscnvol, -1, 1, 0, 80)-10*(k-1));
        for (int i=0; i<numLines; i++) {
            float angle = ofRandom(ofDegToRad(360.0));
            float distance = ofRandom(minRadius, maxRadius)*(k+1);
            float xOffset = cos(angle) * distance;
            float yOffset = sin(angle) * distance;
            float cSize = ofMap(oscvol, 0, 800, 1, 30);
            //float alpha = ofMap(distance, minRadius, maxRadius, ofMap(intensity, 0, 0.05f, 100, 150), ofMap(intensity, 0, 0.05f, 20, 50));  // Make shorter lines more opaque
            ofSetColor(osckey*(12*(k+1))+60,180-osckey*20,0,oscvol*0.35f);
            ofDrawCircle(x+xOffset, y+yOffset, cSize, cSize);
        }
      }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
