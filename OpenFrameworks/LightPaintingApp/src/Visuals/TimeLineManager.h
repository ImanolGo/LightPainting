/*
 *  TimeLineManager.h
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"


//========================== class TimeLineManager =======================================
//==============================================================================
/** \class TimeLineManager TimeLineManager.h
 *	\brief class for managing the winches position and speed
 *	\details It defines all the winches IDs, calculates positions and speed in DMX values
 *           according to the documentation
 */


class TimeLineManager: public Manager
{
    enum PlaybackMode { PlayForward, PlayBackwards, Stop, Play};

public:
    //! Constructor
    TimeLineManager();

    //! Destructor
    virtual ~TimeLineManager();

    void setup();

    void update();
    
    float getCurrentTime() const {return m_time;}
    
    void onTimeChange(float& value)  { m_time = value;}
    
    void onDurationChange(float& value);
    
    void setPlayForward() {m_playDirection = PlayForward;}
    
    void setPlayBackwards() {m_playDirection = PlayBackwards;}
    
    void play();
  
    void reset();
    
    void stop();
    
    void playPause();
    
private:
    
    void updateTime();
    
    void checkPlayback();
   
private:
    

    float                       m_duration;
    float                       m_time;
    PlaybackMode                m_playMode;
    PlaybackMode                m_playDirection;
    
};

