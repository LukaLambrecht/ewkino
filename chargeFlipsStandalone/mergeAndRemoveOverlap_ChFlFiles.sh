!/bin/bash

echo "Start job!"
cd /storage_mnt/storage/user/dmarckx/CMSSW_10_6_28/src/
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
export HOME=/storage_mnt/storage/user/dmarckx/
cd /storage_mnt/storage/user/dmarckx/


#INPUTFILE=`python <<EOF
#print '/pnfs/iihe/cms/store/user/joknolle/higgstuples/ggh_zupsilon_1s2s3s_realistic_Run2SIM_UL2018MiniAOD_v3/220301_114658/file{}.root'.format($1)
#EOF`

#OUTPUTFILE=`python <<EOF
#print '/user/dmarckx/CMSSW_10_6_28/src/Configuration/GenProduction/H_upsilonresearch/signal{}.root'.format($1)
#EOF`

./ewkino/chargeFlipsStandalone/mergeAndRemoveOverlap /pnfs/iihe/cms/store/user/llambrec/dileptonskim_ttw_chargeflips/data/2018/DoubleMuon_Summer20UL18.root-/pnfs/iihe/cms/store/user/llambrec/dileptonskim_ttw_chargeflips/data/2018/EGamma_Summer20UL18.root-/pnfs/iihe/cms/store/user/llambrec/dileptonskim_ttw_chargeflips/data/2018/MuonEG_Summer20UL18.root-/pnfs/iihe/cms/store/user/llambrec/dileptonskim_ttw_chargeflips/data/2018/SingleMuon_Summer20UL18.root /pnfs/iihe/cms/store/user/dmarckx/merged_noOverlap_2018.root false
echo "Job done!"
