/*
Perform a charge flip measurement in data
*/

// include c++ library classes
#include <memory>
#include <string>
#include <fstream>

// include ROOT classes
#include "TH1D.h"
#include "TStyle.h"

// include other parts of framework
#include "../TreeReader/interface/TreeReader.h"
#include "../Tools/interface/analysisTools.h"
#include "../Tools/interface/HistInfo.h"
#include "../Tools/interface/histogramTools.h"
#include "../Tools/interface/ConstantFit.h"
#include "../Tools/interface/systemTools.h"
#include "../Tools/interface/mt2.h"
#include "../Tools/interface/stringTools.h"
#include "../Event/interface/Event.h"
#include "../plotting/interface/tdrStyle.h"
#include "../plotting/interface/plotCode.h"

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
// help function to initialize histograms
std::vector< HistInfo > makeDistributionInfo(){
    std::vector< HistInfo > histInfoVec = {
        HistInfo( "mll", "M_{ll} (GeV)", 40, 80, 105 ),
        HistInfo( "leptonPtLeading", "p_{T}^{leading lepton} (GeV)", 40, 25, 125 ),
        HistInfo( "leptonPtSubLeading", "p_{T}^{subleading lepton} (GeV)", 30, 15, 95 ),

        HistInfo( "leptonEtaLeading", "|#eta|^{leading lepton}", 30, 0, 2.5 ),
        HistInfo( "leptonEtaSubLeading", "|#eta|^{subleading lepton}", 30, 0, 2.5 ),

        //HistInfo( "met", "E_{T}^{miss} (GeV)", 40, 0, 100 ),
        //HistInfo( "ltmet", "L_{T} + E_{T}^{miss} (GeV)", 40, 0, 320 ),
        //HistInfo( "ht", "H_{T} (GeV)", 40, 0, 600 ),
        //HistInfo( "mt2l", "M_{T}^{2l} (GeV)", 40, 0, 300 ),

        //HistInfo( "ptll", "P_{T}^{ll} (GeV)", 40, 0, 300 ),
        //HistInfo( "mt2ll", "M_{T2}^{ll} (GeV)", 40, 0, 120 ),

        //HistInfo( "nJets", "number of jets", 8, 0, 8 ),
        //HistInfo( "nBJets", "number of b-jets (tight deep CSV)", 4, 0, 4 ),
        //HistInfo( "nVertex", "number of vertices", 40, 0, 70 )
    };
    return histInfoVec;
}


// help function to calculate event variables
std::vector< double > computeVariables(){
    return {
        mll,
        l1_pt,
        l2_pt,
        fabs(_lEta[l1]),
        fabs(_lEta[l2])
        //event.metPt(),
        //( event.electron( 0 ).pt() + event.electron( 1 ).pt() + event.metPt() ),
        //event.jetCollection().scalarPtSum(),
        //mt( electronSum, event.met() ),
        //electronSum.pt(),
        //mt2::mt2( event.electron( 0 ), event.electron( 1 ), event.met() ),
        //static_cast< double >( event.numberOfJets() ),
        //static_cast< double >( event.numberOfTightBTaggedJets() ),
        //static_cast< double >( event.numberOfVertices() )
    };
}


