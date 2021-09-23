#include "physical_analysis/Flow/real_flow/MpdCalculator.h"
#include "physical_analysis/Flow/real_flow/MpdCalculator.cxx"
#include "physical_analysis/Flow/get_dca/MakeFitDCA.cxx"
#include "physical_analysis/Flow/restore_dca/restore_dca.c"
#include "physical_analysis/Flow/get_dca/get_dca.cxx"
#include "physical_analysis/Flow/get_dca/get_fit.cxx"
#include "physical_analysis/Flow/get_centrality/get_multiplicity.cxx"

// #include "MpdEvent.h"
#include "physical_analysis/Flow/create_reduced_tree/reducedTreeCreator.C"

int main()
{
    TString inFileHistName, inFileTreeName, outFileName, dcaFileName1, dcaFileName2, gdfile, gffile;
    inFileTreeName = "/home/ubuntu/rootcern/files_root/AuAu_ecm7.7GeV_hydroON_EoS1PT_0-16fm_500ev_1000-1.reco.root";
    outFileName    = "/home/ubuntu/rootcern/files_root/redused.root";
    // dcaFileName    = inFileName;

    gdfile       = "/home/ubuntu/rootcern/testreduse/getdca.root";
    gffile       = "/home/ubuntu/rootcern/testreduse/fitdca.root";
    dcaFileName1 = "/home/ubuntu/rootcern/testreduse/dca1.root";
    dcaFileName2 = "/home/ubuntu/rootcern/testreduse/dca2.root";

    restore_dca(inFileTreeName, dcaFileName1);
    get_dca(dcaFileName1, gdfile);
    get_fit(gdfile, gffile);
    MakeFitDCA(gffile, dcaFileName2);

    get_multiplicity(dcaFileName1, inFileHistName, dcaFileName2);
    // outFileName = "/home/ubuntu/rootcern/test_pr.root";
    /*MpdCalculator mpd = MpdCalculator(inFileName, outFileName, dcaFileName);                                                    //
       mpd.CalculateResolutions(0);                                                                                                    //
       mpd.Write();*/

    reducedTreeCreator rtc = reducedTreeCreator(inFileHistName, dcaFileName1, outFileName, dcaFileName2);   //
    rtc.CreateReducedTree();

    return 0;
}
