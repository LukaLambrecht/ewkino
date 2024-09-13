// inlcude c++ library classes
#include <string>
#include <vector>
#include <exception>
#include <iostream>

// include ROOT classes 
#include "TFile.h"
#include "TTree.h"
#include "TMVA/Reader.h"

// include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Event/interface/Event.h"
#include "../eventselection/interface/eventSelections.h"
#include "../eventselection/interface/eventFlattening.h"


bool chronologicsort(const std::tuple<long, long, long, float>& a,
		 const std::tuple<long, long, long, float>& b){
    if( std::get<0>(a)!=std::get<0>(b) ) return (std::get<0>(a)<std::get<0>(b));
    else if( std::get<1>(a)!=std::get<1>(b) ) return (std::get<1>(a)<std::get<1>(b));
    return (std::get<2>(a)<std::get<2>(b));
}


bool bdtscoresort(const std::tuple<long, long, long, float>& a,
                 const std::tuple<long, long, long, float>& b){
    return (std::get<3>(a)<std::get<3>(b));
}


bool checkReadability(const std::string& pathToFile){
    // temporary function to perform error handling when file cannot be read.
    // maybe to be replaced by exception throwing
    TreeReader treeReader;
    try{treeReader.initSampleFromFile( pathToFile );}
    catch( std::domain_error& ){
        std::cerr << "Can not read file. Returning." << std::endl;
        return false;
    }
    return true;
}


void selectEvents(const std::string& pathToFile,
		    const std::string& outputFilePath,
		    unsigned long nEntries, 
		    const std::string& eventselection,
		    const std::string& selection_type,
		    const std::string& bdtWeightsFile,
		    const double bdtCut){

    // initialize TreeReader from input file
    std::cout << "initializing TreeReader...\n\n";
    TreeReader treeReader;
    treeReader.initSampleFromFile( pathToFile );
    std::string year = treeReader.getYearString();

    // initialize reweighter (needed for syntax)
    std::shared_ptr< ReweighterFactory> reweighterFactory;
    reweighterFactory = std::shared_ptr<ReweighterFactory>( new EmptyReweighterFactory() );
    std::vector<Sample> thissample;
    thissample.push_back(treeReader.currentSample());
    CombinedReweighter reweighter = reweighterFactory->buildReweighter(
                                        "../../weights/", year, thissample );

    // load the BDT
    // default value is nullptr, 
    // in which case the BDT will not be evaluated.
    std::shared_ptr<TMVA::Experimental::RBDT<>> bdt;
    if( bdtWeightsFile.size()!=0 ){
        std::cout << "reading BDT evaluator..." << std::endl;
        bdt = std::make_shared<TMVA::Experimental::RBDT<>>("XGB", bdtWeightsFile);
        std::cout << "successfully loaded BDT evaluator." << std::endl;
    }

    // initialize output structure
    std::vector<std::tuple<long,long,long,float>> evtIds;

    // do event loop
    if( nEntries==0 ) nEntries = treeReader.numberOfEntries();
    long unsigned numberOfEntries = std::min( nEntries, treeReader.numberOfEntries() );
    std::cout << "Starting event loop for " << numberOfEntries << " events." << std::endl;
    for(long unsigned entry = 0; entry < numberOfEntries; entry++){
        if(entry%1000 == 0) std::cout << "processed: " << entry << " of " << numberOfEntries<<std::endl;
	
	// initialize map of variables
	std::map<std::string,double> varmap = eventFlattening::initVarMap();

	// do all selections
        Event event = treeReader.buildEvent(entry, false, false, false, false);
        if(!passES(event, eventselection, selection_type, "nominal")) continue;
	varmap = eventFlattening::eventToEntry(event, reweighter, selection_type, 
						nullptr, nullptr, nullptr, "nominal",
                                                bdt, year);
	if( bdtCut > -1 ){ if( varmap["_eventBDT"]<bdtCut ) continue; }

	// add event to list
	std::tuple<long,long,long,float> evtId = std::make_tuple<long,long,long,float>(
	    (long) varmap["_runNb"],
	    (long) varmap["_lumiBlock"],
	    (long) varmap["_eventNb"],
	    (float) varmap["_eventBDT"] );
	evtIds.push_back(evtId);
    }

    // sort the event list
    std::sort(evtIds.begin(), evtIds.end(), chronologicsort);

    // make output file
    std::ofstream outputFile;
    outputFile.open(outputFilePath);
    for( std::tuple<long,long,long,float> evtId: evtIds ){
	outputFile << std::get<0>(evtId) << ":";
	outputFile << std::get<1>(evtId) << ":";
	outputFile << std::get<2>(evtId) << ":";
	outputFile << std::get<3>(evtId) << std::endl;
    }
    outputFile.close();
}


int main( int argc, char* argv[] ){

    std::cerr << "###starting###" << std::endl;

    int nargs = 7;
    if( argc != nargs+1 ){
        std::cerr << "ERROR: selectEvents.cc requires " << nargs << " arguments to run: " << std::endl;
        std::cerr << "- path to input file" << std::endl;
	std::cerr << "- path to output file" << std::endl;
	std::cerr << "- number of entries to run over" << std::endl;
	std::cerr << "- event selection" << std::endl;
	std::cerr << "- selection type" << std::endl;
	std::cerr << "- path to BDT weights file" << std::endl;
	std::cerr << "- bdt cut value" << std::endl;
        return -1;
    }

    // parse arguments
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    std::string& input_file_path = argvStr[1]; // path to input file to be processed
    std::string& output_file_path = argvStr[2]; // output file to put the result in
    long unsigned nentries = std::stol(argvStr[3]); // number of entries to process (0 for all)
    std::string& event_selection = argvStr[4]; // event selection to perform (see event selection code)
    std::string& selection_type = argvStr[5]; // type of event selection (see event selection code)
    std::string& bdtWeightsFile = argvStr[6];
    double bdtCut = std::stof(argvStr[7]);

    // printouts useful for debugging:
    std::cout << "running ./selectEvents with following settings:" << std::endl;
    std::cout << "input file: " << input_file_path << std::endl;
    std::cout << "output file: " << output_file_path << std::endl;
    std::cout << "nentries: " << nentries << std::endl;
    std::cout << "event selection: " << event_selection << std::endl;
    std::cout << "selection type: " << selection_type << std::endl;
    std::cout << "bdt weights file: " << bdtWeightsFile << std::endl;
    std::cout << "bdt cut: "<< bdtCut << std::endl;

    // check validity
    bool validInput = checkReadability( input_file_path );
    if(!validInput) return -1;

    // fill the histograms
    selectEvents(input_file_path, output_file_path, nentries,
		event_selection, selection_type,
		bdtWeightsFile, bdtCut);

    std::cerr << "###done###" << std::endl;
    return 0;
}
