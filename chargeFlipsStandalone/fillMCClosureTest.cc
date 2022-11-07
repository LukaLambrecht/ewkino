/*
Perform a closure test for the MC charge flip rates
*/

// include c++ library classes 
#include <memory>
#include <thread>

// include ROOT classes
#include "TH2D.h"
#include "TStyle.h"

// include other parts of framework
#include "../TreeReader/interface/TreeReader.h"
#include "../Event/interface/Event.h"
#include "../Tools/interface/systemTools.h"
#include "../Tools/interface/stringTools.h"
#include "../Tools/interface/histogramTools.h"
#include "../Tools/interface/HistInfo.h"
#include "../Tools/interface/analysisTools.h"
#include "../plotting/interface/plotCode.h"
#include "../plotting/interface/tdrStyle.h"

// include dedicated tools
#include "../chargeFlips/interface/chargeFlipSelection.h"
#include "../chargeFlips/interface/chargeFlipTools.h"


// declare variables
static const unsigned nL_max = 20;
static const unsigned gen_nL_max = 20;
ULong64_t         _runNb;
ULong64_t         _lumiBlock;
ULong64_t         _eventNb;
Double_t        _weight;
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
    treeReader._currentTreePtr->SetBranchAddress("_weight", &_weight, &b__weight);
    treeReader._currentTreePtr->SetBranchAddress("_lIsPrompt", _lIsPrompt, &b__lIsPrompt);
    treeReader._currentTreePtr->SetBranchAddress("_lMatchPdgId", _lMatchPdgId, &b__lMatchPdgId);
    treeReader._currentTreePtr->SetBranchAddress("_lMatchCharge", _lMatchCharge, &b__lMatchCharge);
    treeReader._currentTreePtr->SetBranchAddress("_lMomPdgId",  _lMomPdgId, &b__lMomPdgId);
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




// help function for initializing the histograms
std::vector< HistInfo > makeDistributionInfo( const std::string& process ){
    std::vector< HistInfo > histInfoVec = {
	HistInfo( "leptonPtLeading", "p_{T}^{leading lepton} (GeV)", 10, 25, 200 ),
	HistInfo( "leptonPtSubLeading", "p_{T}^{subleading lepton} (GeV)", 10, 15, 150 ),
	HistInfo( "leptonEtaLeading", "|#eta|^{leading lepton}", 10, 0, 2.5 ),
	HistInfo( "leptonEtaSubLeading", "|#eta|^{subleading lepton}", 10, 0, 2.5 ),	
	//HistInfo( "met", "E_{T}^{miss} (GeV)", 10, 0, 300 ),
	( ( process == "DY" ) ? 
	    HistInfo( "mll", "M_{ll} (GeV)", 25, 70, 110 ) : 
	    HistInfo( "mll", "M_{ll} (GeV)", 25, 0, 200 ) ),
	//HistInfo( "ltmet", "L_{T} + E_{T}^{miss} (GeV)", 10, 0, 300 ),
	//HistInfo( "ht", "H_{T} (GeV)", 10, 0, 600 ),
	//HistInfo( "mt2l", "M_{T}^{2l} (GeV)", 10, 0, 300 ),
	//HistInfo( "nJets", "number of jets", 8, 0, 8 ),
	//HistInfo( "nBJets", "number of b-jets (medium deep CSV)", 4, 0, 4 ),
	//HistInfo( "nVertex", "number of vertices", 10, 0, 70 )
    };
    return histInfoVec;
}


// help function for reading the charge flip map
std::shared_ptr< TH2D > readChargeFlipMap( const std::string& year , const std::string& flavour){
    TFile* frFile;
    if(flavour == "electron"){
    frFile = TFile::Open( ("chargeFlipMaps/chargeFlipMap_MC_electron_" + year + ".root" ).c_str() );
    } else{ 
    frFile = TFile::Open( ("chargeFlipMaps/chargeFlipMap_MC_muon_" + year + ".root" ).c_str() );
    }
    std::shared_ptr< TH2D > frMap( dynamic_cast< TH2D* >( 
	frFile->Get( ( "chargeFlipRate_electron_" + year ).c_str() ) ) );
    frMap->SetDirectory( gROOT );
    frFile->Close();
    return frMap;
} 

