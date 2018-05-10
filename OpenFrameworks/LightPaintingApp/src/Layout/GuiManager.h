/*
 *  GuiManager.h
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */



#pragma once

#include "Manager.h"
#include "ofxDatGui.h"

//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application«s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void update();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void onDurationChange(float& value);
    
    void onCurrentTimeChange(float& value);
    
private:
    
    void setupGuiParameters();
    
    void setupGuiEvents();
    
    void setupImageGui();
    
    void setupImagesDropDown();
    
    void setupTimeLineGui();
    
    void drawRectangle();
    
    
private:
    
    ofxDatGui			m_gui;
    
    ofParameter<float>	m_guiFPS;
    
    
    ofParameterGroup        m_parameters;
    
    ofParameter<float>      m_brightness;

    ofParameter<float>      m_timeLineDuration;
    ofParameter<float>      m_timeCurrent;

    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================

