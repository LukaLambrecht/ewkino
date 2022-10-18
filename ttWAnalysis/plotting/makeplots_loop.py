########################################################################################
# Simple submitter that runs makeplots.py for a number of predefined regions and years #
########################################################################################

import sys
import os
sys.path.append('../../jobSubmission')
import condorTools as ct
from jobSettings import CMSSW_VERSION

inputdir = sys.argv[1]
runmode = 'condor'

regions = []
for r in ['signalregion_trilepton', 'signalregion_dilepton']: regions.append(r)
for r in ['wzcontrolregion','zzcontrolregion','zgcontrolregion']: regions.append(r)
#for r in ['nonprompt_trilepton_noossf','nonprompt_trilepton_noz']: regions.append(r)
for r in ['controlregion_trilepton','4lepton_controlregion','nonprompt_dilepton_invMET']: regions.append(r)#, 'nonprompt_dilepton_invMET'
for r in ['nonprompt_dilepton_invMET_mm','nonprompt_dilepton_invMET_me','nonprompt_dilepton_invMET_em','nonprompt_dilepton_invMET_ee',]: regions.append(r)
for r in ['signalregion_dimuon','signalregion_dielectron','signalregion_dilepton_em','signalregion_dilepton_me']: regions.append(r)
for r in ['nonprompt_trilepton']: regions.append(r)
for r in ['nonprompt_dilepton']: regions.append(r)

years = ['2016PreVFP', '2016PostVFP', '2017', '2018']

npmodes = ['npfromsim', 'npfromdata']

variables = '../variables/variables_copyfromtzq.json'

colormap = 'tttt'

cmds = []
for year in years:
  for npmode in npmodes:
    subdir = os.path.join(year, 'merged_'+npmode)
    inputfile = os.path.join(inputdir, subdir, 'merged.root')
    if not os.path.exists(inputfile):
      print('WARNING: input file {} does not exist; continuing...'.format(inputfile))
      continue
    for region in regions:
      thisoutputdir = os.path.join('/user/dmarckx/public_html/14_10/', subdir, 'plots', year+'_'+region+'_'+npmode)#inputdir
      #os.system("mkdir " + thisoutputdir)
      #os.system("cp /user/dmarckx/public_html/fast/index.php " + thisoutputdir + "index.php")
      unblind = True
      if 'signalregion' in region: unblind = False
      cmd = 'python makeplots.py'
      cmd += ' --inputfile '+inputfile
      cmd += ' --year '+year
      cmd += ' --region '+region
      cmd += ' --variables '+variables
      cmd += ' --outputdir '+thisoutputdir
      cmd += ' --unblind {}'.format(unblind)
      cmd += ' --colormap '+colormap
      if runmode=='local':
        print('executing '+cmd)
        os.system(cmd)
      elif runmode=='condor':
        print('submitting '+cmd)
        cmds.append(cmd)
      else: raise Exception('ERROR: runmode "{}" not recognized'.format(runmode))

if runmode=='condor':
  ct.submitCommandsAsCondorCluster('cjob_makeplots', cmds,
                                    cmssw_version=CMSSW_VERSION)
  for year in years:
    for npmode in npmodes:
      subdir = os.path.join(year, 'merged_'+npmode)
      for region in regions:
        thisoutputdir = os.path.join('/user/dmarckx/public_html/14_10/', subdir, 'plots', year+'_'+region+'_'+npmode)#inputdir
        os.system("mkdir -p " + thisoutputdir)
        os.system("cp /user/dmarckx/public_html/fast/index.php " + thisoutputdir + "/index.php")
