import os
import sys
import argparse


if __name__=='__main__':

  # parse arguments
  parser = argparse.ArgumentParser('Merge evtid files')
  parser.add_argument('-i', '--inputfiles', required=True, type=os.path.abspath, nargs='+')
  parser.add_argument('--bdtcut', default=None, type=float)
  args = parser.parse_args()

  # print arguments
  print('Running with following configuration:')
  for arg in vars(args):
    print('  - {}: {}'.format(arg,getattr(args,arg)))

  # loop over input files
  for inputfile in sorted(args.inputfiles):
    with open(inputfile, 'r') as f:
      lines = f.readlines()
    res = []
    # loop over lines
    for i, line in enumerate(lines):
      line = line.strip(' \t\n')
      parts = line.split(':')
      if len(parts)!=4:
        msg = 'ERROR: unparseable line in {} (line {})'.format(inputfile, i+1)
        raise Exception(msg)
      entry = (parts[0], parts[1], parts[2], parts[3])
      # impose extra bdt threshold
      if args.bdtcut is not None and float(entry[3])<args.bdtcut: continue
      # else add to result
      res.append(entry)
    # print results
    print('Events for file {} ({}):'.format(inputfile, len(res)))
    for entry in res: print('- {}:{}:{} ({:.3f})'.format(entry[0], entry[1], entry[2], float(entry[3])))
