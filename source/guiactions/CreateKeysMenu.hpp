// The free and open source remake of Head over Heels
//
// Copyright © 2024 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game by Jon Ritman and Bernie Drummond
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef CreateKeysMenu_hpp_
#define CreateKeysMenu_hpp_

#include "Action.hpp"

namespace gui
{

class MenuWithValues ;


/**
 * Create “select the keys” menu
 */

class CreateKeysMenu : public Action
{

public:

        CreateKeysMenu( ) : Action( ), menuOfKeys( nilPointer ) { }

protected:

        /**
         * Show the keys menu
         */
        virtual void act () ;

private:

        MenuWithValues * menuOfKeys ;

};

}

#endif
