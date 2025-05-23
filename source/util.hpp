// The free and open source remake of Head over Heels
//
// Copyright © 2025 Douglas Mencken dougmencken@gmail.com
// Copyright © 2008 Jorge Rodríguez Santos
// Original game by Jon Ritman and Bernie Drummond
//
// This program is free software
// You may redistribute it and~or modify it under the terms of the GNU General Public License
// either version 3 of the License or at your option any later version

#ifndef util_hpp_
#define util_hpp_

#include <cstring>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#if __cplusplus >= 201103L /* when complier supports c++11 */
    #define __Cxx11__
    #define nilPointer nullptr
    #define autouniqueptr std::unique_ptr
#else
    #define nilPointer NULL
    #define autouniqueptr std::auto_ptr
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <cxxabi.h>

#if defined( DEBUG ) && defined( HAVE_EXECINFO_H ) && HAVE_EXECINFO_H
    #include <execinfo.h>
#endif

#if defined( DEBUG ) && DEBUG
    #define IF_DEBUG( line )    line ;
#else
    #define IF_DEBUG( line )
#endif

#define CONSOLE_COLOR_OFF               "\033[0m"

#define CONSOLE_COLOR_BLACK             "\033[0;30m"
#define CONSOLE_COLOR_BLUE              "\033[0;34m"
#define CONSOLE_COLOR_RED               "\033[0;31m"
#define CONSOLE_COLOR_GREEN             "\033[0;32m"

#define CONSOLE_COLOR_BOLD_BLACK        "\033[1;30m"
#define CONSOLE_COLOR_BOLD_BLUE         "\033[1;34m"
#define CONSOLE_COLOR_BOLD_RED          "\033[1;31m"
#define CONSOLE_COLOR_BOLD_GREEN        "\033[1;32m"

/**
 * Object-function to finalize elements of Standard Template Library container
 */
struct DeleteIt
{
        template < typename T >
        void operator() ( const T* ptr ) const
        {
                delete ptr ;
        }
} ;

class util
{

public:

        inline static std::string pointer2string ( const void * pointer )
        {
                std::ostringstream result ;
                result << std::hex << pointer ;
                return result.str () ;
        }

        inline static std::string number2string ( int number )
        {
                std::ostringstream result ;
                result << std::dec << number ;
                return result.str () ;
        }

        inline static std::string number2hexstring ( int number )
        {
                std::ostringstream result ;
                result << std::hex << number ;
                return result.str () ;
        }

        static std::string toStringWithOrdinalSuffix( unsigned int number )
        {
                unsigned int mod10 = number % 10 ;
                unsigned int mod100 = number % 100 ;

                std::ostringstream result ;
                result << number ;

                if ( mod10 == 1 && mod100 != 11 )
                        result << "st" ;
                else if ( mod10 == 2 && mod100 != 12 )
                        result << "nd" ;
                else if ( mod10 == 3 && mod100 != 13 )
                        result << "rd" ;
                else
                        result << "th" ;

                return result.str() ;
        }

        static bool stringEndsWith( const std::string & str, const std::string & suffix )
        {
                int dl = str.size() - suffix.size() ;
                return dl >= 0 && str.rfind( suffix ) == static_cast< unsigned >( dl );
        }

        static std::string makeRandomString ( const size_t length )
        {
                static const char alphanum[] =  "0123456789"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz" ;
                static const size_t howManyChars = sizeof( alphanum );

                std::string result( length, ' ' );

                for ( size_t i = 0 ; i < length; ++ i )
                {
                        result[ i ] = alphanum[ rand() % ( howManyChars - 1 ) ];
                }

                return result ;
        }

        inline static std::string demangle ( const std::string & mangled )
        {
                int code ;
                char* cxxname = abi::__cxa_demangle( mangled.c_str (), nilPointer, nilPointer, &code );

                if ( code == 0 /* demangling succeeded */ && cxxname != nilPointer )
                {
                        std::string unmangled( cxxname );
                        free( cxxname );
                        return unmangled ;
                }

                return mangled ;
        }

