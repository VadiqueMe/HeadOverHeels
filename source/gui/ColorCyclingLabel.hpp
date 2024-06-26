// The free and open source remake of Head over Heels
//
// Copyright © 2024 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game by Jon Ritman and Bernie Drummond
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef ColorCyclingLabel_hpp_
#define ColorCyclingLabel_hpp_

#include "Label.hpp"
#include "Timer.hpp"


namespace gui
{

class ColorCyclingLabel : public Label
{

public:

        ColorCyclingLabel( const std::string & text, bool doubleHeight = false ) ;

        virtual ~ColorCyclingLabel( ) ;

        virtual void draw () ;

private:

        void updateImageOfLabel () ;

        unsigned short cycle ;

        autouniqueptr < Timer > colorCyclingTimer ;

};

}

#endif
