/*
 *  KeyboardManager.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */


#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager()
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogNotice() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
  
    if(key == 'g' || key == 'G') {
        AppManager::getInstance().getGuiManager().toggleGui();
    }
    
    
    else if(key == 'r' || key == 'R') {
        AppManager::getInstance().getTimeLineManager().reset();
    }
    
    else if(key == 's' || key == 'S')
    {
        AppManager::getInstance().getTimeLineManager().stop();
    }
    
    else if(key == 'p' || key == 'P')
    {
        AppManager::getInstance().getTimeLineManager().play();
    }
    
    else if(key == ' ')
    {
        AppManager::getInstance().getTimeLineManager().playPause();
    }
    
}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogNotice() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}










