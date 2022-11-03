#include "../interface/chargeFlipTools.h"

//include other parts of framework
#include "../../Tools/interface/histogramTools.h"


double chargeFlips::chargeFlipWeight( const Event& event, const std::shared_ptr< TH2 >& chargeFlipMap ){

    // P( A + B ) = P( A ) + P( B ) - P( A & B )
    double summedProbabilities = 0.;
    double multipliedProbabilities = 1.;
    for( const auto& electronPtr : event.electronCollection() ){

		double flipRate = histogram::contentAtValues( chargeFlipMap.get(), electronPtr->pt(), electronPtr->absEta() );
        summedProbabilities += flipRate / ( 1. - flipRate );
        multipliedProbabilities *= flipRate / ( 1. - flipRate );
    }
    return ( summedProbabilities - multipliedProbabilities );
}




double chargeFlips::chargeFlipWeight( const int& flavorl1, const int& flavorl2, const double& ptl1, const double& ptl2, const double& etal1, const double& etal2,  const bool& electron, const std::shared_ptr< TH2 >& chargeFlipMap ){

    // P( A + B ) = P( A ) + P( B ) - P( A & B )
    double summedProbabilities = 0.;
    double multipliedProbabilities = 1.;
    
    std::vector<int> needed_leptons;
    if(electron){ 
        if(flavorl1 == 0){    
            double flipRate = histogram::contentAtValues( chargeFlipMap.get(), ptl1, fabs(etal1) );
            summedProbabilities += flipRate / ( 1. - flipRate );
            multipliedProbabilities *= flipRate / ( 1. - flipRate );
         }
         if(flavorl2 == 0){
            double flipRate = histogram::contentAtValues( chargeFlipMap.get(), ptl2, fabs(etal2) );
            summedProbabilities += flipRate / ( 1. - flipRate );
            multipliedProbabilities *= flipRate / ( 1. - flipRate );
         }
    }
    else{
        if(flavorl1 == 1){
            double flipRate = histogram::contentAtValues( chargeFlipMap.get(), ptl1, fabs(etal1) );
            summedProbabilities += flipRate / ( 1. - flipRate );
            multipliedProbabilities *= flipRate / ( 1. - flipRate );
         }
         if(flavorl2 == 1){
            double flipRate = histogram::contentAtValues( chargeFlipMap.get(), ptl2, fabs(etal2) );
            summedProbabilities += flipRate / ( 1. - flipRate );
            multipliedProbabilities *= flipRate / ( 1. - flipRate );
         }
    }
    
    return ( summedProbabilities - multipliedProbabilities );
    }         
