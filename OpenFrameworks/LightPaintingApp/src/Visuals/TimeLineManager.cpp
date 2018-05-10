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
    
    ofLogNotice() <<"TimeLineManager::initialized" ;
    
}


void TimeLineManager::update()
{
    this->updateTime();
}



void TimeLineManager::updateTime()
{
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
    
    if(m_time>=1.0){
        m_playMode = Stop;
        if(m_playDirection == PlayForward){
            m_time = 0.0;
        }
       
    }
    else if(m_time<=0.0){
        m_playMode = Stop;
        if(m_playDirection == PlayBackwards){
            m_time = 1.0;
        }
    }
}


void TimeLineManager::onDurationChange(float& value)
{
    m_duration = ofClamp(value, 0, 360);
}


void TimeLineManager::reset()
{
    m_playMode = Stop;
    
    if(m_playDirection == PlayForward){
        m_time = 0.0;
    }
    else if(m_playDirection == PlayForward){
        m_time = 1.0;
    }
}


void TimeLineManager::stop()
{
    m_playMode = Stop;
}

void TimeLineManager::play()
{
    m_playMode = Play;
}

void TimeLineManager::playPause()
{
    if(m_playMode == Stop){
        m_playMode = Play;
    }
    else if(m_playMode == Play){
        m_playMode = Stop;
    }
}




