// The free and open source remake of Head over Heels
//
// Copyright © 2024 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game by Jon Ritman and Bernie Drummond
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef CreateAudioMenu_hpp_
#define CreateAudioMenu_hpp_

#include "Action.hpp"


namespace gui
{

class MenuWithValues ;
class Label ;

/**
 * Create menu for adjusting music and sound effects
 */

class CreateAudioMenu : public Action
{

public:

        CreateAudioMenu( ) ;

        virtual std::string getNameOfAction () const {  return "CreateAudioMenu" ;  }

protected:

        /**
         * Show the audio menu
         */
        virtual void doAction () ;

private:

        MenuWithValues * listOfOptions ;

        Label * labelEffects ;
        Label * labelMusic ;

        Label * playRoomTunes ;

};

}

#endif