        /** print the backtrace of a caller */
        static void printBacktrace ( FILE * out = stdout, unsigned int howDeep = 80 )
        {
#if defined( DEBUG ) && defined( HAVE_EXECINFO_H ) && HAVE_EXECINFO_H
                fprintf( out, "backtrace\n" );

                // storage for data of backtrace addresses
                void* addrlist[ howDeep + 1 ];

                // get addresses of backtrace
                size_t addrlen = backtrace( addrlist, sizeof( addrlist ) / sizeof( void * ) );

                if ( addrlen == 0 ) {
                        fprintf( out, "    < empty, possibly broken >\n" );
                        return;
                }

                // convert addresses into strings
                char** listOfSymbols = backtrace_symbols( addrlist, addrlen );

                /* for ( size_t i = 0; i < addrlen; i++ )
                        fprintf( out, "    %s\n", listOfSymbols[ i ] ); */

                std::vector< std::string > symbols [ addrlen ];
                std::vector< size_t > columnLengths ;

                size_t columnOfSymbol = 0 ;
                size_t columnOfLineNumber = 0 ;
                size_t columnOfOffset = 0 ;

                for ( size_t line = 0 ; line < addrlen ; ++ line )
                {
                        size_t column = 0 ;
                        const char* token = std::strtok( listOfSymbols[ line ], " " );

                        while ( token != nilPointer )
                        {
                                std::string demangled = demangle( token );
                                if ( demangled != token ) columnOfSymbol = column ;

                                if ( ( line & 1 ) != 0 /* is odd */ )
                                        if ( demangled == number2string( line ) || demangled == number2string( line + 1 ) )
                                                columnOfLineNumber = column ;

                                if ( demangled == "+" )
                                {
                                        const char* offset = std::strtok( nilPointer, " " );
                                        if ( offset != nilPointer )
                                        {
                                                demangled = std::string( "+ " ) + "0x" + number2hexstring( std::atoi( offset ) ) ;
                                                columnOfOffset = column ;
                                        }
                                }

                                symbols[ line ].push_back( demangled );

                                size_t stringLength = symbols[ line ].back().length ();
                                if ( line == 0 )
                                        columnLengths.push_back( stringLength );
                                else if ( stringLength > columnLengths[ column ] )
                                        columnLengths[ column ] = stringLength ;

                                token = std::strtok( nilPointer, " " );
                                ++ column ;
                        }
                }

                free( listOfSymbols );

                for ( size_t line = 0; line < addrlen; ++line )
                {
                        std::string numberOfLine = number2string( line );

                        fprintf( out, "    " );

                        // print depth

                        size_t banana = number2string( addrlen - 1 ).length() ;
                        {
                                size_t spaces = 0 ;
                                while ( ( spaces ++ ) + numberOfLine.length () < banana )
                                        fprintf( out, " " );
                        }

                        fprintf( out, "%s", numberOfLine.c_str () );

                        // print symbol

                        std::string symbol = symbols[ line ][ columnOfSymbol ] ;
                        fprintf( out, "  %s ", symbol.c_str () );

                        if ( columnOfSymbol + 1 < symbols[ line ].size () )
                        {
                                size_t spaces = 0 ;
                                while ( ( spaces ++ ) + symbol.length () < columnLengths[ columnOfSymbol ] )
                                        fprintf( out, " " );
                        }

                        // print other info like location of symbol

                        const bool printOffset = true ;

                        for ( size_t token = 0 ; token < symbols[ line ].size () ; ++ token )
                        {
                                if ( token == columnOfLineNumber ) continue ;
                                if ( token == columnOfSymbol ) continue ;
                                if ( ! printOffset && token == columnOfOffset ) continue ;

                                fprintf( out, " %s", symbols[ line ][ token ].c_str () );

                                if ( token + 1 < symbols[ line ].size () )
                                {
                                        size_t spaces = 0 ;
                                        while ( ( spaces ++ ) + symbols[ line ][ token ].length () < columnLengths[ token ] )
                                                fprintf( out, " " );
                                }
                        }

                        fprintf( out, "\n" );
                }
#else
                ( void ) howDeep ;
                fprintf( out, "no backtrace for non-debug build or when there’s no execinfo.h\n" );
#endif
        }

} ;

#endif