void deriveChargeFlipCorrections( const std::string& year,
				  const std::string& sampleListFile,
				  const std::string& sampleDirectory,
				  const long nEntries ){

    // read MC charge-flip maps
    std::string cfMapFileName = "chargeFlipMaps/chargeFlipMap_MC_electron" + year + ".root";
    TFile* chargeFlipMapFile = TFile::Open( ( cfMapFileName ).c_str() );
    std::shared_ptr< TH2 > chargeFlipMap_MC( dynamic_cast< TH2* >( 
    chargeFlipMapFile->Get( ( "chargeFlipRate_electron_" + year ).c_str() ) ) );
    chargeFlipMap_MC->SetDirectory( gROOT );
    chargeFlipMapFile->Close();

    // initialize histograms for each contribution
    std::vector< std::string > contributions = { "Data", "Charge-flips", "Nonprompt", "Prompt" };
    std::map< std::string, std::vector< std::shared_ptr< TH1D > > > histogramMap;
    std::vector< HistInfo > histInfoVector = makeDistributionInfo();
    for( const auto& process : contributions ){
	for( const auto& dist : histInfoVector ){
	    std::string histName = dist.name() + "_" + process + "_" + year;
	    histogramMap[ process ].push_back( dist.makeHist( histName ) );
	}
    }
    
    // make TreeReader and loop over samples
    TreeReader treeReader( sampleListFile, sampleDirectory );
    for( unsigned i = 0; i < treeReader.numberOfSamples(); ++i ){
        treeReader.initSample(false, false);
        setBranchAddresses(treeReader);

	// loop over entries
        long unsigned numberOfEntries = treeReader.numberOfEntries();
        if( nEntries>0 && (unsigned)nEntries<numberOfEntries ){
            numberOfEntries = (unsigned) nEntries;
        }
        for( long unsigned entry = 0; entry < numberOfEntries; ++entry ){

	    // build event
            treeReader._currentTreePtr->GetEntry(entry);

            // apply selection (already made in Gianny's skims)
            //if( ! chargeFlips::passChargeFlipEventSelection( event, true, true, true) ) continue;

            // apply further selections
            if( l1_pt < 25. ) continue;
            if( l2_pt < 15. ) continue;
//!!!         //if( !( event.passTriggers_e() || event.passTriggers_ee() ) ) continue;
    
	    // find if leptons are same sign
            double weight = _weight;                                                                 //!!!
            bool isSameSign = !(isOS);
            std::string contributionName;

	    // determine correct category
	    // same sign data
            if( _runNb != 1 && isSameSign ){//( event.isData() && isSameSign ){
                contributionName = "Data";
	    // opposite sign data (-> multiply by charge flip weight)
            } else if( _runNb != 1 ){
                contributionName = "Charge-flips";
                bool electron = true; //muon not yet supported, no real need for now
                weight *= chargeFlips::chargeFlipWeight( _lFlavor[l1],_lFlavor[l2], _lPt[l1], _lPt[l2], _lEta[l1], _lEta[l2], electron, chargeFlipMap_MC );
	    // same sign MC
            } else if( isSameSign ){
                bool isPrompt = true;
                if( !(_lIsPrompt[l2]) || !(_lIsPrompt[l1]) ){
                    isPrompt = false;
                    break;
                }
                if( isPrompt ){
                    contributionName = "Prompt";
                } else {
                    contributionName = "Nonprompt";
                }

            // opposite sign MC events (-> skip)
            } else {
                continue;
            }

            // fill histograms
            auto fillVariables = computeVariables();
            for( size_t dist = 0; dist < histInfoVector.size(); ++dist ){
                histogram::fillValue( histogramMap[ contributionName ][ dist ].get(), 
				      fillVariables[ dist ], weight );
            }
        }
    }

    // set negative bins to zero
    for( const auto& process : contributions ){
        for( size_t dist = 0; dist < histInfoVector.size(); ++dist ){
            analysisTools::setNegativeBinsToZero( histogramMap[ process ][ dist ].get() );
        }
    }

    // determine the scale factor by fitting Mll
    std::shared_ptr< TH1D > chargeFlipPrediction( dynamic_cast< TH1D* >( 
	histogramMap.at( "Charge-flips" )[ 0 ]->Clone() ) );
    std::shared_ptr< TH1D > chargeFlipObserved( dynamic_cast< TH1D* >( 
	histogramMap.at( "Data" )[ 0 ]->Clone() ) );

    // subtract backgrounds from data
    chargeFlipObserved->Add( histogramMap.at( "Prompt" )[ 0 ].get(), -1. );
    chargeFlipObserved->Add( histogramMap.at( "Nonprompt" )[ 0 ].get(), -1. );

    // avoid negative bins in data after the subtraction
    analysisTools::setNegativeBinsToZero( chargeFlipObserved.get() );

    // fit the ratio of observed to predicted to get the scale-factor
    chargeFlipObserved->Divide( chargeFlipPrediction.get() );
    ConstantFit chargeFlipRatioFit( chargeFlipObserved );
    
    // write scale factor to txt file
    std::ofstream scaleFactorDump( "chargeFlipScaleFactor_" + year + ".txt" );
    scaleFactorDump << "charge flip scale factor for " << year;
    scaleFactorDump << " = " << chargeFlipRatioFit.value();
    scaleFactorDump << " +- " << chargeFlipRatioFit.uncertainty() << "\n";
    scaleFactorDump.close();

    // header for plots
    std::string header;
    if( year == "2016" ){
	header = "35.9 fb^{-1}";
    } else if( year == "2017" ){
	header = "41.5 fb^{-1}";
    } else{
	header = "59.7 fb^{-1}";
    }

    // make closure plots before applying the scale-factor
    std::string outputDirectoryBeforeSF = ( "closurePlots_chargeFlips_data_beforeSF_" + year );
    systemTools::makeDirectory( outputDirectoryBeforeSF );
    for( size_t dist = 0; dist < histInfoVector.size(); ++dist ){
        TH1D* predictedHistograms[3] = { histogramMap.at( "Charge-flips" )[ dist ].get(), 
					 histogramMap.at( "Prompt" )[ dist ].get(), 
					 histogramMap.at( "Nonprompt" )[ dist ].get() };

	plotDataVSMC( histogramMap.at( "Data" )[ dist ].get(), predictedHistograms, 
		      &contributions[0], 3, stringTools::formatDirectoryName( outputDirectoryBeforeSF ) 
		      + histInfoVector[ dist ].name() + "_closureTest_chargeFlips_data_beforeSF_" 
		      + year + ".pdf", "", false, false, header );
	plotDataVSMC( histogramMap.at( "Data" )[ dist ].get(), predictedHistograms, 
		      &contributions[0], 3, stringTools::formatDirectoryName( outputDirectoryBeforeSF ) 
		      + histInfoVector[ dist ].name() + "_closureTest_chargeFlips_data_beforeSF_" 
		      + year + "_log.pdf", "", true, false, header );
    }

    // make closure plots after applying the scale-factor
    std::string outputDirectoryAfterSF = ( "closurePlots_chargeFlips_data_afterSF_" + year );
    systemTools::makeDirectory( outputDirectoryAfterSF );
    for( size_t dist = 0; dist < histInfoVector.size(); ++dist ){

        // scale charge-flip prediction
        histogramMap.at( "Charge-flips" )[ dist ]->Scale( chargeFlipRatioFit.value() );

        TH1D* predictedHistograms[3] = { histogramMap.at( "Charge-flips" )[ dist ].get(), 
					 histogramMap.at( "Prompt" )[ dist ].get(), 
					 histogramMap.at( "Nonprompt" )[ dist ].get() };

        // make 20% systematic uncertainty band 
        TH1D* systUnc = dynamic_cast< TH1D* >( predictedHistograms[0]->Clone() );
        for( int b = 1; b < systUnc->GetNbinsX() + 1; ++b ){
            systUnc->SetBinContent( b , systUnc->GetBinContent(b)*0.2 );
        }

	plotDataVSMC( histogramMap.at( "Data" )[ dist ].get(), predictedHistograms, 
		      &contributions[0], 3, stringTools::formatDirectoryName( outputDirectoryAfterSF ) 
		      + histInfoVector[ dist ].name() + "_closureTest_chargeFlips_data_afterSF_" 
		      + year + ".pdf", "", false, false, header, systUnc );
	plotDataVSMC( histogramMap.at( "Data" )[ dist ].get(), predictedHistograms, 
		      &contributions[0], 3, stringTools::formatDirectoryName( outputDirectoryAfterSF ) 
		      + histInfoVector[ dist ].name() + "_closureTest_chargeFlips_data_afterSF_" 
		      + year + "_log.pdf", "", true, false, header, systUnc );
    }
}


int main( int argc, char* argv[] ){

    std::cerr << "###starting###" << std::endl;
    // check command line arguments
    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    if( !( argvStr.size() == 6 ) ){
        std::cerr << "ERROR: found " << argc-1 << " command line args,";
        std::cerr << " while 5 are needed:" << std::endl;
        std::cerr << "  - flavour (only 'electron' supported for now)" << std::endl;
        std::cerr << "  - year" << std::endl;
        std::cerr << "  - sample list" << std::endl;
        std::cerr << "  - sample directory" << std::endl;
        std::cerr << "  - number of entries" << std::endl;
        return 1;
    }
    std::string flavor = argvStr[1];
    std::string year = argvStr[2];
    std::string sampleList = argvStr[3];
    std::string sampleDirectory = argvStr[4];
    long nEntries = std::stol(argvStr[5]);
    setTDRStyle();
    deriveChargeFlipCorrections(
        year, sampleList, sampleDirectory, nEntries);
    std::cerr << "###done###" << std::endl;
    return 0;
}
