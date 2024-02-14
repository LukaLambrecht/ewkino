/*
Study efficiency of event reconstruction
*/

// In particular: find efficiency of the lepton-from-W recognition,
// the b-jet-from-top-quark recognition, and the recoiling-jet recognition.

// Note: this script is not really correct, as it only compares detector level
// to particle level; but particle level is not guaranteed to be correct,
// as the same event reconstruction algorithms are applied as on detector level.
// Better to compare to "true" gen level.

// include c++ library classes 
#include <string>
#include <vector>
#include <exception>
#include <iostream>

// include ROOT classes 
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

// include other parts of framework
#include "../../TreeReader/interface/TreeReader.h"
#include "../../Tools/interface/stringTools.h"
#include "../../Event/interface/Event.h"

// include other parts of the analysis code
#include "../eventselection/interface/eventSelections.h"
#include "../eventselection/interface/eventFlattening.h"
#include "../eventselection/interface/eventSelectionsParticleLevel.h"
#include "../eventselection/interface/eventFlatteningParticleLevel.h"

std::vector<std::shared_ptr<TH1D>> recoEfficiency(
    const std::string& pathToFile,
    long nEvents ){
    
    // initialize output histogram
    std::shared_ptr<TH1D> histLW = std::make_shared<TH1D>(
        "histLW", "histLW;correctly identified;number of events", 2, -0.5, 1.5);
    std::shared_ptr<TH1D> histBJet = std::make_shared<TH1D>(
        "histBJet", "histBJet;correctly identified;number of events", 2, -0.5, 1.5);
    std::shared_ptr<TH1D> histRJet = std::make_shared<TH1D>(
        "histRJet", "histRJet;correctly identified;number of events", 2, -0.5, 1.5);
    std::shared_ptr<TH1D> histTopMassPL = std::make_shared<TH1D>(
        "histTopMassPL", "histTopMass;reconstructed top mass;number of events", 30, 100., 300.);
    std::shared_ptr<TH1D> histTopMassDL = std::make_shared<TH1D>(
        "histTopMassDL", "histTopMass;reconstructed top mass;number of events", 30, 100., 300.);

    // initialize TreeReader
    TreeReader treeReader;
    treeReader.initSampleFromFile( pathToFile );
    
    // do event loop
    long numberOfEntries = treeReader.numberOfEntries();
    if( nEvents<0 || nEvents>numberOfEntries ) nEvents = numberOfEntries;
    for(long entry = 0; entry < nEvents; entry++){
	if(entry%1000 == 0) std::cout<<"processed: "<<entry<<" of "<<nEvents<<std::endl;
	Event event = treeReader.buildEvent(entry, false, false, false, false, true);
	// do event selection at detector level
	bool pass_dl = pass_signalregion( event, "3tight", "nominal", true );
	// do event selection at particle level
	bool pass_pl = eventSelectionsParticleLevel::pass_signalregion_trilepton(event);
	if( !pass_dl || !pass_pl ) continue;
	// get jet and lepton collections at detector and particle level
	LeptonCollection leptons_dl = event.leptonCollection();
	LeptonParticleLevelCollection leptons_pl = event.leptonParticleLevelCollection();
	JetCollection jets_dl = event.jetCollection();
	JetParticleLevelCollection jets_pl = event.jetParticleLevelCollection();
	
	// find lepton from W boson
	unsigned int lWidx_dl = event.WLeptonIndex();
	unsigned int lWidx_pl = leptons_pl.WLeptonIndex();
	std::pair<unsigned int, double> temp = leptons_pl[lWidx_pl].closestLeptonIdxAndDeltaR(leptons_dl);
	unsigned int lWidx_matched = temp.first;
	//double matchDeltaR = temp.second;
	// printouts for testing
	/*std::cout << "----" << std::endl;
	std::cout << leptons_dl[lWidx_dl].pt() << " " << leptons_dl[lWidx_dl].eta() << " " << leptons_dl[lWidx_dl].phi() << std::endl;
	std::cout << leptons_pl[lWidx_pl].pt() << " " << leptons_pl[lWidx_pl].eta() << " " << leptons_pl[lWidx_pl].phi() << std::endl;
	std::cout << deltaR(leptons_dl[lWidx_dl], leptons_pl[lWidx_pl]) << std::endl;
	std::cout << lWidx_dl << " " << lWidx_matched << std::endl;*/
	// fill histogram
	if( lWidx_dl==lWidx_matched ) histLW->Fill(1);
	else histLW->Fill(0);
	
	// find b jet from top quark
	std::pair<double,double> pmz_dl = eventFlattening::pmzcandidates(leptons_dl[lWidx_dl], event.met());
	std::pair<double,int> topresults_dl = eventFlattening::besttopcandidate(jets_dl, 
	    leptons_dl[lWidx_dl], event.met(), pmz_dl.first, pmz_dl.second);
	unsigned int bjetIdx_dl = (unsigned) topresults_dl.second;
	if(jets_dl.numberOfMediumBTaggedJets()==0) bjetIdx_dl = 0;
	std::pair<double,double> pmz_pl = eventFlatteningParticleLevel::pmzcandidates(leptons_pl[lWidx_pl], event.metParticleLevel());
        std::pair<double,int> topresults_pl = eventFlatteningParticleLevel::besttopcandidate(jets_pl, 
            leptons_pl[lWidx_pl], event.metParticleLevel(), pmz_pl.first, pmz_pl.second);
        unsigned int bjetIdx_pl = (unsigned) topresults_pl.second;
	if(jets_pl.numberOfBJets()==0) bjetIdx_pl = 0;
	temp = jets_pl[bjetIdx_pl].closestJetIdxAndDeltaR(jets_dl);
        unsigned int bjetIdx_matched = temp.first;
        //double matchDeltaR = temp.second;
	// printouts for testing
	/*std::cout << "----" << std::endl;
        std::cout << jets_dl[bjetIdx_dl].pt() << " " << jets_dl[bjetIdx_dl].eta() << " " << jets_dl[bjetIdx_dl].phi() << std::endl;
        std::cout << jets_pl[bjetIdx_pl].pt() << " " << jets_pl[bjetIdx_pl].eta() << " " << jets_pl[bjetIdx_pl].phi() << std::endl;
        std::cout << deltaR(jets_dl[bjetIdx_dl], jets_pl[bjetIdx_pl]) << std::endl;
        std::cout << bjetIdx_dl << " " << bjetIdx_matched << std::endl;*/
	// fill histogram
	if( bjetIdx_dl==bjetIdx_matched ) histBJet->Fill(1);
        else histBJet->Fill(0);
	histTopMassDL->Fill(topresults_dl.first);
	histTopMassPL->Fill(topresults_pl.first);

	// find recoiling jet
	unsigned int recoilIdx_dl = 99;
	for(unsigned int i=0; i<jets_dl.size(); i++){
	    if(i != bjetIdx_dl){
		recoilIdx_dl = i;
		break;
	    }
	}
	unsigned int recoilIdx_pl = 99;
        for(unsigned int i=0; i<jets_pl.size(); i++){
            if(i != bjetIdx_pl){
                recoilIdx_pl = i;
                break;
            }
        }
	temp = jets_pl[recoilIdx_pl].closestJetIdxAndDeltaR(jets_dl);
        unsigned int recoilIdx_matched = temp.first;
	// printouts for testing
        /*std::cout << "----" << std::endl;
        std::cout << jets_dl[recoilIdx_dl].pt() << " " << jets_dl[recoilIdx_dl].eta() << " " << jets_dl[recoilIdx_dl].phi() << std::endl;
        std::cout << jets_pl[recoilIdx_pl].pt() << " " << jets_pl[recoilIdx_pl].eta() << " " << jets_pl[recoilIdx_pl].phi() << std::endl;
        std::cout << deltaR(jets_dl[recoilIdx_dl], jets_pl[recoilIdx_pl]) << std::endl;
        std::cout << recoilIdx_dl << " " << recoilIdx_matched << std::endl;*/
	// fill histogram
        if( recoilIdx_dl==recoilIdx_matched ) histRJet->Fill(1);
        else histRJet->Fill(0);
    }

    // return all histograms
    std::vector<std::shared_ptr<TH1D>> res;
    res.push_back(histLW);
    res.push_back(histBJet);
    res.push_back(histRJet);
    res.push_back(histTopMassPL);
    res.push_back(histTopMassDL);
    return res;
}


int main( int argc, char* argv[] ){

    std::cerr<<"###starting###"<<std::endl;
    if( argc != 4 ){
        std::cerr << "ERROR: NAME.cc requires the following arguments: " << std::endl;
	std::cerr << "- input_file_path" << std::endl;
	std::cerr << "- output_file_path" << std::endl;
	std::cerr << "- nevents" << std::endl;
	return -1;
    }

    std::vector< std::string > argvStr( &argv[0], &argv[0] + argc );
    std::string& input_file_path = argvStr[1];
    std::string& output_file_path = argvStr[2];
    long nevents = std::stol(argvStr[3]);
   
    // make the cutflow histogram
    std::vector<std::shared_ptr<TH1D>> res = recoEfficiency( input_file_path, nevents );

    // write to output file
    TFile* outputFilePtr = TFile::Open( output_file_path.c_str() , "RECREATE" );
    for( std::shared_ptr<TH1D> hist : res ){ hist->Write(); }
    outputFilePtr->Close();

    std::cerr<<"###done###"<<std::endl;
    return 0;
}
