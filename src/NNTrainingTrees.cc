//include ROOT classes
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TTree.h"
#include "TROOT.h"

//include C++ library classes
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <tuple>

//include other parts of the code
#include "../interface/treeReader.h"
#include "../interface/analysisTools.h"
#include "../interface/trilepTools.h"
#include "../interface/Reweighter.h"
#include "../interface/HistInfo.h"
#include "../interface/HistCollectionDist.h"
#include "../interface/HistCollectionSample.h"
#include "../interface/kinematicTools.h"
#include "../interface/TrainingTree.h"
#include "../interface/BDTReader.h"
#include "../plotting/plotCode.h"
#include "../plotting/tdrStyle.h"

//include TMVA classes
//#include "TMVA/Tools.h"
//#include "TMVA/Reader.h"

void treeReader::Analyze(){

    //Set CMS plotting style
    setTDRStyle();
    gROOT->SetBatch(kTRUE);

    //read samples and cross sections from txt file
    readSamples("sampleLists/samples_ewkino_2016.txt.txt", "../../ntuples_ewkino");

    //name      xlabel    nBins,  min, max
    histInfo = {
        HistInfo("mll", "M_{ll} (GeV)", 50, 0, 400),
        HistInfo("mt", "M_{T} (GeV)", 50, 0, 600),
        HistInfo("met", "M_{T} (GeV)", 50, 0, 800),
        HistInfo("LTPlusMET", "L_{T} + E_{T}^{miss} (GeV)", 50, 0, 1200),
        HistInfo("HT", "H_{T} (GeV)", 50, 0, 800), 
        HistInfo("m3l", "M_{3l} (GeV)", 50, 0, 1200),
        HistInfo("mt3l", "M_{T}(3l + MET) (GeV)", 50, 0, 800),
    };

    const unsigned nDist = histInfo.size(); //number of distributions to plot

    const std::vector<std::string> mllNames = {"mllInclusive", "onZ", "offZ"};
    const size_t nMll = mllNames.size();

    //initialize vector holding all histograms
    std::vector< std::vector< std::vector< std::shared_ptr< TH1D > > > > hists(nMll);
    for(unsigned m = 0; m < nMll; ++m){
        hists[m] = std::vector< std::vector< std::shared_ptr< TH1D > > >(nDist);
        for(unsigned dist = 0; dist < nDist; ++dist){
            hists[m][dist] = std::vector< std::shared_ptr< TH1D > >( samples.size() );
            for(size_t sam = 0; sam < samples.size(); ++sam){
                hists[m][dist][sam] = histInfo[dist].makeHist( mllNames[m] + samples[sam].getUniqueName() );
            }
        }
    }

    //loop over all samples 
    for(size_t sam = 0; sam < samples.size(); ++sam){

        initSample();
        std::cout<<"Entries in "<< currentSample.getFileName() << " " << nEntries << std::endl;

        double progress = 0; 	//for printing progress bar
        for(long unsigned it = 0; it < nEntries; ++it){
            //print progress bar	
            if(it%100 == 0 && it != 0){
                progress += (double) (100./nEntries);
                analysisTools::printProgress(progress);
            } else if(it == nEntries -1){
                progress = 1.;
                analysisTools::printProgress(progress);
            }

            GetEntry(it);

            //apply triggers and MET filters
            if( !passTriggerCocktail() ) continue;
            if( !passMETFilters() ) continue;

            //vector containing good lepton indices
            std::vector<unsigned> ind;

            //select leptons
            const unsigned lCount = selectLep(ind);
            if(lCount != 3) continue;

            //require pt cuts (25, 15, 10) to be passed
            if(!passPtCuts(ind)) continue;

            //require presence of OSSF pair
            if(trilep::flavorChargeComb(ind, _lFlavor, _lCharge, lCount) != 0) continue; 

            //remove overlap between samples
            if(photonOverlap(currentSample, false)) continue;

            //make lorentzvectors for leptons
            TLorentzVector lepV[lCount];
            for(unsigned l = 0; l < lCount; ++l) lepV[l].SetPtEtaPhiE(_lPt[ind[l]], _lEta[ind[l]], _lPhi[ind[l]], _lE[ind[l]]);

            //find best Z mass and use it to determine the mll category
            std::pair<unsigned, unsigned> bestZ = trilep::bestZ(lepV, ind, _lFlavor, _lCharge, lCount);
            double mll = (lepV[bestZ.first] + lepV[bestZ.second]).M();
            unsigned mllCat;
            if( fabs(mll - 91.1876) >= 15){
                mllCat = 1;
            } else {
                mllCat = 0;
            }
    
            //compute kinematic quantities of the event

            //find lepton from W decay
            TLorentzVector met;
            met.SetPtEtaPhiE( _met, 0, _metPhi, _met);
            double mt = kinematics::mt(met, lepV

            //write variables to histograms 
            for(unsigned dist = 0; dist < nDist; ++dist){
                hists[mllCat][dist][sam]->Fill(std::min(fill[dist], histInfo[dist].maxBinCenter() ), weight);
            }
        }

        //set histograms to 0 if negative
        for(unsigned m = 0; m < nMll; ++m){
            for(unsigned dist = 0; dist < nDist; ++dist){
                analysisTools::setNegativeBinsToZero( hists[m][cat][dist][sam].get() );
            }	
        }
    }

    /*
    //merge histograms with the same physical background
    std::vector<std::string> proc = {"Data", "tZq", "WZ", "multiboson", "TT + Z", "TT/T + X", "X + #gamma", "ZZ/H", "Nonprompt e/#mu"};
    std::vector< std::vector< std::vector< std::vector< TH1D* > > > > mergedHists(nMll);
    for(unsigned mll = 0; mll < nMll; ++mll){
        mergedHists[mll] = std::vector< std::vector < std::vector < TH1D* > > >(nCat);
        for(unsigned cat = 0; cat < nCat; ++cat){
            for(unsigned dist = 0; dist < nDist; ++dist){
                mergedHists[mll][cat].push_back(std::vector<TH1D*>(proc.size() ) );

                //cut off loop before nonprompt contribution
                for(size_t m = 0, sam = 0; m < proc.size() - 1; ++m){
                    mergedHists[mll][cat][dist][m] = (TH1D*) hists[mll][cat][dist][sam]->Clone();
                    while(sam < samples.size() - 1 && samples[sam].getProcessName() == samples[sam + 1].getProcessName() ){
                        mergedHists[mll][cat][dist][m]->Add(hists[mll][cat][dist][sam + 1].get());
                        ++sam;
                    }
                    ++sam;
                }

                //add nonprompt histogram
                mergedHists[mll][cat][dist][proc.size() - 1] = (TH1D*) hists[mll][cat][dist][samples.size()].get()->Clone();
            }
        }
    }
    */
}


int main(){
    treeReader reader;
    reader.Analyze();
    return 0;
}