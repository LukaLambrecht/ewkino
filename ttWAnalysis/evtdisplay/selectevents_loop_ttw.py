###########################################################
# Submit select events for a number of different datasets #
###########################################################

import os
import sys

# define regions
regions = []
#for r in ['signalregion_dilepton_inclusive']: regions.append(r)
for r in ['signalregion_dilepton_mm']: regions.append(r)
#for r in ['signalregion_trilepton']: regions.append(r)

# define years
years = ['2018']

# define selection types
selection_types = []
selection_types.append('tight')

# define sample lists
samplelistdir = 'samplelists/'
samplelistbase = 'samplelist_data_{}_perpd_perera.txt'

# define BDT file
bdtfile = '../bdtweights/v20230601/XGBrobustnessv3_all.root'
bdtcut = 0.75

# define output directory
datetag = 'test'
outputdir = 'output_{}'.format(datetag)

# define number of events
nevents = 0
runmode = 'condor'

# loop over years
for year in years:
      
    # set correct input directory
    inputdir = '/pnfs/iihe/cms/store/user/nivanden/skims_v5'
    inputdiryear = year
    if( year=='2016PreVFP' or year=='2016PostVFP' ): inputdiryear = '2016'
    inputdir = os.path.join(inputdir, inputdiryear)

    # set correct sample list
    samplelist = os.path.join(samplelistdir, samplelistbase.format(year))

    # loop over regions and selection types
    for region in regions:
        for selection_type in selection_types:

            # set correct output directory
            thisoutputdir = os.path.join(outputdir, '{}_{}_{}'.format(year, region, selection_type))
    
            # make and run the command
            cmd = 'python selectevents.py'
            cmd += ' --inputdir ' + inputdir
            cmd += ' --outputdir ' + thisoutputdir
            cmd += ' --samplelist ' + samplelist
            cmd += ' --runmode ' + runmode
            if nevents!=0: cmd += ' --nevents {}'.format(int(nevents))
            if bdtfile is not None: cmd += ' --bdt ' + bdtfile
            if bdtcut is not None: cmd += ' --bdtcut {}'.format(bdtcut)
            cmd += ' --event_selection ' + region
            cmd += ' --selection_type ' + selection_type
            print('executing ' + cmd)
            os.system(cmd)
