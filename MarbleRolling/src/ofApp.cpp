#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetIcoSphereResolution(1);

    sound.load("SchmooveCityLayer1.wav");
    sound2.load("SchmooveCityLayer2.wav");
    sound3.load("SchmooveCityLayer3.wav");
    sound.play();
    sound2.play();
    sound3.play();
    sound.setLoop(true);
    sound2.setLoop(true);
    sound3.setLoop(true);

    gui.setup();
    gui.add(uiMarbleRotation.set("rotate", ofVec3f(0),ofVec3f(-180),ofVec3f(180)));
    gui.add(layer1.set("Layer 1", 0.5, 0.0, 1.0));
    gui.add(layer2.set("Layer 2", 0.0, 0.0, 1.0));
    gui.add(layer3.set("Layer 3", 0.0, 0.0, 1.0));
    gui.add(visualdecay.set("visualdecay", 0.5, 0.0, 1.0));
    gui.add(speakerXPos.set("X Pos", 0.0, -1000.0, 1000.0));
    gui.add(speakerZPos.set("Z Pos", -200.0, -1000.0, 1000.0));
    //zRot = uiMarbleRotation->z;

    maxSpeed = 0.05f;
    curXSpeed = 0;
    curZSpeed = 0;
    gridDrawn = false;

    fft = new float[512];
    for (int i = 0; i < 512; i++) {
        fft[i] = 0;
    }
    bands = 256;
}

//--------------------------------------------------------------
void ofApp::update(){
    //ofLog(OF_LOG_NOTICE, "X rot: "+ofToString(zMov)+"\tZ rot: "+ofToString(xMov));
    //ofLog(OF_LOG_NOTICE, "cos(xMov): "+ofToString(cos(xMov))+"\tsin(xMov): "+ofToString(sin(xMov)));
    //ofLog(OF_LOG_NOTICE, "Distance: "+ofToString(getMarbleDist(zMov,xMov)));
    //ofLog(OF_LOG_NOTICE, "fft[4]: "+ofToString(ofMap(fft[4],0,1,150,255)));
    
    ofSoundUpdate();

    sound.setVolume(layer1*volumeCurve(getMarbleDist(zMov,xMov)));
    sound2.setVolume(layer2*volumeCurve(getMarbleDist(zMov,xMov)));
    sound3.setVolume(layer3*volumeCurve(getMarbleDist(zMov,xMov)));
    soundSpectrum = ofSoundGetSpectrum(bands);
    for(int i = 0; i < bands; i++) {
        fft[i] *= visualdecay;
        if(fft[i] < soundSpectrum[i]) {
            fft[i] = soundSpectrum[i];
        }
    }
    if(goingRight) {
        curXSpeed+=0.0014f;
    }
    if(goingLeft) {
        curXSpeed-=0.0014f;
    }
    if(goingFor) {
        curZSpeed-=0.0014f;
    }
    if(goingBack) {
        curZSpeed+=0.0014f;
    }
    speedLimit(curXSpeed);
    speedLimit(curZSpeed);
    xMov+=curXSpeed;
    zMov+=curZSpeed;
    
}

void ofApp::speedLimit(float& speed) {
    if(speed >= maxSpeed) {
        speed = maxSpeed;
    }
    if(speed <= -maxSpeed) {
        speed = -maxSpeed;
    }
    if(speed > 0) {
        speed -= 0.0004f;
    }
    if(speed < 0) {
        speed += 0.0004f;
    }
}

