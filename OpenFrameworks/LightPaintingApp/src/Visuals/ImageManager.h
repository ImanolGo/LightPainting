/*
 *  ImageManager.h
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */




#pragma once

#include "Manager.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"


//========================== class ImageManager ==============================
//============================================================================
/** \class ImageManager ImageManager.h
 *	\brief Class managing the video player visuals
 *	\details it creates and updates the video player
 */

class ImageManager: public Manager
{
     typedef  map<string, ofPtr<ImageVisual> >      ImageMap;
    
public:
    
    //! Constructor
    ImageManager();
    
    //! Destructor
    ~ImageManager();
    
    //! Setup the Video Manager
    void setup();
    
    //! Update the Video Manager
    void update();
    
    //! Draw the Video Manager
    void draw();
    
    int getHeight() const {return m_currentImage->getOriginalHeight();}
    
    void onBrightnessChange(float& value);
        
    const ImageMap& getImages() const {return m_images;}
    
    void setImage(const string& name);
    
    ofColor getColor(float percentage);
    
private:
    
    void setupFbo();
    
    void setupImages();
    
    void setupRectangle();
    
    bool loadImages();
    
    void addImages(string& name);
    
    string getImageName(const string& path);
    
    void setPixels();
    
    void updateFbo();
    
    void updateRectangle();
    
    void updateColors();
    
    void drawFbo();

    
private:
    
    
    ofFbo                       m_fbo;
    ImageMap                    m_images;
    ofPtr<ImageVisual>          m_currentImage;
    ofPtr<RectangleVisual>      m_cursor;
    ofPixels                    m_pixels;
    float                       m_brightness;
    float                       m_rate;
    
};



