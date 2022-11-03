#include "../Tools/interface/mergeAndRemoveOverlap.h"

// include c++ library classes 
#include <memory>
#include <thread>

//include ROOT classes 
#include "TFile.h"

//include other parts of framework
#include "../Tools/interface/stringTools.h"
#include "../Tools/interface/analysisTools.h"
#include "../TreeReader/interface/TreeReader.h"
#include "../Event/interface/EventTags.h"
#include "../Event/interface/Event.h"



// declare variables
static const unsigned nL_max = 20;
static const unsigned gen_nL_max = 20;
ULong64_t       _runNb;
ULong64_t       _lumiBlock;
ULong64_t       _eventNb;
Double_t        _weight;
UInt_t           _nVertex;
Double_t        _dxy[nL_max];
Double_t        _dz[nL_max];
Double_t        _3dIP[nL_max];
Double_t        _3dIPSig[nL_max];
UInt_t          _nL;
UInt_t          _nMu;
UInt_t          _nEle;
UInt_t          _nLight;
UInt_t          _nTau;
Double_t        _lPt[nL_max];
Double_t        _lPtCorr[nL_max];
Double_t        _lEta[nL_max];
Double_t        _lEtaSC[nL_max];
Double_t        _lPhi[nL_max];
Double_t        _lE[nL_max];
Double_t        _lECorr[nL_max];
UInt_t          _lFlavor[nL_max];
Int_t           _lCharge[nL_max];
Bool_t          _lIsPrompt[nL_max];
Int_t           _lMatchPdgId[nL_max];
Int_t           _lMatchCharge[nL_max];
Int_t           _lMomPdgId[nL_max];
Bool_t          isEE;
Bool_t          isEMu;
Bool_t          isMuMu;
Bool_t          isOS;
Int_t           l1;
Float_t         l1_pt;
Int_t           l2;
Float_t         l2_pt;
Float_t         mll; 
Int_t           nLepSel;


// declare branches
TBranch         *b__runNb;
TBranch         *b__lumiBlock;
TBranch         *b__eventNb;
TBranch        *b__weight;
TBranch        *b__nVertex;
TBranch        *b__dxy;
TBranch        *b__dz;
TBranch        *b__3dIP;
TBranch        *b__3dIPSig;
TBranch        *b__nL;
TBranch        *b__nMu;
TBranch        *b__nEle;
TBranch        *b__nLight;
TBranch        *b__nTau;
TBranch        *b__lPt;
TBranch        *b__lPtCorr;
TBranch        *b__lEta;
TBranch        *b__lEtaSC;
TBranch        *b__lPhi;
TBranch        *b__lE;
TBranch        *b__lECorr;
TBranch        *b__lFlavor;
TBranch        *b__lCharge;
TBranch        *b__lIsPrompt;
TBranch        *b__lMatchPdgId;
TBranch        *b__lMatchCharge;
TBranch        *b__lMomPdgId;
TBranch         *b_isEE;
TBranch         *b_isEMu;
TBranch         *b_isMuMu;
TBranch         *b_isOS;
TBranch         *b_l1;
TBranch         *b_l1_pt;
TBranch         *b_l2;
TBranch         *b_l2_pt;
TBranch         *b_mll;
TBranch         *b_nLepSel;



