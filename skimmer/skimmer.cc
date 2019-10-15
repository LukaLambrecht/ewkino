//include c++ library classes 
#include <string>
#include <vector>
#include <exception>

#include <iostream>

//include ROOT classes 
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

//include other parts of framework
#include "../TreeReader/interface/TreeReader.h"
#include "../Tools/interface/stringTools.h"
#include "../Event/interface/Event.h"
#include "interface/skimSelections.h"


void skimFile( const std::string& pathToFile, const std::string& outputDirectory, const std::string& skimCondition ){

    std::cout << "skimming " << pathToFile << std::endl;

    //initialize TreeReader
    TreeReader treeReader;
    treeReader.initSampleFromFile( pathToFile );

    //make output ROOT file
    std::string outputFilePath = stringTools::formatDirectoryName( outputDirectory ) + stringTools::removeOccurencesOf( pathToFile, "/" );
    TFile* outputFilePtr = TFile::Open( outputFilePath.c_str() , "RECREATE" );
    outputFilePtr->mkdir( "blackJackAndHookers" );
    outputFilePtr->cd( "blackJackAndHookers" );

    //read histograms from input file and write them to the new file
    std::vector< std::shared_ptr< TH1 > > histVector = treeReader.getHistogramsFromCurrentFile();
    for( const auto& histPtr : histVector ){
        histPtr->Write();
    }

    //make output tree
    std::shared_ptr< TTree > outputTreePtr( std::make_shared< TTree >( "blackJackAndHookersTree","blackJackAndHookersTree" ) );
    treeReader.setOutputTree( outputTreePtr.get() );

    for( long unsigned entry = 0; entry < treeReader.numberOfEntries(); ++entry ){

        //build event
        Event event = treeReader.buildEvent( entry, true, true );

        //apply event selection
        if( !passSkim( event, skimCondition ) ) continue;

        //fill new tree
        outputTreePtr->Fill();
    }

    //write new tree
    outputTreePtr->Write( "",  BIT(2) );

    //close output file
    outputFilePtr->Close();
}


int main( int argc, char* argv[] ){
    if( argc != 4 ){
        std::cerr << "skimmer requires exactly three arguments to run : input_file_path, output_directory, skim_condition" << std::endl;
        return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );

    std::string& input_file_path = argvStr[1];
    std::string& output_directory = argvStr[2];
    std::string& skimCondition = argvStr[3];
    skimFile( input_file_path, output_directory, skimCondition );

    return 0;
}