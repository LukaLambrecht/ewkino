#include "../interface/Electron.h"

//include other parts of framework
#include "../interface/ElectronSelector.h"


Electron::Electron( const TreeReader& treeReader, const unsigned leptonIndex ):
    LightLepton( treeReader, leptonIndex, new ElectronSelector( this ) ),
    _passChargeConsistency( treeReader._lElectronChargeConst[leptonIndex] ),
    _passDoubleEGEmulation( treeReader._lElectronPassEmu[leptonIndex] ),
    _passConversionVeto( treeReader._lElectronPassConvVeto[leptonIndex] ),
    _numberOfMissingHits( treeReader._lElectronMissingHits[leptonIndex] ),
    _electronMVASummer16GP( treeReader._lElectronSummer16MvaGP[leptonIndex] ),
    _electronMVASummer16HZZ( treeReader._lElectronSummer16MvaHZZ[leptonIndex] ),
    _electronMVAFall17Iso( treeReader._lElectronMvaFall17Iso[leptonIndex] ),
    _electronMVAFall17NoIso( treeReader._lElectronMvaFall17NoIso[leptonIndex] ),
    _etaSuperCluster( treeReader._lEtaSC[leptonIndex] ),
    _isVetoPOGElectron( treeReader._lPOGVeto[leptonIndex] ),
    _isLoosePOGElectron( treeReader._lPOGLoose[leptonIndex] ),
    _isMediumPOGElectron( treeReader._lPOGMedium[leptonIndex] ),
    _isTightPOGElectron( treeReader._lPOGTight[leptonIndex] )
{
    
    //apply electron energy correction
    setLorentzVector( treeReader._lPtCorr[ leptonIndex ], eta(), phi(), treeReader._lECorr[ leptonIndex ] );

    //make sure also non-cone-corrected pt is set to the corrected value
    _uncorrectedPt = pt();
}


Electron::Electron( const Electron& rhs ) :
	LightLepton( rhs, new ElectronSelector( this ) ),
	_passChargeConsistency( rhs._passChargeConsistency ),
	_passDoubleEGEmulation( rhs._passDoubleEGEmulation ),
	_passConversionVeto( rhs._passConversionVeto ),
	_numberOfMissingHits( rhs._numberOfMissingHits ),
	_electronMVASummer16GP( rhs._electronMVASummer16GP ),
	_electronMVASummer16HZZ( rhs._electronMVASummer16HZZ ),
	_electronMVAFall17Iso( rhs._electronMVAFall17Iso ),
	_electronMVAFall17NoIso( rhs._electronMVAFall17NoIso ),
	_etaSuperCluster( rhs._etaSuperCluster ),
	_isVetoPOGElectron( rhs._isVetoPOGElectron ),
	_isLoosePOGElectron( rhs._isLoosePOGElectron ),
	_isMediumPOGElectron( rhs._isMediumPOGElectron ),
	_isTightPOGElectron( rhs._isTightPOGElectron )
	{}


Electron::Electron( Electron&& rhs ) noexcept : 
	LightLepton( std::move( rhs ), new ElectronSelector( this ) ),
	_passChargeConsistency( rhs._passChargeConsistency ),
    _passDoubleEGEmulation( rhs._passDoubleEGEmulation ),
    _passConversionVeto( rhs._passConversionVeto ),
    _numberOfMissingHits( rhs._numberOfMissingHits ),
    _electronMVASummer16GP( rhs._electronMVASummer16GP ),
    _electronMVASummer16HZZ( rhs._electronMVASummer16HZZ ),
    _electronMVAFall17Iso( rhs._electronMVAFall17Iso ),
    _electronMVAFall17NoIso( rhs._electronMVAFall17NoIso ),
    _etaSuperCluster( rhs._etaSuperCluster ),
    _isVetoPOGElectron( rhs._isVetoPOGElectron ),
    _isLoosePOGElectron( rhs._isLoosePOGElectron ),
    _isMediumPOGElectron( rhs._isMediumPOGElectron ),
    _isTightPOGElectron( rhs._isTightPOGElectron )
    {}


std::ostream& Electron::print( std::ostream& os ) const{
    os << "Electron : ";
    LightLepton::print( os );
    os << " / passChargeConsistency = " << _passChargeConsistency << " / passDoubleEGEmulation = " << _passDoubleEGEmulation << " / passConversionVeto = " << _passConversionVeto <<
        " / numberOfMissingHits = " << _numberOfMissingHits << " / electronMVASummer16GP = " << _electronMVASummer16GP << " / electronMVASummer16HZZ = " << _electronMVASummer16HZZ <<
        " / electronMVAFall17Iso = " << _electronMVAFall17Iso << " / electronMVAFall17NoIso = " << _electronMVAFall17NoIso << " / etaSuperCluster = " << _etaSuperCluster;
    if( _isTightPOGElectron ){
        os << " / tight POG electron";
    } else if( _isMediumPOGElectron ){
        os << " / medium POG electron";
    } else if( _isLoosePOGElectron ){
        os << " / loose POG electron";
    } else if( _isVetoPOGElectron ){
        os << " / veto POG electron";
    } else{
        os << " / fails POG electron selection";
    }
    return os;    
}
