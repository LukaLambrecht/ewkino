/*
Particle-level event variable computing.
This file is similar to eventFlattening.cc,
but uses particle-level variables rather than detector-level variables.
*/


// include header
#include "../interface/eventFlatteningParticleLevel.h"


// helper function to initialize a map of variables to default values
std::map< std::string, double > eventFlatteningParticleLevel::initVarMap(){
    // initialize a map of variables set to their default values
    // note: only simple variables for now, extend when needed.
    std::map< std::string, double> varmap = {
	// counters
	{"_yield",0.5},
	{"_nJets",0}, {"_nBJets",0},
	{"_nMuons",0},{"_nElectrons",0},
	// rest: to do if needed
    };
    return varmap;    
}

 
// main function //

std::map< std::string, double > eventFlatteningParticleLevel::eventToEntry( Event& event ){
    // fill one entry with event variables.

    // initialize all variables in the map
    std::map< std::string, double > varmap = initVarMap();
 
    // get correct object collections and met
    JetParticleLevelCollection jetcollection = event.jetParticleLevelCollection();
    JetParticleLevelCollection bjetcollection = jetcollection.PLbJetCollection();
    MetParticleLevel met = event.metParticleLevel();
    LeptonParticleLevelCollection lepcollection = event.leptonParticleLevelCollection();

    // remove taus
    lepcollection.removeTaus();

    // sort leptons and jets by pt
    lepcollection.sortByPt();
    jetcollection.sortByPt();

    // yield (fixed value of 0.5)
    varmap["_yield"] = 0.5;

    // number of muons and electrons
    varmap["_nMuons"] = lepcollection.numberOfMuons();
    varmap["_nElectrons"] = lepcollection.numberOfElectrons();

    // number of jets and b-jets
    varmap["_nJets"] = jetcollection.numberOfJets();
    varmap["_nBJets"] = jetcollection.numberOfBJets();

    // now return the varmap (e.g. to fill histograms)
    return varmap;
}

std::pair<double,double> eventFlatteningParticleLevel::pmzcandidates(
    const LeptonParticleLevel& lep,
    const MetParticleLevel& met){
    // define lorentz vectors for lepton and met
    LorentzVector lvec;
    lvec.setPxPyPzE(lep.px(), lep.py(), lep.pz(), lep.energy());
    LorentzVector metvec;
    metvec.setPxPyPzE(met.px(), met.py(), 0., std::sqrt(met.px()*met.px()+met.py()*met.py()));
    // call underlying function
    return eventFlattening::pmzcandidates(lvec, metvec);
}

std::pair<double,int> eventFlatteningParticleLevel::besttopcandidate(
    const JetParticleLevelCollection& alljets,
    const LeptonParticleLevel& lep,
    const MetParticleLevel& met,
    double pmz1, double pmz2){
    // make lorentz vectors for lepton and met
    LorentzVector lvec;
    lvec.setPxPyPzE(lep.px(), lep.py(), lep.pz(), lep.energy());
    LorentzVector metvec;
    metvec.setPxPyPzE(met.px(), met.py(), 0, std::sqrt(met.px()*met.px()+met.py()*met.py()));
    // make lorentz vectors for jets
    // note: only b-jets should be taken into account,
    //       so we also need to keep a mapping from new indices (only b-jets)
    //       to old indices (all jets)
    std::vector<LorentzVector> jets;
    std::map<int, int> idxMap;
    int counter = 0;
    for(JetParticleLevelCollection::const_iterator jIt = alljets.cbegin();
	jIt != alljets.cend(); jIt++){
        JetParticleLevel& jetobject = **jIt;
        // consider only b-jets
        if(!jetobject.isBJet()) continue;
        // make a lorentz vector and add it
        LorentzVector jet;
        jet.setPxPyPzE(jetobject.px(),jetobject.py(),jetobject.pz(),jetobject.energy());
        jets.push_back(jet);
        idxMap[counter] = jIt - alljets.cbegin();
        counter++;
    }
    // call underlying function
    std::pair<double,int> res = eventFlattening::besttopcandidate(jets, lvec, metvec, pmz1, pmz2);
    // correct the index
    res.second = idxMap[res.second];
    return res;
}
