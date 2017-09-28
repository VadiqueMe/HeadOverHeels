// The free and open source remake of Head over Heels
//
// Copyright © 2017 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game copyright © 1987 Ocean Software Ltd.
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef TableOfTransparencies_hpp_
#define TableOfTransparencies_hpp_

#include <vector>


namespace isomot
{

/**
 * Table of transparencies for free items
 */

class TableOfTransparencies
{

public:

       /**
        * Initialize table of transparencies
        */
        TableOfTransparencies( ) ;

        virtual ~TableOfTransparencies( ) ;

       /**
        * @param amount percentage of item’s transparency
        */
        void addToDegreeOfTransparency ( const unsigned char amount ) ;

       /**
        * @param amount percentage of item’s transparency
        */
        void removeFromDegreeOfTransparency ( const unsigned char amount ) ;

       /**
        * Count elements in the table with a given degree of transparency
        * @param percentage of transparency
        */
        unsigned int countItemsWithDegreeOfTransparency ( unsigned int degree ) const
        {
                return ( degree >= 0 && degree <= 100 ? this->table[ degree ] : 0 ) ;
        }

private:

       /**
        * Table of transparencies
        * It’s a vector of 101 elements where index means degree 0 to 100 of transparency,
        * and value presents the number of items in room with this degree
        */
        std::vector < unsigned int > table ;

};

}

#endif