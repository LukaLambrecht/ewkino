import os
import sys
import argparse


if __name__=='__main__':

  # parse arguments
  parser = argparse.ArgumentParser('Drop BDT score from evtid file')
  parser.add_argument('-i', '--inputfile', required=True, type=os.path.abspath)
  parser.add_argument('-o', '--outputfile', required=True, type=os.path.abspath)
  parser.add_argument('--bdtcut', default=None, type=float)
  args = parser.parse_args()

  # print arguments
  print('Running with following configuration:')
  for arg in vars(args):
    print('  - {}: {}'.format(arg,getattr(args,arg)))

  # read input files
  lines = []
  with open(inputfile, 'r') as f:
    lines = f.readlines()
    # loop over lines
    for i, line in enumerate(lines):
      line = line.strip(' \t\n')
      parts = line.split(':')
      if len(parts)!=4:
        msg = 'ERROR: unparseable line in {} (line {})'.format(inputfile, i+1)
        raise Exception(msg)
      entry = (parts[0], parts[1], parts[2])
      res.append(entry)

  # write output file
  with open(args.outputfile, 'w') as f:
    for entry in res:
      line = ':'.join(entry)
      f.write(line+'\n')
