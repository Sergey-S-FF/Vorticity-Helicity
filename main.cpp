#include <iostream>
#include <MpdPid.h>
#include <FairMCTrack.h>
#include <iostream>
#include <vector>
#include <TNamed.h>
#include <TStyle.h>
#include <TString.h>
#include <TH1.h>
#include <math.h>
//#include <string.h>


using namespace std;

void Vort22(const Int_t events2analyze = 200) {

    TString inname          =    "/windows/JINR/*.root";
  //TString inname          =    "/home/sergey/rootExamples/*.root";   //urqmd-AuAu-4.0GeV-mb-eos0-5k-10.f14.root"; // .root
  //TString inname          =    "/media/alexey/Seagate Expansion Drive/3.4-eos0/urqmd-AuAu-4.0GeV-mb-eos0-5k-0.f14.root";

    Double_t sigM = 2.0, sigE = 2.0, energy = 9., koef = 1.; /// n-sigma bands for PID selection
    TString Generator = "URQMD", Tracking = "CF";
    MpdPid *pid = new MpdPid(sigM, sigE, energy, koef, Generator, Tracking, "pikapr");

    cout.precision(4);

    Int_t xbins = 600, ybins = 600,  zbins = 600;            //momentum
    Int_t xbins1 = 100, ybins1 = 100, zbins1 = 100;          //coordinate

    Int_t BinsX, BinsY, BinsZ;
    BinsX = BinsY = BinsZ = 20;
    Int_t VortBinsX = BinsX-1, VortBinsY = BinsY-1, VortBinsZ = BinsZ-1;

    Double_t xlow=-3., xup=3., ylow=-3., yup=3., zlow=-3., zup=3.;
    Double_t xlow1, xup1, ylow1, yup1, zlow1, zup1;
    xlow1 = ylow1 = zlow1 = -300.; xup1 = yup1 = zup1 = 300.;

    Double_t Vortxlow1=xlow1+((xup1-xlow1)/(2*BinsX)), Vortxup1=xup1-((xup1-xlow1)/(2*BinsX));
    Double_t Vortylow1=ylow1+((yup1-ylow1)/(2*BinsY)), Vortyup1=yup1-((yup1-ylow1)/(2*BinsY));
    Double_t Vortzlow1=zlow1+((zup1-zlow1)/(2*BinsZ)), Vortzup1=zup1-((zup1-zlow1)/(2*BinsZ));

    Int_t mcNtracks, mcTrackId, pdg, pdgmc, mother, charge, fNtracks, ID, ii, tofFlag, maxloca, nentries, Nka=0, noret=0, noprobcut=0;   //, y;
    Double_t Px, Py ,Pz, mpdPt, mpdP, mcPt, dedx, m2, Eta, AbsEta, Theta, pion, kaon, proton, electron, maxprob, NumChargeTracks=0, mass, mpdPPt;
    TVector3 VCounter[BinsX][BinsY][BinsZ], Velocity[BinsX][BinsY][BinsZ], Pulse[BinsX][BinsY][BinsZ];
//---------------------Создаем Гистограммы Velocity-------------------------------------
    //TString HistNames1[10] = {"Z = (-300:-240)","(-240:-180)","(-180:-120)","(-120:-60)","(-60:0)","(0:60)","(60:120)","(120:180)","(180:240)","(240:300)"};
    TString HistNames1[BinsX];
    for (int i = 0; i < BinsX; i++ )
        HistNames1[i] = to_string(i+1);
    TH2F *VelocityHist[BinsX];
    for (int i = 0; i < BinsX; i++)
        VelocityHist[i] = new TH2F(HistNames1[i],HistNames1[i],BinsX, xlow1, xup1, BinsY, ylow1, yup1);
//--------------------------------------------------------------------------------------
//---------------------Создаем Гистограммы Vorticity-------------------------------------
    //TString HistNames2[10] = {"1 Vort","2 Vort","3 Vort","4 Vort","5 Vort","6 Vort","7 Vort","8 Vort","9 Vort","10 Vort"};

    TString HistNames2[BinsX];
    for (int i = 0; i < BinsX; i++ )
        HistNames2[i] = to_string(i+1) + " hist";
    TH2F *VorticityHist[BinsX];
    for (int i = 0; i < BinsX; i++)
        VorticityHist[i] = new TH2F(HistNames2[i],HistNames2[i], VortBinsX, Vortxlow1, Vortxup1, VortBinsZ, Vortzlow1, Vortzup1);
//---------------------------------------------------------------------------------------

    //TH2F* VelocityXY = new TH2F("Velocity", "V(X,Y)", 10, xlow1, xup1, 10, ylow1, yup1);


    TChain chain("cbmsim");
    chain.Add(inname);

    TClonesArray *mcTracks = NULL;
    TClonesArray *GlobalTracks = NULL;
    TClonesArray *mpdTracks = NULL;
    FairMCEventHeader *fmcHeader = NULL;

    chain.SetBranchAddress("MCTrack", &mcTracks);
    FairMCTrack *mctrack = new FairMCTrack;
    chain.SetBranchAddress("MCEventHeader.", &fmcHeader);
    MpdEvent *event = new MpdEvent();
    chain.SetBranchAddress("MPDEvent.", &event);
    MpdTrack *mpdtrack = new MpdTrack;


    Double_t Vx, Vy, Vz, Energy,EE[BinsX][BinsY][BinsZ], VortY[VortBinsX][BinsY][VortBinsZ]/*, EEE[10][10][10]*/;
    Double_t StartX, StartY, StartZ;
    Int_t xbin, ybin, zbin;
    //-----------------------------------------------------------

    for (int i=0; i<VortBinsX; i++)
        for(int j = 0; j<BinsY; j++)
            for(int k = 0; k<VortBinsZ; k++)
            {
            VortY[i][j][k] = 0;
            }
    //-----------------------------------------------------------
    if (events2analyze == 0)
        {nentries = chain.GetEntries(); // get all entries
        cout<<nentries<<endl;}
    else {nentries = events2analyze;}

    for (Int_t i = 0; i < nentries; i ++)  // entries loop
    {
    chain.GetEntry(i); // current entry
    mcNtracks = mcTracks->GetEntriesFast();
    mpdTracks = (TClonesArray*) event->GetGlobalTracks();
    fNtracks = mpdTracks->GetEntriesFast();
    cout<<i<<"/"<<nentries<<"\r"<<flush;

//-----------------------------------------------------------
    for (int i = 0; i < BinsX; i++)
        for(int j = 0; j < BinsY; j++)
            for(int k = 0; k < BinsZ; k++)
            {
            EE[i][j][k] = 0;
            }
//---------------------------------------------------------------
    //memset()
        for(Int_t iTrack = 0; iTrack < mcNtracks; iTrack++) //tracks loop
        {
        mpdtrack = (MpdTrack*) mpdTracks->UncheckedAt(iTrack);
        //ID = mpdtrack->GetID();
        //mctrack = (FairMCTrack*) mcTracks->UncheckedAt(ID);
        mctrack = (FairMCTrack*) mcTracks->UncheckedAt(iTrack);

        mcPt = mctrack->GetPt();
        pdgmc = mctrack->GetPdgCode();
//--------------------CUTS-----------------------------------------------------
        //mother = mctrack->GetMotherId();
        //Eta = mpdtrack->GetEta();
        //AbsEta = TMath::Abs(Eta);
        //mpdPt = mctrack->GetPt();
        //if (mctrack->GetNPoints() < 20) continue;
        //if (AbsEta > 0.5) continue;
        //if ((mpdPt < 0.1) /*|| (mpdPt > 2.5)*/) continue;
//------------------------------------------------------------------------------
        Px = mctrack->GetPx();
        Py = mctrack->GetPy();
        Pz = mctrack->GetPz();
        //Double_t Pfull = mctrack->GetPt();
        //if (Pfull < 0.1)  continue;
        StartX = mctrack->GetStartX();
        StartY = mctrack->GetStartY();
        StartZ = mctrack->GetStartZ();
        mass = mctrack->GetMass();                  //ПЕРЕДЕЛАТЬ ВСЕ ЧЕРЕЗ TVECTOR3
        Energy = mctrack->GetEnergy();
        xbin = floor((StartX+xup1)/((xup1-xlow1)/BinsX));
        ybin = floor((StartY+yup1)/((yup1-ylow1)/BinsY));
        zbin = floor((StartZ+zup1)/((zup1-zlow1)/BinsZ));
        if( xbin >= 0  && ybin >= 0 && zbin >= 0 && xbin < BinsX && ybin < BinsY && zbin < BinsZ)
            {
            //нужно это условие тк есть единичные события выходящие за пределы [-300;300] и ломающие границы
        Pulse[xbin][ybin][zbin].SetXYZ(Pulse[xbin][ybin][zbin](0) + Px, Pulse[xbin][ybin][zbin](1) + Py, Pulse[xbin][ybin][zbin](2) + Pz);
        EE[xbin][ybin][zbin] += Energy;
            }
        } //tracks end




//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    for(int i = 0; i<BinsX; i++)
        for(int j = 0; j<BinsY; j++)
            for(int k = 0; k<BinsZ; k++)
            {
                if(EE[i][j][k] != 0)
                    {
                        Velocity[i][j][k](0) += Pulse[i][j][k](0)/EE[i][j][k];
                        if(Pulse[i][j][k](0)/EE[i][j][k] != 0)
                            {
                            VCounter[i][j][k](0)++;
                            Pulse[i][j][k](0) = 0;
                            }

                        Velocity[i][j][k](1) += Pulse[i][j][k](1)/EE[i][j][k];
                        if(Pulse[i][j][k](1)/EE[i][j][k] != 0)
                            {
                            VCounter[i][j][k](1)++;
                            Pulse[i][j][k](1) = 0;
                            }

                        Velocity[i][j][k](2) += Pulse[i][j][k](2)/EE[i][j][k];
                        if(Pulse[i][j][k](2)/EE[i][j][k] != 0)
                            {
                            VCounter[i][j][k](2)++;
                            Pulse[i][j][k](2) = 0;
                            }
                    }//end if
            }//end for
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------


    }   // entries end

//-------------- СЧИТАЕМ VELOCITY --------------------------------------------------
    for (int i = 0; i<BinsX; i++)
        for(int j = 0; j<BinsY; j++)
            for(int k = 0; k<BinsZ; k++)
            {
            if ( VCounter[i][j][k](0) != 0) { Velocity[i][j][k](0) = Velocity[i][j][k](0)/VCounter[i][j][k](0); }
            if ( VCounter[i][j][k](1) != 0) { Velocity[i][j][k](1) = Velocity[i][j][k](1)/VCounter[i][j][k](1); }
            if ( VCounter[i][j][k](2) != 0) { Velocity[i][j][k](2) = Velocity[i][j][k](2)/VCounter[i][j][k](2); }
          /*  Velocity[9][0][k](0) = 0;		//
            Velocity[9][0][k](1) = 0;		// ИСКУССТВЕННО УБИРАЕМ ПИК В УГЛУ
            Velocity[9][0][k](2) = 0;		//
            Velocity[9][9][k](0) = 0;		//
            Velocity[9][9][k](1) = 0;		// ИСКУССТВЕННО УБИРАЕМ ПИК В УГЛУ
            Velocity[9][9][k](2) = 0;		//
            Velocity[0][0][k](0) = 0;		//
            Velocity[0][0][k](1) = 0;		// ИСКУССТВЕННО УБИРАЕМ ПИК В УГЛУ
            Velocity[0][0][k](2) = 0;		//
            Velocity[0][9][k](0) = 0;		//
            Velocity[0][9][k](1) = 0;		// ИСКУССТВЕННО УБИРАЕМ ПИК В УГЛУ
            Velocity[0][9][k](2) = 0;	*/	//
            Double_t FullVelocity = Velocity[i][j][k].Mag();//TMath::Sqrt(Velocity[i][j][k](0)*Velocity[i][j][k](0) + Velocity[i][j][k](1)*Velocity[i][j][k](1) + Velocity[i][j][k](2)*Velocity[i][j][k](2));
            //if(k == 5)	//НУЖНЫЙ НАМ СЛОЙ
           // { VelocityXY->SetBinContent(i+1,j+1, FullVelocity); }
            VelocityHist[k]->SetBinContent(i+1,j+1, FullVelocity);
            }


//--------------------------------------------------------------------------------
//---------------------- СЧИТАЕМ VORTICITY ---------------------------------------
    for(int j = 0; j < BinsY; j++)
        for (int i = 0; i < VortBinsX; i++)
            for (int k = 0; k < VortBinsZ; k++)
            {
            VortY[i][j][k] = (Velocity[i][j][k+1](0) - Velocity[i][j][k](0))/((xup1-xlow1)/BinsX) - (Velocity[i+1][j][k](2) - Velocity[i][j][k](2))/((xup1-xlow1)/BinsX);
           // if (j==6)       //  Нужный нам слой Y
           // { VorticityY->SetBinContent(i+1, k+1, VortY[i][j][k]); }
            VorticityHist[j]->SetBinContent(i+1, k+1, VortY[i][j][k]);
            }

//---------------------- СЧИТАЕМ HELICITY -----------------------------------------



//----------------------- РИСУЕМ ГРАФИКИ -----------------------------------------
    int WinX=4000, WinY=2000;		//     РАЗМЕР ОКОН
    gStyle->SetOptStat(0);
    TCanvas* cVelocity = new TCanvas("Velocity", "Vel", WinX, WinY);
    cVelocity->Divide(5,4);
    for (int i = 1; i<=BinsX; i++)
        {cVelocity->cd(i); VelocityHist[i-1]->Draw("LEGO2");}
    cVelocity->SaveAs("Velocity.gif");  // МОЖНО СОХРАНИТЬ В РАСШИРЕНИИ .root
    cVelocity->SaveAs("Velocity.root");
//--------------------------------------------------------------------------------

    gStyle->SetOptStat(0);
    TCanvas* cVorticity = new TCanvas("Y component Vorticity", "Vort", WinX, WinY);
    cVorticity->Divide(5,4);  //(столбцов, строк)
    for(int i = 1; i<=BinsX; i++)
        {cVorticity->cd(i); VorticityHist[i-1]->Draw("Cont4");}
    cVorticity->SaveAs("Y_component_Vorticity.gif");  // МОЖНО СОХРАНИТЬ В РАСШИРЕНИИ .root
    cVorticity->SaveAs("Vorticity.root");

}
int main()
{
    Vort22(100);
    return 0;
}
