// include header 
#include "../interface/eventSelectionsParticleLevel.h"

//include c++ library classes
#include <functional>

bool eventSelectionsParticleLevel::passES(Event& event, const std::string& eventselection ){
    // arguments:
    // - object of type Event
    // - event selection identifier, see map below for allowd values

    // map event selection to function
    static std::map< std::string, std::function< bool(Event&) > > 
	    ESFunctionMap = {
		// no selection
		{ "noselection", pass_noselection },
		// signal regions
		{ "signalregion_trilepton", pass_signalregion_trilepton },
	    };
    auto it = ESFunctionMap.find( eventselection );
    if( it == ESFunctionMap.cend() ){
	std::string msg = "ERROR in eventSelectionParticleLevel.cc / passES:";
	msg += " unknown event selection " + eventselection;
        throw std::invalid_argument( msg );
    }
    return (it->second)(event);
}

// help functions for event cleaning //

constexpr double halfwindow = 15;

void eventSelectionsParticleLevel::cleanLeptonsAndJets(Event& event){
    // do lepton selection and cleaning
    event.selectGoodParticleLevelLeptons();
    event.leptonParticleLevelCollection().removeTaus();
    // do jet selection and cleaning
    event.selectGoodParticleLevelJets();
    event.cleanParticleLevelJetsFromLeptons();
    // sort leptons and jets by pt
    event.leptonParticleLevelCollection().sortByPt();
    event.jetParticleLevelCollection().sortByPt();
}


// help functions for trigger and pt-threshold selections //

bool eventSelectionsParticleLevel::passTriLeptonPtThresholds(const Event& event){
    event.leptonParticleLevelCollection().sortByPt();
    if(event.leptonParticleLevelCollection()[0].pt() < 25.
	|| event.leptonParticleLevelCollection()[1].pt() < 15.
        || event.leptonParticleLevelCollection()[2].pt() < 10.) return false;
    return true; 
}

// help functions for determining number of jets and b-jets //

std::pair<int,int> eventSelectionsParticleLevel::nJetsNBJets(const Event& event){
    // determine the number of jets and b-jets
    int njets = event.jetParticleLevelCollection().numberOfJets();
    int nbjets = event.jetParticleLevelCollection().numberOfBJets();
    return std::make_pair(njets,nbjets);
}

// help function for lepton pair mass constraint //

bool eventSelectionsParticleLevel::passMllMassVeto( const Event& event ){
    for( LeptonParticleLevelCollection::const_iterator l1It = event.leptonParticleLevelCollection().cbegin(); 
	l1It != event.leptonParticleLevelCollection().cend(); l1It++ ){
	for( LeptonParticleLevelCollection::const_iterator l2It = l1It+1; 
	    l2It != event.leptonParticleLevelCollection().cend(); l2It++ ){
            LeptonParticleLevel& lep1 = **l1It;
            LeptonParticleLevel& lep2 = **l2It;
            if( LeptonParticleLevel::sameFlavor(lep1,lep2) && (lep1+lep2).mass() < 12. ) return false;
	}
    }
    return true;
}


// dedicated functions to check if event passes certain conditions //

// -------------
// no selection 
// -------------

bool eventSelectionsParticleLevel::pass_noselection(Event& event){
    cleanLeptonsAndJets(event);
    return true;
}


// ---------------
// signal regions 
// ---------------

bool eventSelectionsParticleLevel::pass_signalregion_trilepton(Event& event){
    // signal region with three leptons and Z veto
    cleanLeptonsAndJets(event);
    LeptonParticleLevelCollection lepcollection = event.leptonParticleLevelCollection();
    // basic requirements
    if( lepcollection.numberOfLeptons()!=3 ) return false;
    if( !passTriLeptonPtThresholds(event) ) return false;
    // Z candidate
    if( !lepcollection.hasOppositeSignSameFlavorPair() ) return false;
    if( !lepcollection.hasZTollCandidate(halfwindow) ) return false;
    // number of jets and b-jets
    std::pair<int,int> njetsnbjets = nJetsNBJets(event);
    if( njetsnbjets.second < 1 ) return false;
    if( njetsnbjets.first < 2 ) return false;
    return true; 
}

std::tuple<int,std::string> eventSelectionsParticleLevel::pass_signalregion_trilepton_cutflow(
    Event& event){
    // copy of pass_signalregion_trilepton but with different return type
    // to allow cutflow studies
    return std::make_tuple(-1, "To do");
}