// set branch addresses
void setBranchAddresses( TreeReader treeReader ){
    treeReader._currentTreePtr->SetBranchAddress("_runNb", &_runNb, &b__runNb);
    treeReader._currentTreePtr->SetBranchAddress("_lumiBlock", &_lumiBlock, &b__lumiBlock);
    treeReader._currentTreePtr->SetBranchAddress("_eventNb", &_eventNb, &b__eventNb);
    treeReader._currentTreePtr->SetBranchAddress("_nVertex", &_nVertex, &b__nVertex);
    treeReader._currentTreePtr->SetBranchAddress("_dxy", &_dxy, &b__dxy);
    treeReader._currentTreePtr->SetBranchAddress("_dz", &_dz, &b__dz);
    treeReader._currentTreePtr->SetBranchAddress("_3dIP", &_3dIP, &b__3dIP);
    treeReader._currentTreePtr->SetBranchAddress("_3dIPSig", &_3dIPSig, &b__3dIPSig);
    treeReader._currentTreePtr->SetBranchAddress("_nL", &_nL, &b__nL);
    treeReader._currentTreePtr->SetBranchAddress("_nMu", &_nMu, &b__nMu);
    treeReader._currentTreePtr->SetBranchAddress("_nEle", &_nEle, &b__nEle);
    treeReader._currentTreePtr->SetBranchAddress("_nLight", &_nLight, &b__nLight);
    treeReader._currentTreePtr->SetBranchAddress("_nTau", &_nTau, &b__nTau);
    treeReader._currentTreePtr->SetBranchAddress("_lPt", _lPt, &b__lPt);
    treeReader._currentTreePtr->SetBranchAddress("_lPtCorr", _lPtCorr, &b__lPtCorr);
    treeReader._currentTreePtr->SetBranchAddress("_lEta", _lEta, &b__lEta);
    treeReader._currentTreePtr->SetBranchAddress("_lEtaSC", _lEtaSC, &b__lEtaSC);
    treeReader._currentTreePtr->SetBranchAddress("_lPhi", _lPhi, &b__lPhi);
    treeReader._currentTreePtr->SetBranchAddress("_lE", _lE, &b__lE);
    treeReader._currentTreePtr->SetBranchAddress("_lECorr", _lECorr, &b__lECorr);
    treeReader._currentTreePtr->SetBranchAddress("_lFlavor", _lFlavor, &b__lFlavor);
    treeReader._currentTreePtr->SetBranchAddress("_lCharge", _lCharge, &b__lCharge);
    //treeReader._currentTreePtr->SetBranchAddress("_weight", &_weight, &b__weight);
    //treeReader._currentTreePtr->SetBranchAddress("_lIsPrompt", _lIsPrompt, &b__lIsPrompt);
    //treeReader._currentTreePtr->SetBranchAddress("_lMatchPdgId", _lMatchPdgId, &b__lMatchPdgId);
    //treeReader._currentTreePtr->SetBranchAddress("_lMatchCharge", _lMatchCharge, &b__lMatchCharge);
    //treeReader._currentTreePtr->SetBranchAddress("_lMomPdgId",  _lMomPdgId, &b__lMomPdgId);
    treeReader._currentTreePtr->SetBranchAddress("isEE",  &isEE, &b_isEE);
    treeReader._currentTreePtr->SetBranchAddress("isEMu",  &isEMu, &b_isEMu);
    treeReader._currentTreePtr->SetBranchAddress("isMuMu",  &isMuMu, &b_isMuMu);
    treeReader._currentTreePtr->SetBranchAddress("isOS",  &isOS, &b_isOS);
    treeReader._currentTreePtr->SetBranchAddress("l1",  &l1, &b_l1);
    treeReader._currentTreePtr->SetBranchAddress("l1_pt",  &l1_pt, &b_l1_pt);
    treeReader._currentTreePtr->SetBranchAddress("l2",  &l2, &b_l2);
    treeReader._currentTreePtr->SetBranchAddress("l2_pt",  &l2_pt, &b_l2_pt);
    treeReader._currentTreePtr->SetBranchAddress("mll",  &mll, &b_mll);
    treeReader._currentTreePtr->SetBranchAddress("nLepSel",  &nLepSel, &b_nLepSel);
}



bool yearsAreConsistent_ChFl( const std::vector< std::string >& inputPathVector ){

    //index specifying the year of the previous file 
    int previousYearIndex = -1;
    for( const auto& inputFilePath : inputPathVector ){
        
        std::pair< bool, bool > is2017Or2018 = analysisTools::fileIs2017Or2018( inputFilePath );

        //from the second file onwards, check that all files have the same year 
        if( previousYearIndex != -1 ){
            int yearIndex = 1*( is2017Or2018.first ) + 2*( is2017Or2018.second );
            if( yearIndex != previousYearIndex ){
                return false;
            }
            previousYearIndex = yearIndex;
        }

    }
    return true;
}



bool eventIsNew(  const long unsigned runNumber, const long unsigned luminosityBlock, const long unsigned eventNumber, std::set< EventTags >& usedEventTags ){

    //search set for the current event 
    auto tagIt = usedEventTags.find( EventTags(runNumber, luminosityBlock, eventNumber) );

    //continue if event with the same tags was seen before
    if( tagIt != usedEventTags.end() ){
        return false;
    }

    //add unseen events to the set 
    usedEventTags.emplace( EventTags(runNumber, luminosityBlock, eventNumber) ); 

    return true;
    }


