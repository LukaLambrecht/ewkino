# Selecting interesting events for event displays

The functionality in this folder selects interesting events in data.
Interesting events are defined as:
- Passing the event selection.
- High BDT values.
The output is a list of event ids (run / lumiblock/ event number).
The further processing should be done in CMSSW with the iSpy tool.

# Some documentation for further processing with the iSpy tool

### Get the selected events in AOD / MiniAOD format.

For this step, the `edmPickEvents` utility (part of CMSSW) can be used.
See here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents.

The prerequisites are:
- A sufficiently recent CMSSW version (exact version should not matter, I worked in 10-6-30, for Run II UL).
- A txt file as described above.
- The name of the dataset (on DAS) to which the events belong.

Use the tool as follows: `edmPickEvents.py <name of dataset> <name of txt file>`. Add the argument `--runInteractive` to actually run the event getting (instead of simply printing out the correct file).

Note: dataset `/DoubleMuon/Run2018D-12Nov2019_UL2018-v3/AOD` is not available on disk anymore, only on tape, so no choice but to use MiniAOD version.

### Produce an .ig file from the AOD / MiniAOD events.

See here: https://github.com/cms-outreach/ispy-analyzers.
Follow the instructions, seem to run without issues.
Output file is `igOutput_0.ig`.

Note: the instructions seem to b implicitly for AOD only. For miniAOD, use the cfg file `ispy_miniAOD.py` instead of `ispy_10_X_X.py`. The output file is `miniAOD_0.ig` instead of `igOutput_0.ig`.

### Display

Upload the ig file here: http://cern.ch/ispy-webgl-dev
(or here: http://ispy-webgl-dev.web.cern.ch/, points to the same webpage).
