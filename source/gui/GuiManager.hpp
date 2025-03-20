// The free and open source remake of Head over Heels
//
// Copyright © 2025 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game by Jon Ritman and Bernie Drummond
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef GuiManager_hpp_
#define GuiManager_hpp_

#include <string>
#include <map>

#include "util.hpp"

#include "LanguageStrings.hpp"


namespace gui
{

class Slide ;
class SlideWithHeadAndHeels ;


class GuiManager
{

private:

        GuiManager( ) ;

        GuiManager( const GuiManager & ) { }

public:

        ~GuiManager( ) ;

        static GuiManager & getInstance () ;

        void firstMenu () ;

        /**
         * Draw the user interface and handle keys
         */
        void loop () ;

        void changeSlide ( Slide * newSlide, bool dive /* it becomes rightToLeft for barWipeHorizontally */ = false ) ;
        void changeToSlideFor ( const std::string & forAction, bool dive = false ) ;

        void setSlideForAction( Slide * theSlide, const std::string & nameOfAction ) ;

        /**
         * Find among all the slides the one that is associated with this action.
         * And if there’s no such slide, create a new one
         */
        Slide & findOrCreateSlideForAction ( const std::string & nameOfAction ) ;
        SlideWithHeadAndHeels & findOrCreateSlideWithHeadAndHeelsForAction ( const std::string & nameOfAction ) ;

        void freeSlides () ;

        void refreshSlides () ;

        void redraw () const ;

        void exit () {  this->looping = false ;  }

        bool isInFullScreen () const {  return this->inFullScreen ;  }

       /**
        * Switches between full screen video & windowed video
        */
        void toggleFullScreenVideo () ;

       /**
        * @return A string of characters in the LLL_CC format
        *         where LLL is a language code according to ISO 639
        *         and CC is a country code according to ISO 3166
        */
        const std::string & getLanguage () const {  return this->chosenLanguage ;  }

        void setLanguage ( const std::string & language )
        {
                if ( language != this->chosenLanguage ) {
                        if ( this->languageStrings != nilPointer ) {
                                delete this->languageStrings ;
                                this->languageStrings = nilPointer ;
                        }

                        this->chosenLanguage = language ;
                }
        }

        LanguageStrings & getOrMakeLanguageStrings ()
        {
                if ( this->languageStrings == nilPointer ) {
                        this->languageStrings = ( ! this->chosenLanguage.empty() )
                                                        ? new LanguageStrings( this->chosenLanguage + ".xml", "en_US.xml" )
                                                        : new LanguageStrings( "en_US.xml" ) ;
                }

                return * this->languageStrings ;
        }

        Slide * getActiveSlide () const {  return this->activeSlide ;  }

        unsigned int countSlides () const {  return this->slides.size() ;  }

private:

        void setActiveSlide ( Slide * newSlide ) ;

        /**
         * Unique object of this class for the whole game
         */
        static GuiManager * instance ;

        // the slide that is currently being drawn
        Slide * activeSlide ;

        std::map < /* name of action */ std::string, Slide * > slides ;

        // a language of the user interface in the LLL_CC format
        std::string chosenLanguage ;

        LanguageStrings * languageStrings ;

        // when true draw the user interface and handle the keyboard
        bool looping ;

        /**
         * Draw graphics in full screen when true or in a window when false
         */
        bool inFullScreen ;

};

}

#endif
