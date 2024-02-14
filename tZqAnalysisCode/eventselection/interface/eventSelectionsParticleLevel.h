#ifndef eventSelectionsParticleLevel_H
#define eventSelectionsParticleLevel_H

// include other parts of framework
#include "../../../Event/interface/Event.h"
#include "../../../constants/particleMasses.h"

namespace eventSelectionsParticleLevel{
    bool passES(Event&, const std::string& eventselection);
    // help functions
    void cleanLeptonsAndJets(Event&);
    bool passTriLeptonPtThresholds(const Event& event);
    std::pair<int,int> nJetsNBJets(const Event& event);
    bool passMllMassVeto(const Event& event);
    // no selection
    bool pass_noselection(Event& event);
    // signal regions
    bool pass_signalregion_trilepton(Event& event);
    // cutflow functions
    std::tuple<int,std::string> pass_signalregion_trilepton_cutflow(Event& event);
}

#endif
