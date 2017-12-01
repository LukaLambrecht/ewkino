#include "../interface/Reweighter.h"

Reweighter::Reweighter(){
    //initialize pu weights
    const std::string vars[3] = {"central", "down", "up"};
    for(unsigned v = 0; v < 3; ++v){
        TFile* puFile = TFile::Open((const TString&) "../weights/puw_nTrueInt_Moriond2017_36p5fb_Summer16_" + vars[v] + ".root");
        puWeights[v] = (TH1D*) puFile->Get("puw");
        puFile->Close();
    }
    //initialize b-tag weights
    //currently assuming medium WP of deepCSV tagger 
    bTagCalib = new BTagCalibration("deepCsv", "../weights/DeepCSV_Moriond17_B_H.csv");
    bTagCalibReader =  new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"});
    bTagCalibReader->load(calib, BTagEntry::FLAV_B, "comb");
    bTagCalibReader->load(calib, BTagEntry::FLAV_C, "comb");
    bTagCalibReader->load(calib, BTagEntry::FLAV_UDSG, "incl");    
    //initialize b-tag efficiencies
    TFile* bTagFile = TFile::Open("../weights/bTagEff_deepCSV_medium_cleaned.root");
    const std::string quarkFlavors[3] = {"udsg", "charm", "beauty"};
    for(unsigned flav = 0; flav < nFlav; ++flav){
        bTagEff[flav] = (TH1D*) bTagFile->Get("bTagEff_" + quarkFlavors[flav];
    }
    bTagFile.Close();
    //Read Muon reco SF Weights
    TFile* muonRecoFile = TFile::Open("../weights/muonTrackingSF_2016.root");
    muonRecoSF = (TGraph*) muonRecoFile->Get("ratio_eff_eta3_dr030e030_corr");
    //Read Electron reco SF Weights
    TFile* electronRecoFile = TFile::Open("../weights/electronRecoSF_2016.root");
    electronRecoSF = (TH2D*) electronRecoFile("EGamma_SF2D");
    //Read muon id SF weights
    TFile* muonMediumFile = TFile::Open("../weights/muonScaleFactors_MediumIDtoReco.root");
    TFile* muonMiniIsoFile = TFile::Open("../weights/muonScaleFactors_miniIso0p4toMediumID.root");
    TFile* muonIPFile = TFile::Open("../weights/muonScaleFactors_dxy0p05dz0p1toMediumID.root");
    TFile* muonSIP3DFile = TFile::Open("../weights/muonScaleFactors_sip3d4toMediumID.root");
    muonMediumSF = (TH2D*) muonMediumFile->Get("SF");
    muonMiniIsoSF = (TH2D*) muonMiniIsoFile->Get("SF");
    muonIPSF = (TH2D*) muonIPFile->Get("SF");
    muonSIP3DSF = (Th2D*) muonSIP3DFile->Get("SF");
    muonMediumFile->Close();
    muonMiniIsoFile->Close();
    muonIPFile->Close();
    muonSIP3DFile->Close();
    //read electron id SF weights       
    TFile* electronIdFile = TFile::Open("../weights/electronIDScalFactors.root");
    electronIdSF = (TH2D*) electronIdFile->Get("GsfElectronToLeptonMvaTIDEmuTightIP2DSIP3D8mini04");
    electronIdFile->Close();
}

Reweighter::~Reweighter(){
    delete bTagCalib;
    delete bTagCalibReader;
}

double Reweighter::puWeight(const double nTrueInt, const unsigned period, const unsigned unc){
    if(unc < 3){
        if(period == 0) return puWeights[unc]->GetBinContent(puWeights[unc]->FindBin(std::min(nTrueInt, (float) 49.) ) );
        else return 1.;                                                 //weights for 2017 data not yet available
    }
    else{
        std::cerr << "wrong pu uncertainty requested: returning weight 1" << std::endl;
        return 1.;
    }
}

double Reweighter::bTagWeight(const unsigned jetFlavor, const double jetPt, const double jetEta, const double jetCSV, const unsigned unc = 0){
    static const BTagEntry::JetFlavor flavorEntries[3] = {BTagEntry::FLAV_UDSG, BTagEntry::FLAV_C, BTagEntry::FLAV_B};
    static const std::string uncName[3] = {"central", "down", "up"};
    return bTagCalibReader.eval_auto_bounds(uncName[unc], flavInd(jetFlavor), jetEta, jetPt, jetCSV);
}

double Reweighter::bTagEff(const unsigned jetFlavor, const double jetPt, const double jetEta){
    return bTagEff[flavInd(jetFlavor)]->GetBinContent(bTagEff[flavInd(jetFlavor)]->FindBin(std::min(jetPt, 599.)), std::min(fabs(jetEta), 2.4)) );
}

double Reweighter::muonRecoWeight(const double eta){
    return muonRecoSF->Eval(eta);
}

double Reweighter::electronRecoWeight(const double superClusterEta, const double pt){
    return electronRecoSF->Eval(superClusterEta, pt);
}

double Reweighter::muonIdWeight(const double pt, const double eta){
    double sf = muonMediumFile->GetBinContent(muonMediumFile->FindBin(std::min(pt, 119.), std::min(fabs(eta), 2.4) ) );
    sf *= muonMiniIsoFile->GetBinContent(muonMiniIsoFile->FindBin(std::min(pt, 119.), std::min(fabs(eta), 2.4) ) );
    sf *= muonIPFile->GetBinContent(muonMIPFile->FindBin(std::min(pt, 119.), std::min(fabs(eta), 2.4) ) );
    sf *= muonSIP3DFile->GetBinContent(muonMSIP3DFile->FindBin(std::min(pt, 119.), std::min(fabs(eta), 2.4) ) );
    return sf;
}

double Reweighter::electronIdWeight(const double pt, const double eta){
    return electronIdSF->GetBinContent(electronIdSF->FindBin(std::min(pt, 199.), std::min(fabs(eta), 2.5) ) );
}
