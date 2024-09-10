##############################################################
# Utility script for synchronizing postfit plots with Oviedo #
##############################################################
# Copied from /afs/cern.ch/work/s/sesanche/public/forDavid/postfit_plots.py
# on Sep 10, 2024

# However, this script does not seem to be directly useful, for the following reasons:
# - many custom and hard-coded naming conventions.
# - uses only shapes from the FitDiagnostics output file;
#   this does not allow to plot different distributions than the one used in the fit.

import ROOT as r
import ROOT
import cmsstyle as CMS
import math

r.gROOT.SetBatch(True)
fit="prefit" # "fit_s"

proc_list=reversed([
    "t#bar{t}W (#Delta y^{l}>0)"  ,
    "t#bar{t}W (#Delta y^{l}<0)"  ,
    "t#bar{t}W (other)"           ,
    "Rares"                       ,
    "Conversions"                 ,
    "t#bar{t}Z"                   ,
    "Diboson"                     ,
    "Non-prompt"                  ,
])

    


procs={
    "t#bar{t}W (#Delta y^{l}>0)"  : ["TTW_asymmetry_positive"],
    "t#bar{t}W (#Delta y^{l}<0)"  : ["TTW_asymmetry_negative"],
    "t#bar{t}W (other)"           : ["TTW_ooa"],
    "Rares"                       : ["tHW", "tHq", "ttH", "ttVV", "Rares", "VVV", 'tZq'],
    "Conversions"                 : ["Convs"],
    "t#bar{t}Z"                   : ["TTZ"],
    "Diboson"                     : ["WZ",  "ZZ"],
    "Non-prompt"                  : [ "data_fakes"],

}
histos={}
total_err=None
thedata=None
tf=r.TFile.Open("fitDiagnostics.Test.root")
for ch in "ch25,ch26,ch27,ch28".split(","):
    for subproc in tf.Get("shapes_%s/%s"%(fit,ch)).GetListOfKeys():
        found=False
        for proc in procs:
            if subproc.GetName() in procs[proc]:
                found=True
                if proc in histos:
                    histos[proc].Add( subproc.ReadObj() )
                else:
                    histos[proc]=subproc.ReadObj().Clone(proc)
                break
        if "total" == subproc.GetName():
            if total_err is None:
                total_err=subproc.ReadObj().Clone("total_err")
            else:
                total_err.Add( subproc.ReadObj() ) 
        if not found:
            print("Warning, couldnt map", subproc.GetName())

data_histo=histos["Diboson"].Clone("data")
data_histo.Reset()
for ch in "ch25,ch26,ch27,ch28".split(","):
    subdata=tf.Get("shapes_%s/%s/data"%(fit,ch))
    for i in range(data_histo.GetNbinsX()):
        data_histo.SetBinContent(i+1, subdata.GetPointY(i)+data_histo.GetBinContent(i+1))

square=CMS.kSquare
iPos=0

dicanv = CMS.cmsDiCanvas("sanem", 0, 32, 0, 80, 0.5, 1.5, "Category", "Events", "Data/MC", square=square, extraSpace=0.1, iPos=iPos,)
            
dicanv.cd(1)
stack=r.THStack()

leg = CMS.cmsLeg(0.21, 0.7 - 0.07 * 3, 0.89, 0.89, textSize=0.05)
leg.AddEntry(data_histo, "Data", "pe")
leg.SetNColumns(3)
CMS.cmsDrawStack(stack, leg, dict( [(x,histos[x]) for x in proc_list]))
CMS.cmsDraw(total_err, "e2same0", lcolor = 335, lwidth = 1, msize = 0, fcolor = ROOT.kBlack, fstyle = 3004,)
for i in range(data_histo.GetNbinsX()):
    data_histo.SetBinError(i+1, math.sqrt(data_histo.GetBinContent(i+1)))
CMS.cmsDraw(data_histo, "P,E", mcolor=ROOT.kBlack)


dicanv.cd(2)
ratio_den=stack.GetStack().Last()
for i in range(ratio_den.GetNbinsX()):
    ratio_den.SetBinError(i+1,0)
ratio=data_histo.Clone("ratio")
ratio.Divide(ratio_den)
ratio_err=total_err.Clone("ratio_err")
ratio_err.Divide(ratio_den)

CMS.cmsDraw(ratio_err, "e2same0", lcolor = 335, lwidth = 1, msize = 0, fcolor = ROOT.kBlack, fstyle = 3004,)
CMS.cmsDraw(ratio, "P,E", mcolor=ROOT.kBlack)


dicanv.SaveAs("%s.pdf"%fit) 
