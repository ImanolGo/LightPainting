/*
 *  TimeLineManager.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */


#include "TimeLineManager.h"
#include "AppManager.h"



TimeLineManager::TimeLineManager(): Manager(),  m_time(0), m_duration(10), m_playMode(Stop), m_playDirection(PlayForward)
{
    //Intentionally left empty
}

TimeLineManager::~TimeLineManager()
{
   ofLogNotice() << "TimeLineManager::destructor";
}


//--------------------------------------------------------------

void TimeLineManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    this->setupText();
    
    ofLogNotice() <<"TimeLineManager::initialized" ;
    
}


void TimeLineManager::setupText()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float size = 98;
    
    float w = width;
    float h = size;
    float x =  width*0.5;
    float y = height*0.5;
    ofPoint pos = ofPoint(x, y);
    string text = "00:00:000";
    string fontName = "fonts/open-sans/OpenSans-Bold.ttf";
    
    m_text = TextVisual(pos,w,h,true);
    m_text.setText(text, fontName, size);
   // m_text.setColor(ofColor::white);
    
    
    m_text2.load(fontName, size);
}


void TimeLineManager::update()
{
    this->updateTime();
    this->updateText();
}

void TimeLineManager::updateText()
{
    
    float time = 0.0;
    if(m_playDirection == PlayForward){
         time = (1 - m_time)*m_duration;
    }
    else{
         time = m_time*m_duration;
    }
    
    //float time = (1 - m_time)*m_duration;
    int minutes = time/60;
    int seconds = time - 60*minutes;
    int remain = (time - floor(time))*1000 ;
    m_timeString = ofToString(minutes,2, '0') + ":" + ofToString(seconds,2, '0') + ":" + ofToString(remain,3, '0');
}

void TimeLineManager::draw()
{
    //ofEnableAlphaBlending();
    this->drawText();
    //ofClear(255,0,0);
}

void TimeLineManager::drawText()
{
    //m_text.draw();
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    ofRectangle rect = m_text2.getStringBoundingBox("00:00:000", 0,0);
    
    m_text2.drawString(m_timeString, width*0.5 - rect.width*0.5, height*0.5 + rect.height*0.5);
}



void TimeLineManager::updateTime()
{
    AppManager::getInstance().getGuiManager().onCurrentTimeChange(m_time);
    
    if(m_playMode == Stop){
        return;
    }
    
    if(m_playDirection == PlayForward){
        m_time += ofGetLastFrameTime()/m_duration;
        this->checkPlayback();
    }
    else if(m_playDirection == PlayBackwards){
        m_time -= ofGetLastFrameTime()/m_duration;
        this->checkPlayback();
    }
}


void TimeLineManager::checkPlayback()
{
    if(m_playMode == Stop){
        return;
    }
    
    if(m_time>1.0){
        m_playMode = Stop;
        m_time = 1.0;
       
    }
    else if(m_time<0.0){
        m_playMode = Stop;
         m_time = 0.0;
    }
}


void TimeLineManager::onDurationChange(float& value)
{
    m_duration = ofClamp(value, 0, 360);
}


void TimeLineManager::onTimeChange(float& value)
{
    m_time = value;
    m_time = ofClamp(m_time, 0.0, 1.0);
    //ofLogNotice() <<"TimeLineManager::onTimeChange << time = " << m_time;
}

void TimeLineManager::reset()
{
    m_playMode = Stop;
    
    ofLogNotice() <<"TimeLineManager::Reset";
    if(m_playDirection == PlayForward){
        m_time = 0.0;
    }
    else if(m_playDirection == PlayBackwards){
        m_time = 1.0;
    }
}


void TimeLineManager::stop()
{
    m_playMode = Stop;
    ofLogNotice() <<"TimeLineManager::Stop" ;
}

void TimeLineManager::play()
{
    m_playMode = Play;
    ofLogNotice() <<"TimeLineManager::Play" ;
}

void TimeLineManager::playPause()
{
    if(m_playMode == Stop){
        m_playMode = Play;
        ofLogNotice() <<"TimeLineManager::Play" ;
        if(m_time >= 1.0 && m_playDirection == PlayForward){
            m_time = 0.0;
        }
        else if(m_time <= 0.0 && m_playDirection == PlayBackwards){
            m_time = 1.0;
        }
    }
    else if(m_playMode == Play){
        m_playMode = Stop;
        ofLogNotice() <<"TimeLineManager::Stop" ;
    }
}





