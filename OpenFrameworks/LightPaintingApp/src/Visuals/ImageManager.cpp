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


ImageManager::ImageManager(): Manager(), m_brightness(1.0), m_rate(0.5), m_topMargin(0.0), m_bottomMargin(0.0), m_contrast(1.0), m_saturation(1.0), m_useBrcosa(true), m_hue(0.0)
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
    this->setupCursor();
    this->setupImages();
    this->setupShader();
    
    ofLogNotice() <<"ImageManager::initialized" ;
    
}

void ImageManager::setupCursor()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float h = height;
    float w = width/200;
    float x = 0;
    float y = h*0.5;
    
    m_cursor = ofPtr<RectangleVisual>(new RectangleVisual(ofPoint(x,y), w, h, true));

}

void ImageManager::setupShader()
{
    m_brcosaShader.load("shaders/brcosa_GLSL");
    ofLogNotice() <<"ImageManager::setupShader" ;
    
}

void ImageManager::setupImages()
{
    this->loadImages();
    
}

void ImageManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height,GL_RGB);
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_fboImage.allocate(width,height,GL_RGB);
    m_fboImage.begin(); ofClear(0); m_fboImage.end();
    
    
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
    
    dir.sort();
    
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
    this->reload();
    
    return true;
    
}

void ImageManager::setFbo()
{

    auto& texture = m_currentImage->getTexture();
    
    m_fbo.allocate(texture.getWidth() + 2,texture.getHeight(),GL_RGB);
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_fboImage.allocate(texture.getWidth(),texture.getHeight(),GL_RGB);
    m_fboImage.begin(); ofClear(0); m_fboImage.end();
    
    
    
    ofLogNotice() <<"ImageManager::setFbo -> w = " <<  m_fbo.getWidth() << ", h = " <<  m_fbo.getHeight();
}


void ImageManager::addImages(string& name)
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    ofPtr<ImageVisual> image = ofPtr<ImageVisual> (new ImageVisual());
    image->setResource(name, true);
    //image->setWidth(width);
    //image->setHeight(height);
    
    m_images[name] = image;
    m_imageNames.push_back(name);
    
        //auto& texture = m_currentImage.getTexture();
       // texture.readToPixels(m_pixels);
    
//    ofLogNotice() <<"ImageManager::setupImage -> w = " <<  m_pixels.getWidth() << ", h = " <<  m_pixels.getHeight();
    
    
}

void ImageManager::update()
{
    this->updateCursor();
    this->updateFbo();
    //this->updatePixels();
}


void ImageManager::updateCursor()
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
        //m_currentImage->setColor(ofColor(m_brightness*255));
        this->drawImage();
        this->drawRectangles();
    m_fbo.end();
}


void ImageManager::drawImage()
{
    m_fboImage.begin();
        m_brcosaShader.begin();
        //m_brcosaShader.setUniformTexture("tex0", tex1, 0);
        m_brcosaShader.setUniform3f("avgluma", 0.62,0.62,0.62);
        m_brcosaShader.setUniform1f("hue", m_hue);
        m_brcosaShader.setUniform1f("contrast", m_contrast);
        m_brcosaShader.setUniform1f("brightness", m_brightness);
        m_brcosaShader.setUniform1f("saturation", m_saturation);
        m_brcosaShader.setUniform1f("alpha", 1.0);
        m_currentImage->draw();
        m_brcosaShader.end();

    m_fboImage.end();
    
    m_fboImage.draw(1,0);
  
}

void ImageManager::drawRectangles()
{
    ofSetColor(0, 0, 0);
    ofDrawRectangle(0,0, m_fbo.getWidth(), m_topMargin*m_fbo.getHeight());
    ofDrawRectangle(0, m_fbo.getHeight(), m_fbo.getWidth(), -m_bottomMargin*m_fbo.getHeight());
}

void ImageManager::updatePixels()
{
    m_pixels.clear();
    auto& texture = m_fbo.getTexture();
    texture.readToPixels(m_pixels);
    
    ofLogNotice() <<"ImageManager::updatePixels -> w = " <<  m_pixels.getWidth() << ", h = " <<  m_pixels.getHeight();
    
}

ofColor ImageManager::getColor(float percentage)
{
    if(percentage<0 || percentage>1.0){
        return ofColor::black;
    }
    
    float time = AppManager::getInstance().getTimeLineManager().getCurrentTime();
    int x = (m_pixels.getWidth()-1) * time;
    int y = (m_pixels.getHeight()-1) * percentage;
    
    return m_pixels.getColor(x,y);;
}



void ImageManager::draw()
{
    this->drawFbo();
    m_cursor->draw();
}



void ImageManager::drawFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_fbo.draw(0,0, width,height);
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
    this->reload();
}


void ImageManager::setTopMargin(float & value)
{
    m_topMargin = value;
    this->reload();
}

void ImageManager::setBottomMargin(float & value)
{
   m_bottomMargin = value;
   this->reload();
}

void ImageManager::onBrightnessChange(float& value)
{
 
    m_brightness = value;
    this->reload();
}

void ImageManager::onHueChange(float& value)
{
    m_hue = value;
    this->reload();
}

void ImageManager::onSaturationChange(float& value)
{
    m_saturation = value;
    this->reload();
}

void ImageManager::onContrastChange(float& value)
{
    m_contrast = value;
    this->reload();
}

void ImageManager::reload()
{
    this->setFbo();
    this->updateFbo();
    this->updatePixels();
}




