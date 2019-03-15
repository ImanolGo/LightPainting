/*
 *  GuiManager.cpp
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */



#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->setupImagesDropDown();
    this->setupImageGui();
    this->setupTimeLineGui();
    this->setupGuiEvents();
    this->loadGuiValues();
    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    ofxDatGuiLog::quiet();
    
    m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, true);
    
    // add some components //
    //m_gui.addLabel("PrimaveraSound GUI");
    
    m_gui.addFRM();
    m_gui.addButton("* Save GUI");
    
    m_gui.addToggle("Mirrored", true);
    
    m_gui.addBreak();
    
}


void GuiManager::setupImagesDropDown()
{
    string label = "IMAGES:";
    vector<string> opts;
    auto images = AppManager::getInstance().getImageManager().getImages();
    for(auto image: images){
        opts.push_back(image.first);
    }
    
    m_gui.addDropdown(label, opts);
    auto menu = m_gui.getDropdown(label);
    menu->expand(); //let's have it open by default
    menu->setStripeColor(ofColor::orange);
    for (int i=0; i<menu->size(); i++) menu->getChildAt(i)->setStripeColor(ofColor::yellow);
    m_gui.addBreak();
}


void GuiManager::setupImageGui()
{
    auto imageManager = &AppManager::getInstance().getImageManager();
    m_brightness.set("Brightness", 1.0 , 0.0, 2.0);
    m_brightness.addListener(imageManager, &ImageManager::onBrightnessChange);
    m_parameters.add(m_brightness);
    
    m_saturation.set("Saturation", 1.0 , 0.0, 2.0);
    m_saturation.addListener(imageManager, &ImageManager::onSaturationChange);
    m_parameters.add(m_saturation);
    
    m_contrast.set("Contrast", 1.0 , 0.0, 2.0);
    m_contrast.addListener(imageManager, &ImageManager::onContrastChange);
    m_parameters.add(m_contrast);
    
    m_topMargin.set("Top", 1.0 , 0.0, 1.0);
    m_topMargin.addListener(imageManager, &ImageManager::setTopMargin);
    m_parameters.add(m_topMargin);
    
    m_bottomMargin.set("Bottom", 1.0 , 0.0, 1.0);
    m_bottomMargin.addListener(imageManager, &ImageManager::setBottomMargin);
    m_parameters.add(m_bottomMargin);
    
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("IMAGE", ofColor::purple);
    //folder->addToggle("ColorCorrection", true);
    folder->addSlider(m_brightness);
    folder->addSlider(m_saturation);
    folder->addSlider(m_contrast);
    folder->addSlider(m_topMargin);
    folder->addSlider(m_bottomMargin);
    folder->expand();
    
    m_gui.addBreak();
}

void GuiManager::setupTimeLineGui()
{
    auto timeLineManager = &AppManager::getInstance().getTimeLineManager();
    
    m_timeCurrent.set("Time", 0.0, 0.0, 1.0);
    m_timeCurrent.addListener(timeLineManager, &TimeLineManager::onTimeChange);
    m_parameters.add(m_timeCurrent);
    
    m_timeLineDuration.set("Duration", 10, 0, 60);
    m_timeLineDuration.addListener(timeLineManager, &TimeLineManager::onDurationChange);
    m_parameters.add(m_timeLineDuration);
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("TIMELINE", ofColor::greenYellow);
    
    folder->addSlider(m_timeCurrent);
    folder->addSlider(m_timeLineDuration);
    folder->addButton("* Play");
    folder->addButton("* Stop");
    folder->addButton("* Reset");
    folder->addToggle("Reverse");
    folder->expand();
    
    m_gui.addBreak();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
    ofDisableSmoothing();
}

void GuiManager::update()
{
    m_gui.update();
}


void GuiManager::saveGuiValues()
{
    ofXml xml;
    ofSerialize(xml, m_parameters);
    xml.save(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    ofXml xml;
    xml.load(GUI_SETTINGS_FILE_NAME);
    ofDeserialize(xml, m_parameters);
}


void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}


void GuiManager::setupGuiEvents()
{
    m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}

void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "IMAGES:")
    {
        AppManager::getInstance().getImageManager().setImage(e.target->getLabel());
        m_gui.getDropdown(e.target->getName())->setLabel("IMAGES: " + e.target->getLabel());
    }
}   

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;

    if(e.target->getName() == "* Reset")
    {
        AppManager::getInstance().getTimeLineManager().reset();
    }
    
    else if(e.target->getName() == "* Play")
    {
        AppManager::getInstance().getTimeLineManager().play();
    }
    
   
    else if(e.target->getName() == "* Stop")
    {
        AppManager::getInstance().getTimeLineManager().stop();
    }
    
    else if(e.target->getName() == "* Save GUI")
    {
        this->saveGuiValues();
    }
    
    
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " selected" << endl;
    
    if(e.target->getName() == "Reverse")
    {
        if(e.target->getChecked()){
            AppManager::getInstance().getTimeLineManager().setPlayBackwards();
        }
        else{
            AppManager::getInstance().getTimeLineManager().setPlayForward();
        }
        
    }
    
    else if(e.target->getName() == "ColorCorrection")
    {
        AppManager::getInstance().getImageManager().onUseBrcosaChange(e.target->getChecked());
    }
    
    else if(e.target->getName() == "Mirrored")
    {
        AppManager::getInstance().getLedsManager().setMirrored(e.target->getChecked());
    }
}


void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}


void GuiManager::onDurationChange(float& value)
{
    m_timeLineDuration = ofClamp(value, m_timeLineDuration.getMin(),  m_timeLineDuration.getMax());
}

void GuiManager::onCurrentTimeChange(float& value)
{
    m_timeCurrent= ofClamp(value, m_timeCurrent.getMin(),  m_timeCurrent.getMax());
}


