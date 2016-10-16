
#include "CreateListOfSavedGames.hpp"
#include "GuiManager.hpp"
#include "LanguageManager.hpp"
#include "Font.hpp"
#include "Screen.hpp"
#include "Menu.hpp"
#include "Icon.hpp"
#include "Label.hpp"
#include "CreateMainMenu.hpp"
#include "LoadGame.hpp"
#include "SaveGame.hpp"
#include "Ism.hpp"

using gui::CreateListOfSavedGames;


CreateListOfSavedGames::CreateListOfSavedGames( BITMAP* picture, bool isLoadMenu )
: Action(),
  where( picture ),
  loadMenu( isLoadMenu )
{

}

void CreateListOfSavedGames::doIt ()
{
        Screen* screen = new Screen( 0, 0, this->where );

        // Imagen de fondo
        screen->setBackground( GuiManager::getInstance()->findImage( "background" ) );

        // Si es el menú de carga y se pulsa Esc se vuelve al menú principal
        if ( loadMenu )
        {
                screen->setAction( new CreateMainMenu( this->where ) );
        }

        CreateMainMenu::placeHeadAndHeels( screen, /* icons */ true, /* copyrights */ false );

        Label* label = 0;
        LanguageManager* languageManager = GuiManager::getInstance()->getLanguageManager();

        // Las partidas guardadas
        Menu* menu = new Menu( 80, 160 );
        for ( int fileCount = 1; fileCount < 10; fileCount++ )
        {
                std::stringstream ss;
                ss << fileCount;
                std::string file = isomot::homePath() + "savegame/savegame" + ss.str () + ".xml";

                if ( exists( file.c_str() ) )
                {
                        short rooms = 0;
                        short planets = 0;
                        this->recoverFileInfo( file, &rooms, &planets );
                        ss.str( std::string() );
                        ss << fileCount << ". " << rooms << " " << languageManager->findLanguageString( "rooms" )->getText() << "; "
                                                << planets << " " << languageManager->findLanguageString( "planets" )->getText();
                        label = new Label( ss.str() );

                        if ( loadMenu )
                        {
                                label->setAction( new LoadGame( this->where, fileCount ) );
                        }
                        else
                        {
                                label->setAction( new SaveGame( this->where, fileCount ) );
                        }

                        if ( fileCount == 1 )
                        {
                                menu->addActiveOption( label );
                        }
                        else
                        {
                                menu->addOption( label );
                        }

                        continue;
                }
                else
                {
                        ss.str( std::string() );
                        ss << fileCount << ". " << languageManager->findLanguageString( "empty-slot" )->getText();
                        label = new Label( ss.str() );
                        if ( ! loadMenu )
                        {
                                label->setAction( new SaveGame( this->where, fileCount ) );
                        }

                        if ( fileCount == 1 )
                        {
                                menu->addActiveOption( label );
                        }
                        else
                        {
                                menu->addOption( label );
                        }
                }
        }

        // Añade el menú a la pantalla actual
        screen->addWidget( menu );

        // Crea la cadena de responsabilidad
        screen->setSucessor( menu );

        // Cambia la pantalla mostrada en la interfaz
        GuiManager::getInstance()->changeScreen( screen );
}

void CreateListOfSavedGames::recoverFileInfo( const std::string& fileName, short* rooms, short* planets )
{
        try
        {
                std::auto_ptr< sgxml::SaveGameXML > saveGameXML( sgxml::savegame( fileName.c_str() ) );

                *rooms = saveGameXML->exploredRooms().visited().size();
                *planets = ( saveGameXML->freeByblos() ? 1 : 0 ) +
                                ( saveGameXML->freeEgyptus() ? 1 : 0 ) +
                                ( saveGameXML->freePenitentiary() ? 1 : 0 ) +
                                ( saveGameXML->freeSafari() ? 1 : 0 ) +
                                ( saveGameXML->freeBlacktooth() ? 1 : 0 );
        }
        catch( const xml_schema::exception& e )
        {
                std::cout << e << std::endl ;
        }
}