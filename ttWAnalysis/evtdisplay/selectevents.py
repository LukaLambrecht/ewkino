##########################################################
# python script to run selectevents.cc in job submission #
##########################################################

import os
import sys
import argparse
sys.path.append(os.path.abspath('../../jobSubmission'))
import condorTools as ct
CMSSW_VERSION = '~/CMSSW_12_4_6' # newer version needed for BDT evaluation
sys.path.append(os.path.abspath('../../Tools/python'))
import argparsetools as apt
from samplelisttools import readsamplelist
sys.path.append(os.path.abspath('../eventselection'))
from eventselector import event_selections, selection_types
from eventflattener import year_from_samplelist


if __name__=='__main__':

  # parse arguments
  parser = argparse.ArgumentParser('Run select events')
  parser.add_argument('-i', '--inputdir', required=True, type=os.path.abspath)
  parser.add_argument('-s', '--samplelist', required=True, type=os.path.abspath)
  parser.add_argument('-o', '--outputdir', required=True, type=os.path.abspath)
  parser.add_argument('--event_selection', required=True, choices=event_selections)
  parser.add_argument('--selection_type', default='tight', choices=selection_types)
  parser.add_argument('--bdt', default=None, type=apt.path_or_none)
  parser.add_argument('--bdtcut', default=None, type=float)
  parser.add_argument('-n', '--nevents', default=0, type=int)
  parser.add_argument('--runmode', default='condor', choices=['condor','local'])
  args = parser.parse_args()

  # print arguments
  print('Running with following configuration:')
  for arg in vars(args):
    print('  - {}: {}'.format(arg,getattr(args,arg)))
 
  # argument checks and parsing
  if not os.path.exists(args.inputdir):
    raise Exception('ERROR: input directory {} does not exist.'.format(args.inputdir))
  if not os.path.exists(args.samplelist):
    raise Exception('ERROR: sample list {} does not exist.'.format(args.samplelist))
  if os.path.exists(args.outputdir):
      print('WARNING: output directory {} already exists. Clean it? (y/n)'.format(args.outputdir))
      go=raw_input()
      if not go=='y': sys.exit()
      os.system('rm -r '+args.outputdir)

  # check if executable is present
  exe = './selectevents'
  if not os.path.exists(exe):
    raise Exception('ERROR: {} executable was not found.'.format(exe))

  # make output directory
  if not os.path.exists(args.outputdir): os.makedirs(args.outputdir)

  # check samples
  samples = readsamplelist( args.samplelist, sampledir=args.inputdir )
  nsamples = samples.number()
  print('Found {} samples.'.format(nsamples))
  print('Full list of samples:')
  print(samples)

  # check bdt weight file
  bdt = 'none'
  bdtcut = -99
  if( args.bdt is not None ):
    if not os.path.exists(args.bdt):
      raise Exception('ERROR: BDT file {} does not exist'.format(args.bdt))
    bdt = args.bdt
  if( args.bdtcut is not None ):
    bdtcut = args.bdtcut
  if( args.bdt is None and args.bdtcut is not None ):
    msg = 'ERROR: you have specified a BDT cut value but no BDT weights file.'
    raise Exception(msg)

  # loop over input files and submit jobs
  commands = []
  for i, sample in enumerate(samples.samples):
    # make output file name
    outputfilename = sample.name.replace('.root', '_evtids.txt')
    outputfile = os.path.join(args.outputdir, outputfilename)
    # make the basic command
    command = exe + ' {} {} {} {} {} {} {}'.format(
                    sample.path, outputfile, args.nevents,
                    args.event_selection, args.selection_type,
                    bdt, bdtcut )
    commands.append(command)

  # submit the jobs
  if args.runmode=='local':
    for command in commands:
      print(command)
      os.system(command)
  elif args.runmode=='condor':
    ct.submitCommandsAsCondorCluster( 'cjob_runanalysis', commands,
                                      cmssw_version=CMSSW_VERSION ) 