//======================================================= main function ====================================================
void closureTest_MC( const std::string& process,
                     const std::string& flavour, 
		     const std::string& year, 
		     const std::string& sampleListFile,
		     const std::string& sampleDirectory,
		     const long nEntries ){

    // check process string
    if( ! (process == "TT" || process == "DY" ) ){
        throw std::invalid_argument( "Given closure test process argument is '" + process 
		+ "' while it should be DY or TT." );
    }

    // make collection of histograms
    std::vector< std::shared_ptr< TH1D > > observedHists; 
    std::vector< std::shared_ptr< TH1D > > predictedHists;

    std::vector< HistInfo > histInfoVec = makeDistributionInfo( process );

    for( const auto& histInfo : histInfoVec ){
        observedHists.push_back( histInfo.makeHist( 
	    histInfo.name() + "_observed_" + process + "_" + year ) );
        predictedHists.push_back( histInfo.makeHist( 
	    histInfo.name() + "_predicted_"  + process + "_" + year ) );
    }
    
    // read fake-rate map corresponding to this year and flavor 
    std::shared_ptr< TH2D > chargeFlipMap_electron = readChargeFlipMap( year, flavour );


    // make TreeReader and loop over samples
    TreeReader treeReader( sampleListFile, sampleDirectory );
    for( unsigned i = 0; i < treeReader.numberOfSamples(); ++i ){
        treeReader.initSample( false, false );
        setBranchAddresses(treeReader);

    
        // loop over entries
        long unsigned numberOfEntries = treeReader.numberOfEntries();
        if( nEntries>0 && (unsigned)nEntries<numberOfEntries ){
            numberOfEntries = (unsigned) nEntries;
        }

        std::cout << "starting loop over " << numberOfEntries << " events." << std::endl;
        for( long unsigned entry = 0; entry < numberOfEntries; ++entry ){
            treeReader._currentTreePtr->GetEntry(entry); 
	    	    
            // apply event selection (already made in skims by Gianny)
            // if( !chargeFlips::passChargeFlipEventSelection( event, true, false, false ) ) continue;

            // get electron collection
            // ElectronCollection electrons = event.electronCollection();

	    // consider only events with only prompt leptons
            // bool promptElectrons = true;
            // for( const auto& electronPtr : electrons ){
            //    if( ! electronPtr->isPrompt() ){
            //        promptElectrons = false;
            //        break;
            //    }
            //}
            //if( !promptElectrons ) continue;
            
            // check lepton indices
            if( (_lFlavor[l1]==0 && fabs(l1_pt-_lPtCorr[l1])>1e-3)
                       || (_lFlavor[l1]==1 && fabs(l1_pt-_lPt[l1])>1e-3)){
                std::string msg = "ERROR: something wrong with first lepton index...";
                msg.append( " l1_pt: "+std::to_string(l1_pt)+"," );
                msg.append( " l1(pt): "+std::to_string(_lPt[l1])+"," );
                msg.append( " l1(ptcorr): "+std::to_string(_lPtCorr[l1])+"." );
                throw std::runtime_error(msg);
                }
                
             // check lepton indices
             if( (_lFlavor[l2]==0 && fabs(l2_pt-_lPtCorr[l2])>1e-3)
                        || (_lFlavor[l2]==1 && fabs(l2_pt-_lPt[l2])>1e-3) ){
                 std::string msg = "ERROR: something wrong with lepton indices...";
                 msg.append( " l2_pt: "+std::to_string(l2_pt)+"," );
                 msg.append( " l2(pt): "+std::to_string(_lPt[l2])+"," );
                 msg.append( " l2(ptcorr): "+std::to_string(_lPtCorr[l2])+"." );                                                                                                                                                                               throw std::runtime_error(msg);                                                                                                                                                                                                                } 
             // check first lepton
             bool considerL1 = true;
             if( flavour=="muon" && _lFlavor[l1]!=1 ) considerL1 = false;                                    
             if( flavour=="electron" && _lFlavor[l1]!=0 ) considerL1 = false;
             if( !_lIsPrompt[l1] ) considerL1 = false;  
             if( _lMatchPdgId[l1]==22 ) considerL1 = false;
             bool l1IsChargeFlip = (_lCharge[l1]!=_lMatchCharge[l1]);


            // check second lepton
            bool considerL2 = true;
            if( flavour=="muon" && _lFlavor[l2]!=1 ) considerL2 = false;
            if( flavour=="electron" && _lFlavor[l2]!=0 ) considerL2 = false;
            if( !_lIsPrompt[l2] ) considerL2 = false;
            if( _lMatchPdgId[l2]==22 ) considerL2 = false;
            bool l2IsChargeFlip = (_lCharge[l2]!=_lMatchCharge[l2]);
            
            if( !(considerL1 && considerL2) ) continue;
            if(process == "DY" && !(fabs(mll-91.1876)<15)) continue;
            // compute plotting variables 
            std::vector< double > variables = { l1_pt, l2_pt,
                fabs(_lEta[l1]), fabs(_lEta[l2]),
                //event.metPt(),
                mll
                //electrons.scalarPtSum() + event.metPt(),
                //event.HT(),
                //mt( electrons.objectSum(), event.met() ),
                //static_cast< double >( event.numberOfJets() ),
                //static_cast< double >( event.numberOfMediumBTaggedJets() ),
                //static_cast< double >( event.numberOfVertices() )
            };
                
            // event is 'observed' if an electron is assigned the wrong charge
            //bool isObserved = false;
            //for( const auto& electronPtr:  electrons ){
            //    if( electronPtr->isChargeFlip() ){
            //        isObserved = true;
            //    }
            //}

            if( l1IsChargeFlip || l2IsChargeFlip ){
                for( std::vector< double >::size_type v = 0; v < variables.size(); ++v ){
                    observedHists[v]->Fill( std::min( variables[v],  
			histInfoVec[v].maxBinCenter() ), 1 );//1: event weight
                }

            }
                // compute event weight with fake-rate
                bool electron = false;
                if(flavour == "electron"){electron = true; }

                double weight = chargeFlips::chargeFlipWeight( 
				    _lFlavor[l1],_lFlavor[l2], _lPt[l1], _lPt[l2], _lEta[l1], _lEta[l2], electron, chargeFlipMap_electron );
                for( std::vector< double >::size_type v = 0; v < variables.size(); ++v ){
                    predictedHists[v]->Fill( std::min( variables[v],  
			histInfoVec[v].maxBinCenter() ), weight );//1: weight
                }
        }
    }

    // write file
    std::string fileName = "closurePlotsv2_MC_" + process + "_" + year + ".root";
    TFile* outputFilePtr = TFile::Open( fileName.c_str(), "RECREATE" );
    outputFilePtr->cd();
    for( std::vector< HistInfo >::size_type v = 0; v < histInfoVec.size(); ++v ){
        TH1D* predicted = predictedHists[v].get();
        TH1D* observed = observedHists[v].get();
        std::string pName = histInfoVec[v].name() + "_" + process + "_" + year + "_predicted";
        std::string oName = histInfoVec[v].name() + "_" + process + "_" + year + "_observed";
        predicted->SetName( pName.c_str() );
        observed->SetName( oName.c_str() );
        predicted->Write();
        observed->Write();
    }
    outputFilePtr->Close();
}

int main( int argc, char* argv[] ){

    std::cerr << "###starting###" << std::endl;
    // check command line arguments
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    if( !( argvStr.size() == 7 ) ){
        std::cerr << "ERROR: found " << argc-1 << " command line args,";
        std::cerr << " while 6 are needed:" << std::endl;
        std::cerr << "  - process" << std::endl;
	std::cerr << "  - flavour (only 'electron' supported for now)" << std::endl;
        std::cerr << "  - year" << std::endl;
        std::cerr << "  - sample list" << std::endl;
        std::cerr << "  - sample directory" << std::endl;
        std::cerr << "  - number of entries" << std::endl;
        return 1;
    }
    std::string process = argvStr[1];
    std::string flavour = argvStr[2];
    std::string year = argvStr[3];
    std::string sampleList = argvStr[4];
    std::string sampleDirectory = argvStr[5];
    long nEntries = std::stol(argvStr[6]);
    setTDRStyle();
    closureTest_MC(process, flavour, year, sampleList, sampleDirectory, nEntries);
    std::cerr << "###done###" << std::endl;
    return 0; 
}
