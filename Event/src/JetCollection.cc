#include "../interface/JetCollection.h"


//include other parts of framework
#include "../interface/LeptonCollection.h"


JetCollection::JetCollection( const TreeReader& treeReader ){
    for( unsigned j = 0; j < treeReader._nJets; ++j ){
        this->push_back( Jet( treeReader, j ) ); 
    }
}


void JetCollection::selectGoodJets(){
    selectObjects( &Jet::isGood );
}


void JetCollection::selectGoodAnyVariationJets(){
    selectObjects( &Jet::isGoodAnyVariation );
}


void JetCollection::cleanJetsFromLeptons( const LeptonCollection& leptonCollection, bool (Lepton::*passSelection)() const, const double coneSize ){
    for( const_iterator jetIt = cbegin(); jetIt != cend(); ){
        Jet& jet = **jetIt;

        //increment iterator if jet is not deleted 
        bool isDeleted = false;
        for( LeptonCollection::const_iterator lIt = leptonCollection.cbegin(); lIt != leptonCollection.cend(); ++lIt ){
            Lepton& lepton = **lIt;

            //lepton must pass specified selection
            if( !(lepton.*passSelection)() ) continue;

            //remove jet if it overlaps with a selected lepton
            if( deltaR( jet, lepton ) < coneSize ){

                jetIt = erase( jetIt );
                isDeleted = true;
                break;
            }
        }
        if( !isDeleted ){
            ++jetIt;
        }
    }
}


JetCollection JetCollection::buildSubCollection( bool (Jet::*passSelection)() const ) const{
    std::vector< std::shared_ptr< Jet > > jetVector;
    for( const auto& jetPtr : *this ){
        if( ( *jetPtr.*passSelection )() ){

            //jets are shared between collections!
            jetVector.push_back( jetPtr );
        }
    }
    return JetCollection( jetVector );
}


JetCollection JetCollection::goodJetCollection() const{
    return buildSubCollection( &Jet::isGood );
}


JetCollection JetCollection::goodAnyVariationJetCollection() const{
    return buildSubCollection( &Jet::isGoodAnyVariation );
}


JetCollection JetCollection::looseBTagCollection() const{
    return buildSubCollection( &Jet::isBTaggedLoose );
}


JetCollection JetCollection::mediumBTagCollection() const{
    return buildSubCollection( &Jet::isBTaggedMedium );
}


JetCollection JetCollection::tightBTagCollection() const{
    return buildSubCollection( &Jet::isBTaggedTight );
}


JetCollection JetCollection::buildVariedCollection( Jet (Jet::*variedJet)() const ) const{
    std::vector< std::shared_ptr< Jet > > jetVector;
    for( const auto& jetPtr : *this ){

        //jets are NOT shared between collections!
        jetVector.push_back( std::make_shared< Jet >( (*jetPtr.*variedJet)() ) );
    }
    return JetCollection( jetVector );
}


JetCollection JetCollection::JECDownCollection() const{
    return buildVariedCollection( &Jet::JetJECDown );
}


JetCollection JetCollection::JECUpCollection() const{
    return buildVariedCollection( &Jet::JetJECUp );
}


JetCollection JetCollection::JERDownCollection() const{
    return buildVariedCollection( &Jet::JetJERDown );
}


JetCollection JetCollection::JERUpCollection() const{
    return buildVariedCollection( &Jet::JetJERUp );
}


JetCollection::size_type JetCollection::numberOfLooseBTaggedJets() const{
    return count( &Jet::isBTaggedLoose );
}


JetCollection::size_type JetCollection::numberOfMediumBTaggedJets() const{
    return count( &Jet::isBTaggedMedium );
}


JetCollection::size_type JetCollection::numberOfTightBTaggedJets() const{
    return count( &Jet::isBTaggedTight );
}


JetCollection::size_type JetCollection::numberOfGoodJets() const{
    return count( &Jet::isGood );
}


JetCollection::size_type JetCollection::numberOfGoodAnyVariationJets() const{
    return count( &Jet::isGoodAnyVariation );
}


void JetCollection::cleanJetsFromLooseLeptons( const LeptonCollection& leptonCollection, const double coneSize ){
    cleanJetsFromLeptons( leptonCollection, &Lepton::isLoose, coneSize );
} 


void JetCollection::cleanJetsFromFOLeptons( const LeptonCollection& leptonCollection, const double coneSize ){
    cleanJetsFromLeptons( leptonCollection, &Lepton::isFO, coneSize );
} 


void JetCollection::cleanJetsFromTightLeptons( const LeptonCollection& leptonCollection, const double coneSize ){
    cleanJetsFromLeptons( leptonCollection, &Lepton::isTight, coneSize );
}