void mergeAndRemoveOverlap_ChFl( const std::vector< std::string >& inputPathVector, 
			    const std::string& outputPath, 
			    const bool allowMergingYears ){

    // size of input vector must be at least 2, otherwise there can be no merging 
    if( inputPathVector.size() < 2 ){
	std::string msg = "ERROR in mergeAndRemoveOverlap: Input path vector has size ";
	msg += std::to_string( inputPathVector.size() ) + ", while it should be at least 2.";
        throw std::length_error( msg );
    }

    // unless explicitly specified, don't allow the merging of files for different years
    if( !( allowMergingYears || yearsAreConsistent_ChFl( inputPathVector ) ) ){
	std::string msg = "ERROR in mergeAndRemoveOverlap: Can't merge datasets";
	msg += " corresponding to different files unless explicitly specified.";
        throw std::logic_error( msg );
    }
    
    // initialize TreeReader
    TreeReader treeReader;

    // make output file and output Tree
    TFile* outputFilePtr = TFile::Open( outputPath.c_str(), "RECREATE" );
    outputFilePtr->mkdir( "blackJackAndHookers" );
    outputFilePtr->cd( "blackJackAndHookers" );
    std::shared_ptr< TTree > outputTreePtr( std::make_shared< TTree >( 
				"blackJackAndHookersTree","blackJackAndHookersTree" ) );
    
    // histograms stored in file
    //std::map< std::string, std::shared_ptr< TH1 > > outputHistogramMap;

    // set of events that has been seen
    // use std::set so search scales as log(N) 
    std::set< EventTags > usedEventTags;

    // loop over files
    for( auto inputPathIt = inputPathVector.cbegin(); 
	inputPathIt != inputPathVector.cend(); 
	++inputPathIt ){

	std::cout << "processing file " << inputPathIt-inputPathVector.begin()+1;
	std::cout << " of " << inputPathVector.size() << std::endl;
        const auto& inputFilePath = *inputPathIt;

        // open next sample
        // DO NOT reset triggers because this will invalidate the addresses set by setOutputTree 
	// and trigger decisions in output file will be wrong!
        treeReader.initSampleFromFile( inputFilePath, false, false);
        std::cout << "Youll never see me";
        treeReader.setOutputTree( outputTreePtr.get() );

        setBranchAddresses(treeReader);
        outputTreePtr->SetDirectory( outputFilePtr);

        // set output histograms and output tree for first file
        if( inputPathIt == inputPathVector.cbegin() ){
/*
            for( const auto& histPtr : treeReader.getHistogramsFromCurrentFile() ){
                outputHistogramMap[ histPtr->GetName() ] = histPtr;
            }*/

            // set uo output tree
          
        // for next files, add the histograms to the current histograms 
	// and check that no unknown histograms are present
        }/* else {
            for( const auto& histPtr : treeReader.getHistogramsFromCurrentFile() ){
                auto histIt = outputHistogramMap.find( histPtr->GetName() );
                if( histIt == outputHistogramMap.cend() ){
		    std::string msg = "ERROR in mergeAndRemoveOverlap: ";
		    msg += "Histogram " + std::string( histPtr->GetName() ); 
		    msg += " not found in file " + inputPathVector[0] + ".";
                    throw std::invalid_argument( msg );
                }
                histIt->second->Add( histPtr.get() );
            }
        }*/

        // loop over events in tree and write them to the output tree if there is no overlap
        for( long unsigned entry = 0; entry < treeReader.numberOfEntries(); ++entry ){
	    if(entry%1000000 == 0){
		std::cout << "processed: " << entry;
		std::cout << " of "<< treeReader.numberOfEntries() << std::endl;
	    }

            treeReader._currentTreePtr->GetEntry(entry);

            // check if event is new and insert it into the list of used events 
            if( eventIsNew( _runNb, _lumiBlock, _eventNb, usedEventTags ) ){

                // write event to output tree
                outputTreePtr->Fill();
            }
        }

    }

    // need to change directory for writing
    outputFilePtr->cd( "blackJackAndHookers" );

    // write output histograms 
    // for( const auto& histPair : outputHistogramMap ){
    //    histPair.second->Write();
    //}
    
    // write output tree 
    outputTreePtr->Write( "", BIT(2) );

    // close output file
    outputFilePtr->Close();
    }




int main( int argc, char* argv[] ){

    std::cerr << "###starting###" << std::endl;
    // check command line arguments
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    if( !( argvStr.size() == 4 ) ){
        std::cerr << "ERROR: found " << argc-1 << " command line args,";
        std::cerr << " while 3 are needed:" << std::endl;
        std::cerr << "  - inputPathVector" << std::endl;
        std::cerr << "  - outputPath" << std::endl;
        std::cerr << "  - allowYearsMerge" << std::endl;
        return 1;
    }
    std::string s1 = argvStr[1];  
    std::vector<std::string> inputPathVector;
    std::string token;
    std::stringstream ss(s1);
    while (getline(ss, token, '-')){
        inputPathVector.push_back(token);
    }
    std::string outputPath = argvStr[2];
    std::string s3 = argvStr[3];
    bool allowYearsMerge = (s3 == "true"); 
    std::cerr << inputPathVector[0]; 
    mergeAndRemoveOverlap_ChFl(inputPathVector, outputPath, allowYearsMerge);
    std::cerr << "###done###" << std::endl;
    return 0;
}

