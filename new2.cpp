// #include "physical_analysis/Flow/real_flow/MpdCalculator.cxx"
// #include "physical_analysis/Flow/real_flow/MpdCalculator.h"

// #include "physical_analysis/Flow/create_reduced_tree/reducedTreeCreator.C"

#include "TString.h"
#include <iostream>

int main()
{
    std::cout << "Hello" << std::endl;
    TString inFileName, outFileName, dcaFileName;
    // inFileName = "/home/ubuntu/rootcern/AuAu_ecm7.7GeV_hydroON_EoS1PT_0-16fm_500ev_1000-1.reco.root";
    // MpdCalculator mpd = MpdCalculator(inFileName, outFileName, dcaFileName);                                                  //
    // mpd.CalculateResolutions(0);                                                                                                     //
    //  mpd.Write();

    // reducedTreeCreator rtc = reducedTreeCreator("inFileHistName", "inFileTreeName", "outFileName", "dcaFileName");   //
    // rtc.CreateReducedTree();

    return 0;
}
