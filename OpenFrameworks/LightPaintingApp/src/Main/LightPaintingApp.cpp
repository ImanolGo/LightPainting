/*
 *  LightPaintingApp.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */

#include "AppManager.h"

#include "LightPaintingApp.h"

//--------------------------------------------------------------
void LightPaintingApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void LightPaintingApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void LightPaintingApp::draw(){
    AppManager::getInstance().draw();
}

void LightPaintingApp::exit()
{
    ofLogNotice() <<"LightPaintingApp::exit";
    AppManager::getInstance().exit();
}

//--------------------------------------------------------------
void LightPaintingApp::keyPressed(int key){

}

//--------------------------------------------------------------
void LightPaintingApp::keyReleased(int key){

}

//--------------------------------------------------------------
void LightPaintingApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void LightPaintingApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void LightPaintingApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void LightPaintingApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void LightPaintingApp::windowResized(int w, int h)
{
    AppManager::getInstance().getLayoutManager().windowResized(w,h);
}

//--------------------------------------------------------------
void LightPaintingApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void LightPaintingApp::dragEvent(ofDragInfo dragInfo)
{
}
