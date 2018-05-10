/*
 *  AppManager.h
 *  Light Painting App
 *
 *  Created by Imanol Gomez on 10/05/18.
 *
 */

#pragma once

#include "ofMain.h"

#include "managers.h"


//========================== class AppManager ==============================
//============================================================================
/** \class AppManager AppManager.h
 *	\brief Class managing the whole application
 *	\details it set-ups, updates and renders all the different managers used for the application
 */

class AppManager: public Manager
{
public:

    //! Destructor
    ~AppManager();

    //! Returns the singleton instance.
    static AppManager& getInstance();

    //! Compares two transition objects
    void setup();

    //! updates the logic
    void update();

    //! calls the view manager to draw
    void draw();
    
    //! calls at exit
    void exit();

    //==========================================================================

    //! Returns the settings manager
    SettingsManager& getSettingsManager() { return m_settingsManager; }
    
    //! Returns the layout manager
    LayoutManager& getLayoutManager() { return m_layoutManager; }
    
    //! Returns the resource manager
    ResourceManager& getResourceManager() { return m_resourceManager; }
    
    //! Returns the view manager
    ViewManager&  getViewManager() { return m_viewManager; }
    
    //! Returns the visual effects manager
    VisualEffectsManager&  getVisualEffectsManager() { return m_visualEffectsManager; }
    
    //! Returns the GUI manager
    GuiManager&   getGuiManager() { return m_guiManager; }
        
    //! Returns the dmx manager
    UdpManager&  getUdpManager() { return m_udpManager; }
    
    //! Returns the leds manager
    LedsManager&  getLedsManager() { return m_ledsManager; }
    
    //! Returns the image manager
    ImageManager&  getImageManager() { return m_imageManager; }
    
    //! Returns the timeline manager
    TimeLineManager&  getTimeLineManager() { return m_timeLineManager; }
    
    //==========================================================================
    
    void toggleDebugMode();
    
    void setDebugMode(bool showDebug);
    

private:

     //! Constructor
     AppManager();

	//! Stop the compiler generating methods of copy the object
	 AppManager(AppManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     AppManager& operator=(AppManager const& copy);     // Don't implement

    //==========================================================================

    //! Set-up all the managers
    void setupManagers();

    //! Set-up openFrameworks
    void setupOF();

    //! update all the managers
    void updateManagers();

private:


    SettingsManager          m_settingsManager;          ///< Manages the application's settings
    ResourceManager          m_resourceManager;          ///< Manages the application's resources
    ViewManager              m_viewManager;              ///< Manages visuals
    VisualEffectsManager     m_visualEffectsManager;     ///< Manages visual effects
    LayoutManager            m_layoutManager;            ///< Manages the layout
    KeyboardManager          m_keyboardManager;          ///< Manages the keyboard
    GuiManager               m_guiManager;               ///< Manages the graphical user interface
    UdpManager               m_udpManager;               ///< Manages the UDP messages
    LedsManager              m_ledsManager;              ///< Manages the leds
    ImageManager             m_imageManager;             ///< Manages the image
    TimeLineManager          m_timeLineManager;          ///< Manages the time line
    bool                     m_debugMode;
};

//==========================================================================


