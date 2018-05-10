/*
 *  ImageManager.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */



#include "ofMain.h"

#include "ImageManager.h"
#include "AppManager.h"
#include "ofQTKitPlayer.h"


ImageManager::ImageManager(): Manager(), m_brightness(1.0), m_rate(0.5)
{
    //Intentionally left empty
}


ImageManager::~ImageManager()
{
    ofLogNotice() <<"ImageManager::Destructor" ;
}


void ImageManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->setupFbo();
    this->setupRectangle();
    this->setupImages();
    
    ofLogNotice() <<"ImageManager::initialized" ;
    
}

void ImageManager::setupRectangle()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float h = height;
    float w = width/170;
    float x = 0;
    float y = h*0.5;
    
    m_cursor = ofPtr<RectangleVisual>(new RectangleVisual(ofPoint(x,y), w, h, true));

}

void ImageManager::setupImages()
{
    this->loadImages();
    
}

void ImageManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height,GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

bool ImageManager::loadImages()
{
    //some path, may be absolute or relative to bin/data
    string path = "images/leds/";
    
    ofDirectory dir(path);
    
    
    ofLogNotice() <<"ImageManager::loadImages ..." ;
    //only show png files
    dir.allowExt("png");
    dir.allowExt("jpeg");
    dir.allowExt("jpg");
    //populate the directory object
    
    
    if(dir.listDir()==0){
        ofLogNotice()<< "ImageManager::loadImages-> No image files found in: " << dir.getAbsolutePath();
        return false;
    }
    
    ofLogNotice()<< "ImageManager::loadImages-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "ImageManager::loadImages-> Size: " << dir.size();
    
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string name = this->getImageName(dir.getPath(i));
        ofLogNotice()<< "ImageManager::loadImages-> Loaded: " << name;
        AppManager::getInstance().getResourceManager().addTexture(name, dir.getPath(i));
        this->addImages(name);
    }
    
    
    m_currentImage = m_images.begin()->second;
    this->setPixels();
    
    return true;
    
}

void ImageManager::setPixels()
{
    m_pixels.clear();
    auto& texture = m_currentImage->getTexture();
    texture.readToPixels(m_pixels);
    
    ofLogNotice() <<"ImageManager::setPixels -> w = " <<  m_pixels.getWidth() << ", h = " <<  m_pixels.getHeight();
}


void ImageManager::addImages(string& name)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    ofPtr<ImageVisual> image = ofPtr<ImageVisual> (new ImageVisual());
    image->setResource(name);
    image->setWidth(width);
    image->setHeight(height);
    
    m_images[name] = image;
    
        //auto& texture = m_currentImage.getTexture();
       // texture.readToPixels(m_pixels);
    
//    ofLogNotice() <<"ImageManager::setupImage -> w = " <<  m_pixels.getWidth() << ", h = " <<  m_pixels.getHeight();
}

void ImageManager::update()
{
    this->updateFbo();
    //this->updateColors();
}


void ImageManager::updateRectangle()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    auto pos = m_cursor->getPosition();
    pos.x = width*AppManager::getInstance().getTimeLineManager().getCurrentTime();
    m_cursor->setPosition(pos);
}

void ImageManager::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        m_currentImage->draw();
        m_cursor->draw();
    m_fbo.end();
}

void ImageManager::updateColors()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    auto pos = m_cursor->getPosition();
    
   // float time = AppManager::getInstance().getTimeLineManager().getCurrentTime();
    int x = (m_pixels.getWidth()-1) * pos.x/width;
    
    for(int y=0; y<m_pixels.getHeight(); y++)
    {
        ofColor color = m_pixels.getColor(x,y);
        color.setBrightness(color.getBrightness()*m_brightness);
        //ofColor color = ofColor::white;
        AppManager::getInstance().getLedsManager().setColor(y, color);
    }
    
}

ofColor ImageManager::getColor(float percentage)
{
    if(percentage<0 || percentage>1.0){
        return ofColor::black;
    }
    
    float time = AppManager::getInstance().getTimeLineManager().getCurrentTime();
    int x = (m_pixels.getWidth()-1) * time;
    int y = (m_pixels.getHeight()-1) * percentage;
    
    ofColor color = m_pixels.getColor(x,y);
    color.setBrightness(color.getBrightness()*m_brightness);
    return color;
}



void ImageManager::draw()
{
    this->drawFbo();
}



void ImageManager::drawFbo()
{
    m_fbo.draw(0,0);
}

void ImageManager::onBrightnessChange(float& value)
{
    //ofColor color(value);
    //m_currentImage->setColor(color);
    m_brightness = value;
}

string ImageManager::getImageName(const string& path)
{
    std::vector<std::string> strs = ofSplitString(path, "/");
    strs = ofSplitString(strs.back(), ".");
    return strs.front();
}

void ImageManager::setImage(const string& name)
{
    if(m_images.find(name) == m_images.end()){
        return;
    }
    
    m_currentImage = m_images[name];
    this->setPixels();
}




