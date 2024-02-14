#ifndef eventFlatteningParticleLevel_H
#define eventFlatteningParticleLevel_H

// include other parts of framework
#include "../../../constants/particleMasses.h"
#include "../../../Event/interface/Event.h"

// include other parts of analysis code
#include "eventFlattening.h"

// function declarations

namespace eventFlatteningParticleLevel{
    std::map< std::string, double > initVarMap();
    std::map< std::string, double > eventToEntry(Event& event);

    std::pair<double,double> pmzcandidates(
	const LeptonParticleLevel& lep,
	const MetParticleLevel& met);
    std::pair<double,int> besttopcandidate(
	const JetParticleLevelCollection& alljets,
	const LeptonParticleLevel& lep,
	const MetParticleLevel& met,
	double pmz1, double pmz2);
}

#endif