void ofApp::colorLimit(float& c) {
    if (c < 0) {
        c = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    float fftLowSum = 0;
    for (int i = 0; i < 8; i++) {
        fftLowSum += fft[i];
    }
    float threshold = layer1+layer2*0.5f+layer3;
    if(fftLowSum > (threshold)+0.5f) {
        bgR = 200 - getMarbleDist(zMov,xMov)/12.5f;
        bgG = 150 - getMarbleDist(zMov,xMov)/17.f;
        bgB = 40 - getMarbleDist(zMov,xMov)/65.f;
    }
    if(bgR + bgG + bgB > 10) {
        bgR -= 2.f;
        bgG -= 3.f;
        bgB -= 0.5f;
    }
    colorLimit(bgR);
    colorLimit(bgG);
    colorLimit(bgB);
    //ofLog(OF_LOG_NOTICE, ofToString(bgR)+", "+ofToString(bgG)+", "+ofToString(bgB));
    ofBackgroundGradient(ofColor(bgR,bgG,bgB), ofColor(45,0,60),OF_GRADIENT_CIRCULAR);
    int gridSize = 30;
    ofTranslate(ofGetWidth()/2-getMarblePos(xMov), ofGetHeight()/2+140,60-getMarblePos(zMov));
    ofDrawGrid(gridSize, 500, false, false, true, false);
    for(int i = 0; i < bands; i+=4) {
        for(int j = 0; j < bands; j++) {
            int rectWidth = 8;
            float rectX = j*rectWidth+getMarblePos(xMov)-ofGetWidth()/2+48;
            float rectY = -127;
            float rectZ = getMarblePos(zMov);
            float rectHeight = -fft[j]*500-5;
            /*if (freqRect.height < -ofGetHeight()*0.5f) {
                freqRect.height *= 0.8f;
            }*/
            ofFill();
            if(j >= 100 && fft[j] > 0.015f) {
                ofSetColor(0, 255, 200+fft[j]*70);
                StarBrush(getMarblePos(xMov)-ofGetWidth()/2+ofRandom(60, 1600), ofRandom(-550, -145), getMarblePos(zMov), fft[j]);
                //ofDrawCircle(getMarblePos(xMov)-ofGetWidth()/2+ofRandom(60, 1600), ofRandom(-550, -130), getMarblePos(zMov), 4.5f);
            }
            ofSetColor(200, 50+j*1.5f, 100+fft[j]*700);
            ofDrawRectangle(rectX, rectY, rectZ, rectWidth, rectHeight);
        }
        
    }
    if(getMarblePos(zMov)>=speakerZPos) {
        myDrawCylinder(speakerXPos, speakerZPos);
    }
    ofTranslate(getMarblePos(xMov), 0,getMarblePos(zMov));
    //ofRotateXDeg(uiMarbleRotation->x);
    //ofRotateYDeg(uiMarbleRotation->y);
    //ofRotateZDeg(uiMarbleRotation->z);
    ofRotateZRad(xMov);
    ofRotateRad(zMov,-cos(xMov),sin(xMov),0);
    
    ofSetColor(255);
    ofNoFill();
    ofDrawIcoSphere(0,0,0,50);
    // to draw cylinder in front of marble when you're behind it

    ofSetColor(0,255,0,180);
    ofDrawCylinder(1,120);
    ofRotateZDeg(90);
    ofSetColor(0,0,255,180);
    ofDrawCylinder(1,120);
    ofRotateZDeg(-90);
    ofRotateXDeg(90);
    ofSetColor(255,0,0,180);
    ofDrawCylinder(1,120);
    ofRotateXDeg(-90);
    ofPopMatrix();
    if(getMarblePos(zMov)<speakerZPos) {
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2-getMarblePos(xMov), ofGetHeight()/2+140,60-getMarblePos(zMov));
        myDrawCylinder(speakerXPos, speakerZPos);
        ofTranslate(getMarblePos(xMov), 0,getMarblePos(zMov));
        ofPopMatrix();
    }

    if (bHide){
        gui.draw();
    }

}

void ofApp::StarBrush(float x, float y, float z, float intensity) {
    //ofLog(OF_LOG_NOTICE, ofToString(intensity)+", "+ofToString(255-intensity*1400)+", "+ofToString(120+intensity*1200));
    int numLines = int(ofMap(intensity, 0.015f, 0.045f, 25, 50));
    int minRadius = 2;
    int maxRadius = 25;
    for (int i=0; i<numLines; i++) {
        float angle = ofRandom(ofDegToRad(360.0));
        float distance = ofRandom(minRadius, maxRadius);
        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        float alpha = ofMap(distance, minRadius, maxRadius, ofMap(intensity, 0, 0.05f, 100, 150), ofMap(intensity, 0, 0.05f, 20, 50));  // Make shorter lines more opaque
        ofSetColor(200-ofMap(intensity, 0, 0.04f, 0, 200), ofMap(intensity, 0.015f, 0.05f, 0, 180), 120+intensity*ofMap(intensity, 0.015f, 0.05f, 80, 255), alpha);
        ofDrawLine(x, y, z, x+xOffset, y+yOffset, z);
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h') {
        bHide = !bHide;
    }
    //have a pointer to the z rotation of marble
    //while d is pressed, it increases
    //while a is pressed, it decreases
    if(key == 'd') {
        goingRight = true;
    }
    if(key == 'a') {
        goingLeft = true;
    }
    if(key == 'w') {
        goingFor = true;
    }
    if(key == 's') {
        goingBack = true;
    }
    if(key == 'l') {
        maxSpeed = 0.12f;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'd') {
        goingRight = false;
    }
    if(key == 'a') {
        goingLeft = false;
    }
    if(key == 'w') {
        goingFor = false;
    }
    if(key == 's') {
        goingBack = false;
    }
    if(key == 'l') {
        maxSpeed = 0.05f;
    }

}

float ofApp::getMarblePos(float p) {
    return p*30;
}

float ofApp::getMarbleDist(float x, float z) {
    return sqrt(pow(speakerXPos-getMarblePos(z),2)+pow(speakerZPos-getMarblePos(x),2));
}

float ofApp::volumeCurve(float dist) {
    int maxDist = 2500;
    int minDist = 200;
    if (dist < minDist) {
        return 1.5;
    }
    else if (dist > maxDist) {
        return 0;
    }
    else {
        return ((dist - maxDist)*1.5)/(maxDist-minDist);
    }
}

void ofApp::myDrawCylinder(float x, float z) {
    ofSetColor(0,50,125);
    ofFill();
    ofDrawCylinder(x, -100, z+100, 55, 200);
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
