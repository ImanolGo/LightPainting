/*
 *  LedsManager.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */




#include "ofMain.h"

#include "LedsManager.h"
#include "AppManager.h"


const string LedsManager::LEDS_LIST_PATH = "leds/";


LedsManager::LedsManager(): Manager(), m_ledsSize(2.0)
{
    //Intentionally left empty
}


LedsManager::~LedsManager()
{
    this->saveLedsPositions();
    ofLogNotice() <<"LedsManager::Destructor" ;
}


void LedsManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->setupLeds();
    
    ofLogNotice() <<"LedsManager::initialized" ;
    
}


void LedsManager::setupLeds()
{
    this->createLedsPosition();
    //this->readLedsPositions();
    //this->normalizeLeds();
}

void LedsManager::createLedsPosition()
{
    auto numLeds = AppManager::getInstance().getSettingsManager().getNumLeds();
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    int margin = width/50;
    float frame_width = width - 2*margin;
    float w = frame_width/numLeds;
    
    for(int i=0; i<numLeds; i++)
    {
        float x = margin + i*w; x/=width;
        float y = height*0.5; y/=height;
        
        this->createLed(ofPoint(x,y));
    }
}

void LedsManager::addLed(const ofPoint& position)
{
    string name = "Leds";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    
    ofRectangle windowRect(rect->x, rect->y, rect->width, rect->height);
    createLed(position);
    
    if (!m_leds.empty()) {
        m_leds.back()->normalize(windowRect);
    }
    
    ofLogNotice() <<"LedsManager::addLed-> id = " <<  m_leds.back()->getId();
}

void LedsManager::deleteLastLed()
{
    if(!m_leds.empty()){
        int id = m_leds.back()->getId();
        m_leds.pop_back();
        
        ofLogNotice() <<"LedsManager::deleteLastLed-> id = " << id;
    }
}

void LedsManager::readLedsPositions()
{
    
    string led_position_path = LEDS_LIST_PATH + "LedPositions.txt";
    ofBuffer buffer = ofBufferFromFile(led_position_path);
    
    
    if(buffer.size())
    {
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
        {
            string line = *it;
            ofPoint ledPosition;
            
            if(parseLedLine(line,ledPosition))
            {
                createLed(ledPosition);
            }
            
        }
    }
    
}


void LedsManager::saveLedsPositions()
{
    string led_position_path = LEDS_LIST_PATH + "LedPositions.txt";
    ofFile ledPosFile(led_position_path, ofFile::WriteOnly, true);
    
    for(auto led: m_leds)
    {
        ofPoint pos =led->getPosition();
        ledPosFile << "{" << pos.x << ", " << pos.y << ", 0.0}" <<std::endl;
    }
    
    ledPosFile.close();
    
    ofLogNotice() <<"LedsManager::saveLedsPositions -> saved led positions to file " <<  led_position_path;
    
}

void LedsManager::createLed(const ofPoint& position)
{
    ofPtr<Led> led = ofPtr<Led> (new Led ( position, m_leds.size() + 1) );
    led->setColor(ofColor::black);
    led->setWidth(m_ledsSize);
    m_leds.push_back(led);
    
    
    ofLogNotice() <<"LedsManager::createLed -> id " << led->getId() << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y;
}

bool LedsManager::parseLedLine(string& line, ofPoint& position)
{
    if(line.size() == 0){
        return false;
    }
    
    char chars[] = "{}";
    removeCharsFromString(line, chars);
    vector <string> strings = ofSplitString(line, ", " );
    
    position.x = ofToFloat(strings[0]);
    position.y = ofToFloat(strings[1]);
    position.z = ofToFloat(strings[2]);
    
    return true;
}

void LedsManager::removeCharsFromString( string &str, char* charsToRemove ) {
    for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}

void LedsManager::update()
{
    this->updateLeds();
}

void LedsManager::updateLeds()
{
    for(int i = 0; i<m_leds.size(); i++){
        float percentage = float(i)/m_leds.size();
        auto color = AppManager::getInstance().getImageManager().getColor(percentage);
        this->setColor(i, color);
    }
}

void LedsManager::setPixels(ofPixelsRef pixels)
{
    this->setLedColors(pixels);
}

void LedsManager::setLedColors(ofPixelsRef pixels)
{
    for(auto led: m_leds){
        led->setPixelColor(pixels);
    }
}


void LedsManager::draw()
{
    this->drawLeds();
}

void LedsManager::draw(int width, int height)
{
    this->drawLeds(width, height);
}

void LedsManager::drawLeds(int width, int height)
{
    for(auto led: m_leds)
    {
        led->draw(width,height);
    }
}

void LedsManager::onSetLedsSize(float &value)
{
    m_ledsSize = value;
    
    for(auto led: m_leds){
        led->setWidth(m_ledsSize);
    }
}

void LedsManager::showChannels(bool _showChannels)
{
    for(auto led: m_leds)
    {
        led->showId(_showChannels);
    }
}

void  LedsManager::setColor(int index, const ofColor& color)
{
    if(index<0 || index>=m_leds.size()){
        return;
    }
    
    m_leds[index]->setColor(color);
}




