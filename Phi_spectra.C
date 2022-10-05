#define _VANILLA_ROOT_
#define _USE_MATH_DEFINE

// C++ headers

#include <iostream>
#include <string>
#include <cmath>
#include <vector>


// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TVector3.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TString.h"

//PicoDst headers
#include "../StPicoDstReader.h"
#include "../StPicoDst.h"
#include "../StPicoEvent.h"
#include "../StPicoTrack.h"
#include "../StPicoBTofHit.h"
#include "../StPicoBTowHit.h"
#include "../StPicoEmcTrigger.h"
#include "../StPicoBTofPidTraits.h"
#include "../StPicoTrackCovMatrix.h"
#include "../StPicoPhysicalHelix.h"
#include "../StPicoHelix.h"
#include "../PhysicalConstants.h"
#include "../SystemOfUnits.h"

//My headers
#include "Cuts.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
R__LOAD_LIBRARY(../libStPicoDst)
#endif


bool Event_cuts(StPicoEvent *event){
    Double_t Vz = event->primaryVertex().Z();
    Double_t Vx = event->primaryVertex().X();
    Double_t Vy = event->primaryVertex().Y();
    Double_t rV = sqrt(pow((event->primaryVertex().X() - 0.313),2) + pow((event->primaryVertex().Y() + 0.072),2));
    if(abs(event->primaryVertex().Z())>5.)  return false;   //45!
    if(rV> 1.)                              return false;
    return true;
}

bool Track_cuts(StPicoTrack *picoTrack, StPicoEvent *event){
    if(!picoTrack)                                              return false;
    if(!picoTrack->isPrimary())                                 return false;
    Double_t nHitsOverPoss = ((Double_t)(picoTrack->nHitsFit()) / (Double_t)(picoTrack->nHitsMax()));
    if(picoTrack->gDCA(event->primaryVertex()).Mag()>3.)        return false;
    if(abs(picoTrack->pMom().Eta())>1)                          return false;
    if(picoTrack->nHitsFit()<16.)                               return false;
    if(nHitsOverPoss<0.52)                                      return false;
    if(nHitsOverPoss>1.02)                                      return false;
    if(picoTrack->pMom().Pt()<0.15)                             return false;
    return true;
}

bool KaonSelection(StPicoTrack *picoTrack){
    if(abs(picoTrack->nSigmaKaon()) > 2)                        return false;
    return true;
}

//Int_t starLibs = gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
//Int_t picoLib  = gSystem->Load("StPicoEvent");
//Int_t picoLib1 = gSystem->Load("StRefMultCorr");

const Char_t *defaultOutFile = "Spectra_phi_with_wide.root";
const Char_t *defaultOutFile_wide = "M_inv_KK_mixing_z5_wide.root";

const Double_t M_Kaon_sqr = 0.24364; //MeV^2

using namespace std;

//int InvMass(const Char_t *inFile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/27GeV_production_2018/ReversedFullField/P19ib/2018/140/19140002/st_physics_19140002_raw_1000013.picoDst.root",   const Char_t *outFileName=defaultOutFile)
int Spectra_test1(const Char_t *inFile = "data_big.list",   const Char_t *outFileName=defaultOutFile, const Char_t *outFileName_wide=defaultOutFile_wide)
//int InvMass(const Char_t *inFile = "/media/ubuntu/Seagate Expansion Drive/Test_data/st_physics_12126079_raw_0020001.picoDst.root",   const Char_t *outFileName=defaultOutFile)
{

// Open input and output files --------------------------------------------------------------------------------
        StPicoDstReader *picoReader = new StPicoDstReader(inFile);
        picoReader->Init();
        TFile ofile(outFileName,"RECREATE");

// Initializing some variables --------------------------------------------------------------------------------
        Double_t RefMult, Vz, Vx, Vy, nHitsOverPoss;
        Int_t cent9 = 0;
        Double_t Pt_counter[17];
        for(int i = 1; i < 18; i++)
            Pt_counter[i] = 0;

// Initializing histograms ------------------------------------------------------------------------------------
        //TH1D* hInvMass_wide[9];
        //TH1D* hInvMass_background_wide[9];


        //TH1D* hDivide_InvMass_background_wide[9];

        //TH1D* hAdd_InvMass_minus_background_wide[9];

        TH1D* hInvMass[9][15];
        TH1D* hInvMass_background[9][15];

        TH1D* hDivide_InvMass_background[9][15];

        TH1D* hAdd_InvMass_minus_background[9][15];

        //Wide versions of histograms -----------------------------------------
        TH1D* hInvMass_wide[9][15];
        TH1D* hInvMass_background_wide[9][15];

        TH1D* hDivide_InvMass_background_wide[9][15];
        TH1D* hAdd_InvMass_minus_background_wide[9][15];



// Initializing histograms  for spectra------------------------------------------------------------------------------------
        TH1D* hBeta;
        hBeta = new TH1D("hBeta","Beta distribution", 150, 0., 1.5);

        TH1D* hSpectra[9];
        //hSpectra = new TH1D("hSpectra","Phi meson Spectra", 50, 0., 5.);

        TH1D* hInvMass_Pt_Summ[9];
        TH1D* hInvMass_cent9_Summ[15];

        TH1D* hInvMass_background_Pt_Summ[9];
        TH1D* hInvMass_background_cent9_Summ[15];

        TH1D* hAdd_InvMass_minus_background_Pt_Summ[9];
        TH1D* hAdd_InvMass_minus_background_cent9_Summ[15];

        /*TH1D* hSpectra_bins;
        Double_t Lower[16];
        Lower[0] = 0;
        Lower[1] = 0.3;
        Lower[2] = 0.4;
        Lower[3] = 0.5;
        Lower[4] = 0.6;
        Lower[5] = 0.7;
        Lower[6] = 0.8;
        Lower[7] = 0.9;
        Lower[8] = 1.;
        Lower[9] = 1.3;
        Lower[10] = 1.7;
        Lower[11] = 2.;
        Lower[12] = 2.5;
        Lower[13] = 3.;
        Lower[14] = 4.;
        Lower[15] = 5.;     //Здесь может в один момент вылезти ошибка, тк могут появиться импульсы Pt > 5., и тогда при заполнении гистограммы, индекс Pt_bin будет равен 15...?
        hSpectra_bins = new TH1D("hSpectra_bins", "Phi meson Spectra", 15,Lower);*/

// Defining histograms ----------------------------------------------------------------------------------------
/*
Гистограммы инвариантной массы пары каонов(положительного и отрицательного)
Отличие этого блока гистограмм от следующего в диапазоне, здесь он выбран шире для наглядности,
для того чтобы видеть, как в целом выглядит гистограмма, и пик на её фоне. Диапазон: (left_wide;right_wide) = (0.95;1.45)
*/

        //Не нужно вообще
        /*Double_t left_wide = 0.95, right_wide = 1.45;
        Double_t num_of_bins_wide = 500;

        for(int i = 0; i < 9; i++){
            TString name_hist_wide[4], title_hist_wide[4];

        name_hist_wide[0] = "hInvMass_wide_cent9_" + to_string(i);
        title_hist_wide[0] = "Invariant Mass of K+ K-, cent9 = " + to_string(i);
        hInvMass_wide[i] = new TH1D(name_hist_wide[0], title_hist_wide[0], num_of_bins_wide, left_wide, right_wide); // Inv Mass spectre

        name_hist_wide[1] = "hInvMass_background_plus_wide_cent9_" + to_string(i);
        title_hist_wide[1] = "Background. Used K+ K- from different events, cent9 = " + to_string(i);
        hInvMass_background_wide[i] = new TH1D(name_hist_wide[1], title_hist_wide[1], num_of_bins_wide, left_wide, right_wide); // Inv Mass spectre background only

        name_hist_wide[2] = "hDivide_InvMass_background_wide_cent9_" + to_string(i);
        title_hist_wide[2] = "Divide InvMass and InvMass_background, cent9 = " + to_string(i);
        hDivide_InvMass_background_wide[i] = new TH1D(name_hist_wide[2], title_hist_wide[2], num_of_bins_wide, left_wide, right_wide);

        name_hist_wide[3] = "hAdd_InvMass_minus_background_wide_cent9_" + to_string(i);
        title_hist_wide[3] = "InvMass - InvMass_background, cent9 = " + to_string(i);
        hAdd_InvMass_minus_background_wide[i] = new TH1D(name_hist_wide[3], title_hist_wide[3], num_of_bins_wide, left_wide, right_wide);
        }*/
//-------------------------------------------------------------------------------------------------------------
/*
Гистограммы инвариантной массы пары каонов(положительного и отрицательного)
Диапазон выбран для того, чтобы максимально приблизить пик для дальнейшей аппроксимации. Диапазон: (left;right) = (0.98; 1.08)
*/
        Double_t left = 0.98, right = 1.08;
        Double_t num_of_bins = 100;
        Double_t Lower[16];
        Lower[0] = 0;
        Lower[1] = 0.3;
        Lower[2] = 0.4;
        Lower[3] = 0.5;
        Lower[4] = 0.6;
        Lower[5] = 0.7;
        Lower[6] = 0.8;
        Lower[7] = 0.9;
        Lower[8] = 1.;
        Lower[9] = 1.3;
        Lower[10] = 1.7;
        Lower[11] = 2.;
        Lower[12] = 2.5;
        Lower[13] = 3.;
        Lower[14] = 4.;
        Lower[15] = 5.;     //Здесь может в один момент вылезти ошибка, тк могут появиться импульсы Pt > 5., и тогда при заполнении гистограммы, индекс Pt_bin будет равен 15...?

        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 15; j++){

                TString name_hist[4], title_hist[4];

                name_hist[0] = "hInvMass_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[0] = "Invariant Mass of K+ K-, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hInvMass[i][j] = new TH1D(name_hist[0], title_hist[0], num_of_bins, left, right); // Inv Mass spectre
                hInvMass[i][j]->SetXTitle("GeV");
                hInvMass[i][j]->SetYTitle("Entries");

                name_hist[1] = "hInvMass_background_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[1] = "Background. Used K+ K- from different events, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hInvMass_background[i][j] = new TH1D(name_hist[1], title_hist[1], num_of_bins, left, right); // Inv Mass spectre background only
                hInvMass_background[i][j]->SetXTitle("GeV");
                hInvMass_background[i][j]->SetYTitle("Entries");

                name_hist[2] = "hDivide_InvMass_background_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[2] = "Divide InvMass and InvMass_background, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hDivide_InvMass_background[i][j] = new TH1D(name_hist[2], title_hist[2], num_of_bins, left, right);
                hDivide_InvMass_background[i][j]->SetXTitle("GeV");
                hDivide_InvMass_background[i][j]->SetYTitle("InvMass/ background");

                name_hist[3] = "hAdd_InvMass_minus_background_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[3] = "InvMass - InvMass_background, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hAdd_InvMass_minus_background[i][j] = new TH1D(name_hist[3], title_hist[3], num_of_bins, left, right);
                hAdd_InvMass_minus_background[i][j]->SetXTitle("GeV");
                hAdd_InvMass_minus_background[i][j]->SetYTitle("InvMass - background");
            }


        for(int i = 0; i < 9; i++){
            TString name_hist, title_hist;
            name_hist = "hSpectra_cent9_" + to_string(i);
            title_hist = "Spectra phi, cent9 = " + to_string(i);
            hSpectra[i] = new TH1D(name_hist, title_hist, 15, Lower);
            hSpectra[i]->SetXTitle("Pt");
            hSpectra[i]->SetYTitle("dN^{2}/(2*pi*Pt)*dPt*dY*BR");

            TString name_hist_summ[3], title_hist_summ[3];
            name_hist_summ[0] = "hInvMass_Pt_Summ_cent9_" + to_string(i);
            title_hist_summ[0] = "Invariant Mass of K+ K-, Summ over Pt, cent9 = " + to_string(i);
            hInvMass_Pt_Summ[i] = new TH1D(name_hist_summ[0], title_hist_summ[0], num_of_bins, left, right);

            name_hist_summ[1] = "hInvMass_background_Pt_Summ_cent9_" + to_string(i);
            title_hist_summ[1] = "Background. Used K+ K- from different events, Summ over Pt, cent9 = " + to_string(i);
            hInvMass_background_Pt_Summ[i] = new TH1D(name_hist_summ[1], title_hist_summ[1], num_of_bins, left, right);

            name_hist_summ[2] = "hAdd_InvMass_minus_background_Pt_Summ_cent9_" + to_string(i);
            title_hist_summ[2] = "InvMass - InvMass_background, Summ over Pt, cent9 = " + to_string(i);
            hAdd_InvMass_minus_background_Pt_Summ[i] = new TH1D(name_hist_summ[2], title_hist_summ[2], num_of_bins, left, right);
        }

        for(int j = 0; j < 15; j++){
            TString name_hist_summ[3], title_hist_summ[3];
            name_hist_summ[0] = "hInvMass_cent9_Summ_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
            title_hist_summ[0] = "Invariant Mass of K+ K-, Summ over cent9, " + to_string(Lower[j]) + " < Pt < " + to_string(Lower[j+1]);
            hInvMass_cent9_Summ[j] = new TH1D(name_hist_summ[0], title_hist_summ[0], num_of_bins, left, right);

            name_hist_summ[1] = "hInvMass_background_cent9_Summ_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
            title_hist_summ[1] = "Background. Used K+ K- from different events, Summ over cent9, " + to_string(Lower[j]) + " < Pt < " + to_string(Lower[j+1]);
            hInvMass_background_cent9_Summ[j] = new TH1D(name_hist_summ[1], title_hist_summ[1], num_of_bins, left, right);

            name_hist_summ[2] = "hAdd_InvMass_minus_background_cent9_Summ_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]);
            title_hist_summ[2] = "InvMass - InvMass_background, Summ over cent9, " + to_string(Lower[j]) + " < Pt < " + to_string(Lower[j+1]);
            hAdd_InvMass_minus_background_cent9_Summ[j] = new TH1D(name_hist_summ[2], title_hist_summ[2], num_of_bins, left, right);
        }


        //Defining wide histograms --------------------------------------------------------------------------------

        Double_t left_wide = 0.95, right_wide = 3.45;
        Double_t num_of_bins_wide = 2500;

        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 15; j++){

                TString name_hist[4], title_hist[4];

                name_hist[0] = "hInvMass_wide_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[0] = "Invariant Mass of K+ K-, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hInvMass_wide[i][j] = new TH1D(name_hist[0], title_hist[0], num_of_bins_wide, left_wide, right_wide); // Inv Mass spectre
                hInvMass_wide[i][j]->SetXTitle("GeV");
                hInvMass_wide[i][j]->SetYTitle("Entries");

                name_hist[1] = "hInvMass_background_wide_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[1] = "Background. Used K+ K- from different events, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hInvMass_background_wide[i][j] = new TH1D(name_hist[1], title_hist[1], num_of_bins_wide, left_wide, right_wide); // Inv Mass spectre background only
                hInvMass_background_wide[i][j]->SetXTitle("GeV");
                hInvMass_background_wide[i][j]->SetYTitle("Entries");

                name_hist[2] = "hDivide_InvMass_background_wide_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[2] = "Divide InvMass and InvMass_background, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hDivide_InvMass_background_wide[i][j] = new TH1D(name_hist[2], title_hist[2], num_of_bins_wide, left_wide, right_wide);
                hDivide_InvMass_background_wide[i][j]->SetXTitle("GeV");
                hDivide_InvMass_background_wide[i][j]->SetYTitle("InvMass/ background");

                name_hist[3] = "hAdd_InvMass_minus_background_wide_cent9_" + to_string(i) + "_Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                title_hist[3] = "InvMass - InvMass_background, cent9 = " + to_string(i) + ", " + to_string(Lower[j]) +" < Pt < " + to_string(Lower[j+1]);
                hAdd_InvMass_minus_background_wide[i][j] = new TH1D(name_hist[3], title_hist[3], num_of_bins_wide, left_wide, right_wide);
                hAdd_InvMass_minus_background_wide[i][j]->SetXTitle("GeV");
                hAdd_InvMass_minus_background_wide[i][j]->SetYTitle("InvMass - background");
            }

// Setting the status of branches -----------------------------------------------------------------------------
        cout << "Explicit read status for some branches" << endl;
        picoReader->SetStatus("*",0);
        picoReader->SetStatus("Event*", 1);
        picoReader->SetStatus("Track*", 1);
        picoReader->SetStatus("BTofHit*",1);
        picoReader->SetStatus("BTofPidTraits*", 1);
        cout << "Status has been set" << endl;
//Variables for mixing events for background -------------------------------------------------------------------------------------------
        vector<double> track_params;
        vector<vector<double>> event_tracks;
        vector<vector<vector<double>>> RefMult_Class[9];

        // Reading by events ------------------------------------------------------------------------------------------
                Long64_t events2read = picoReader->chain()->GetEntries();
                if (events2read > 4300000) events2read = 4300000;
                for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {


                        //cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]\r";
                        if (iEvent %1000 == 0)
                            cout<<"Working on event #[" << iEvent << "/" << events2read << "]"<<endl;
                        Bool_t readEvent = picoReader->readPicoEvent(iEvent);
                        if( !readEvent ) { std::cout << "readPicoEvent failed!\n"; break;}
                        StPicoDst *dst = picoReader->picoDst();
                        StPicoEvent *event = dst->event();
                        if(iEvent + 1 == events2read)
                        {
                         cout << "Well_Done" << endl;
                        }

                        RefMult = event->refMult();
                        if      (RefMult > 466) {cent9 = 0;}    //в процентах       //0-5%          200GeV
                        else if (RefMult > 396) {cent9 = 1;}                        //5-10%
                        else if (RefMult > 281) {cent9 = 2;}                        //10-20%
                        else if (RefMult > 193) {cent9 = 3;}                        //20-30%
                        else if (RefMult > 125) {cent9 = 4;}                        //30-40%
                        else if (RefMult > 76)  {cent9 = 5;}                        //40-50%
                        else if (RefMult > 43)  {cent9 = 6;}                        //50-60%
                        else if (RefMult > 22)  {cent9 = 7;}                        //60-70%
                        else if (RefMult > 10)  {cent9 = 8;}                        //70-80%
                        //else if (RefMult > 0)   {cent9 = -1;}                       //не подходит нам

                        /*Vz = event->primaryVertex().Z();
                        Vx = event->primaryVertex().X();
                        Vy = event->primaryVertex().Y();



                        466, 396, 335,    281 ,    234, 193 ,    156,   125,    98,    76,    58,    43,    31,   22,    15,   10
                        0-5  5-10  10-15  15-20   20-25 25-30   30-35  35-40   40-45  45-50  50-55   55-60  60-65  65-70 70-75  75-80


                        Double_t rV = sqrt(pow((event->primaryVertex().X() - 0.313),2) + pow((event->primaryVertex().Y() + 0.072),2));

        // Event cuts -------------------------------------------------------------------------------------------------
                        if(abs(event->primaryVertex().Z())>5.) continue;   //45!
                        if(rV> 1.)				continue;*/

                        if(!Event_cuts(event))                  continue;


        // Reading by tracks ------------------------------------------------------------------------------------------
                        for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
                                StPicoTrack *picoTrack = dst->track(iTrk);
                                /*if(!picoTrack) 			continue;
                                if(!picoTrack->isPrimary())	continue;
                                nHitsOverPoss = ((Double_t)(picoTrack->nHitsFit()) / (Double_t)(picoTrack->nHitsMax()));

        // Track cuts------------------------------------------------------------------------------------------------
                                //Сделать отбор фи мезонов чисто по TPC, без TOF, без cut'a на импульс,
                                //Построить гистограммы nSigmaKaon(pPtot), m2(p*q), dEdx(pPtot,pPt)
                                //Фитировать S-B полиномом и гауссом, получить интеграл в области(пусть 2*sigma), который будет означать число сигнальных событий N^sig_KK -
                                // - это и будет число фи
                                //Эффективность можно брать из статей, прямо из таблиц подставлять значения
                                //Благодаря поправке на эффективность часть спектра в области малых Pt станет выше, и спектр примет нормальную форму

                                //if(picoTrack->pPtot() > 0.8 && !picoTrack->isTofTrack())    continue;       //для dEdX оставить, для TOF убрать
                                */

                                if(!Track_cuts(picoTrack, event))                   continue;

                                //добавить для pPtot() > 0.8 и nSigmaPion() < 2

                                //if(abs(picoTrack->nSigmaKaon()) > 2)                             continue;
                                if(!KaonSelection(picoTrack))                       continue;

                               /* if(abs(picoTrack->nSigmaElectron()) < 1)                  continue;
                                if(abs(picoTrack->nSigmaProton()) < 1)                      continue; */

                                //if(abs(picoTrack->nSigmaPion()) < 2 && !picoTrack->isTofTrack())    continue;


                                //if(abs(picoTrack->nSigmaPion()) < abs(picoTrack->nSigmaKaon()))     continue;       //???

        //Reading track parameters ---------------------------------------------------------------------------------------------
                                track_params.clear();
                                track_params.push_back(picoTrack->pMom().Mag2());           //pMom!!!   [0]
                                //hMag2Mom->Fill(picoTrack->pMom().X()*picoTrack->pMom().X() + picoTrack->pMom().Y()*picoTrack->pMom().Y() + picoTrack->pMom().Z()*picoTrack->pMom().Z());
                                track_params.push_back(picoTrack->pMom().X());      //Px    //pMom!!!   [1]
                                track_params.push_back(picoTrack->pMom().Y());      //Py                [2]
                                track_params.push_back(picoTrack->pMom().Z());      //Pz                [3]
                                /*Double_t Beta = 0;
                                Int_t IsTofTrack = 0;
                                if(picoTrack->isTofTrack()){
                                    IsTofTrack = 1;
                                    StPicoBTofPidTraits *trait = dst->btofPidTraits( picoTrack->bTofPidTraitsIndex() );
                                    //Сделать условие на существование trait
                                    Beta = static_cast<Double_t>(trait->btofBeta());

                                    hBeta->Fill(Beta);
                                }else{Beta = -1;}*/
                                //Отладка
                                /*if (TMath::Sqrt(picoTrack->pMom().Mag2()) > 0.8){
                                    cout<<"Track parameters: pPtot = "<<TMath::Sqrt(picoTrack->pMom().Mag2())<<" \t Beta = "<<Beta<<"   \t nSigmaKaon/Pion = "<<picoTrack->nSigmaKaon()<<"/"<<picoTrack->nSigmaPion()<<endl;
                                }*/
                                //-------
                                track_params.push_back((double)picoTrack->charge());            //[4]
                                /*track_params.push_back(Beta);                                   //[5]
                                track_params.push_back(IsTofTrack);                             //[6]
                                track_params.push_back(picoTrack->pMom().Eta());                //[7]*/

                                event_tracks.push_back(track_params);
                                //cout<<"track_params.size() = "<<track_params.size()<<endl;
                                //cout<<"event_tracks.size() =        "<<event_tracks.size()<<endl;

                        } 	// end iTrack

                        RefMult_Class[cent9].push_back(event_tracks);
                        if(RefMult_Class[cent9].size() == 3)                            //here we can change the number of mixing event || 3 for 2 mixing events, 4 for 3 and so on
                            RefMult_Class[cent9].erase(RefMult_Class[cent9].begin());
                        //Filling histogram for hInvMass -------------------------------------------------------------------------------

                        //cout<<"Size_plus = "<<Size_plus<< "\t Size_minus = "<<Size_minus<<endl;
                        int Number_of_tracks = event_tracks.size();
                        //cout<<"Number_of_tracks = "<< Number_of_tracks<< endl;

                        //here we can change the number of mixing event
                        for(int i_track = 0; i_track < Number_of_tracks; i_track++){
                            Double_t E_plus, E_minus, px1, py1, pz1, px2, py2, pz2;
                            Double_t Summ_Mom_Mag2;
                            Double_t M_sqr;
                            Double_t Eta1, Eta2;
                            //cout<<"LOOKING FOR K+!"<<endl;
                            if(event_tracks[i_track][4] > 0){
//                                if (TMath::Sqrt(event_tracks[i_track][0]) < 0.8){
                                    E_plus  = TMath::Sqrt(M_Kaon_sqr + event_tracks[i_track][0]);
                                    px1= event_tracks[i_track][1];
                                    py1= event_tracks[i_track][2];
                                    pz1= event_tracks[i_track][3];
//                                    Eta1 = event_tracks[i_track][7];
//                                }
/*                                if (event_tracks[i_track][6] == 1 && event_tracks[i_track][5] != 0 && TMath::Sqrt(event_tracks[i_track][0]) > 0.8 ){
                                    Double_t Beta1 = event_tracks[i_track][5];
                                    M_sqr = event_tracks[i_track][0]*(1/(Beta1*Beta1) - 1);
                                    E_plus = TMath::Sqrt(M_sqr + event_tracks[i_track][0]);
                                    px1= event_tracks[i_track][1];
                                    py1= event_tracks[i_track][2];
                                    pz1= event_tracks[i_track][3];
                                    Eta1 = event_tracks[i_track][7];
                                }
*/
                                //cout<<"FOUND K+!!!"<<endl;
                                //cout<<"Here his params: E_plus = "<<E_plus<<"; px1 = "<<px1<<"; py1 = "<<py1<<"; pz1 = "<< pz1 << endl;
                                //cout<<"LOOKING FOR K-!"<<endl;
                                for(int j_track = 0; j_track < Number_of_tracks; j_track++){
                                    if(event_tracks[j_track][4] < 0){
                                        //cout<<"FOUND K-!!!"<<endl;
//                                        if (TMath::Sqrt(event_tracks[j_track][0]) < 0.8){
                                            E_minus = TMath::Sqrt(M_Kaon_sqr + event_tracks[j_track][0]);
                                            px2 = event_tracks[j_track][1];
                                            py2 = event_tracks[j_track][2];
                                            pz2 = event_tracks[j_track][3];
//                                            Eta2 = event_tracks[j_track][7];
//                                        }
/*                                        if (event_tracks[j_track][6] == 1 && event_tracks[j_track][5] != 0 && TMath::Sqrt(event_tracks[j_track][0]) > 0.8 ){
                                            Double_t Beta2 = event_tracks[j_track][5];
                                            M_sqr = event_tracks[j_track][0]*(1/(Beta2*Beta2) - 1);
                                            E_minus = TMath::Sqrt(M_sqr + event_tracks[j_track][0]);
                                            px2= event_tracks[j_track][1];
                                            py2= event_tracks[j_track][2];
                                            pz2= event_tracks[j_track][3];
                                            Eta2 = event_tracks[j_track][7];
                                        }
*/
                                        //cout<<"Here his params: E_minus = "<<E_minus<<"; px2 = "<<px2<<"; py2 = "<<py2<<"; pz2 = "<< pz2 << endl;

                                        //cout<<"TIME TO RESULT!"<<endl;
                                        Summ_Mom_Mag2 = (px1 + px2)*(px1 + px2) + (py1 + py2)*(py1 + py2) + (pz1 + pz2)*(pz1 + pz2);

                                        Double_t M_inv_KK = TMath::Sqrt(pow((E_plus +E_minus), 2) - Summ_Mom_Mag2 );
                                        //cout<<"E_plus = "<<E_plus<<"; E_minus = "<<E_minus<<"; Sum_Mom_Mag2 = "<<Summ_Mom_Mag2<<endl;
                                        //cout<<"M_inv_KK = " << M_inv_KK<<endl;
        //Rapidity --------------------------------------------------------------------------------------------
                                      /*  Double_t Rapidity1, Rapidity2, Rapidity12, Rapidity12_check;
                                        Rapidity1 = 0.5*TMath::Log((E_plus+pz1)/(E_plus-pz1));      //wikipedia
                                        Rapidity2 = 0.5*TMath::Log((E_minus+pz2)/(E_minus-pz2));
                                        Rapidity12 = Rapidity1 + Rapidity2;
                                        Rapidity12_check = 0.5*TMath::Log(((E_plus + E_minus) + (pz1+pz2))/((E_plus + E_minus) - (pz1+pz2)));*/

                                        //cout<<"Rapidity parameters: Rapidity1 = "<<Rapidity1<<"\t Rapidity2 = "<<Rapidity2<<"\t Rapidity12 = "<<Rapidity12<<"\t Rapidity12_check = "<<Rapidity12_check<<endl;

        //PseudoRapidity --------------------------------------------------------------------------------------
                                       /* Double_t PseudoRapidity1, PseudoRapidity2, PseudoRapidity12, PseudoRapidity12_check;
                                        Double_t P_mod1, P_mod2, P_mod12;
                                        P_mod1  = TMath::Sqrt(px1*px1 + py1*py1 + pz1*pz1);
                                        P_mod2  = TMath::Sqrt(px2*px2 + py2*py2 + pz2*pz2);
                                        P_mod12 = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2) + (pz1+pz2)*(pz1+pz2));
                                        PseudoRapidity1 = 0.5*TMath::Log((P_mod1 + pz1)/(P_mod1 - pz1));
                                        PseudoRapidity2 = 0.5*TMath::Log((P_mod2 + pz2)/(P_mod2 - pz2));
                                        PseudoRapidity12 = PseudoRapidity1 + PseudoRapidity2;
                                        PseudoRapidity12_check = 0.5*TMath::Log((P_mod12 + (pz1+pz2))/(P_mod12 - (pz1+pz2)));*/
                                        //cout<<"Real PseudoRapidity: Eta1 = "<<Eta1<<"  \t Eta2 = "<<Eta2<<endl;
                                        //cout<<"PseudoRapidity: PseudoRapidity1 = "<<PseudoRapidity1<<"\t PseudoRapidity2 = "<<PseudoRapidity2<<"\t PseudoRapidity12 = "<<PseudoRapidity12<<"\t PseudoRapidity12_check = "<<PseudoRapidity12_check<<endl;

                                        //if (abs(Rapidity12_check) > 0.5)      continue;       //Так же не забыть добавить в background
        //Dip-angle cut -------------------------------------------------------------------------------------
                                        /*Double_t Dip_angle_theta;       //вопросы?
                                        Dip_angle_theta = TMath::ACos((TMath::Sqrt(px1*px1+py1*py1)*TMath::Sqrt(px2*px2+py2*py2) + pz1*pz2)/(TMath::Sqrt(px1*px1+py1*py1+pz1*pz1)*TMath::Sqrt(px2*px2+py2*py2+pz2*pz2)));
                                        if(Dip_angle_theta < 0.04)      continue;*/

                                        Double_t Pt_KK = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));    //Pt для K+K-
                                        Int_t Pt_bin = -1;
                                        if      (Pt_KK > 5.)    {Pt_bin = 15;}      //сделать через switch
                                        else if (Pt_KK > 4.)    {Pt_bin = 15;}
                                        else if (Pt_KK > 3.)    {Pt_bin = 14;}
                                        else if (Pt_KK > 2.5)   {Pt_bin = 13;}
                                        else if (Pt_KK > 2.)    {Pt_bin = 12;}
                                        else if (Pt_KK > 1.7)   {Pt_bin = 11;}
                                        else if (Pt_KK > 1.3)   {Pt_bin = 10;}
                                        else if (Pt_KK > 1.)    {Pt_bin = 9;}
                                        else if (Pt_KK > 0.9)   {Pt_bin = 8;}
                                        else if (Pt_KK > 0.8)   {Pt_bin = 7;}
                                        else if (Pt_KK > 0.7)   {Pt_bin = 6;}
                                        else if (Pt_KK > 0.6)   {Pt_bin = 5;}
                                        else if (Pt_KK > 0.5)   {Pt_bin = 4;}
                                        else if (Pt_KK > 0.4)   {Pt_bin = 3;}
                                        else if (Pt_KK > 0.3)   {Pt_bin = 2;}
                                        else if (Pt_KK > 0.)    {Pt_bin = 1;}

                                        if(Pt_bin < 0)
                                            cout<<"Pt_bin < 0! Error!"<<endl;
                                        hInvMass[cent9][Pt_bin-1]->Fill(M_inv_KK);
                                        //
                                        hInvMass_wide[cent9][Pt_bin-1]->Fill(M_inv_KK);

                                        hInvMass_cent9_Summ[Pt_bin-1]->Fill(M_inv_KK);
                                        hInvMass_Pt_Summ[cent9]->Fill(M_inv_KK);

        //Filling Spectra -------------------------------------------------------------------------------------
                                      /*  if(M_inv_KK > 1.011 && M_inv_KK < 1.027){
                                            Double_t Pt_KK = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));    //Pt распределение для всех пар K+K-
                                            Int_t Pt_bin = 1;
                                            if      (Pt_KK > 5.)    {Pt_bin = 16;}      //сделать через switch
                                            else if (Pt_KK > 4.)    {Pt_bin = 15;}
                                            else if (Pt_KK > 3.)    {Pt_bin = 14;}
                                            else if (Pt_KK > 2.5)   {Pt_bin = 13;}
                                            else if (Pt_KK > 2.)    {Pt_bin = 12;}
                                            else if (Pt_KK > 1.7)   {Pt_bin = 11;}
                                            else if (Pt_KK > 1.3)   {Pt_bin = 10;}
                                            else if (Pt_KK > 1.)    {Pt_bin = 9;}
                                            else if (Pt_KK > 0.9)   {Pt_bin = 8;}
                                            else if (Pt_KK > 0.8)   {Pt_bin = 7;}
                                            else if (Pt_KK > 0.7)   {Pt_bin = 6;}
                                            else if (Pt_KK > 0.6)   {Pt_bin = 5;}
                                            else if (Pt_KK > 0.5)   {Pt_bin = 4;}
                                            else if (Pt_KK > 0.4)   {Pt_bin = 3;}
                                            else if (Pt_KK > 0.3)   {Pt_bin = 2;}
                                            Pt_counter[Pt_bin]++;

                                            Double_t dPt = 0.1 ;            //Сделать переменные dPt, dEta ...
                                            Double_t weight = 1/(2*TMath::Pi()*Pt_KK)*1/(0.1*1*0.491);  //Стоит еще разделить на N_evt после, и на коэфф эффективности (аксептанс) и на отношение S/B
                                            hSpectra->Fill(Pt_KK, weight);                              //Насчет коэффициентов смотреть формулу 3, раздел 5.2, страница 32 analysis note
                                            //hSpectra_bins->Fill(Pt_KK);
                                        }*/
                                        //cout<<"working on i = "<<i<< " and j = "<<j<<endl;
                                    }   //end 2 if
                                }  //end 2 for
                            } //end 1 if
                        }   //end 1 for


        //Filling background -------------------------------------------------------------------------------------
                        int Number_of_tracks_2 = RefMult_Class[cent9][0].size();

                        //cout<<"Number_of_tracks_2 = "<< Number_of_tracks_2<< endl;
                        if(RefMult_Class[cent9].size() == 2)
                        for(int i_track = 0; i_track < Number_of_tracks; i_track++){
                                Double_t E_plus, E_minus, px1, py1, pz1, px2, py2, pz2;
                                Double_t Summ_Mom_Mag2_1, Summ_Mom_Mag2_2;
                                Double_t M_sqr;
                                //cout<<"LOOKING FOR K+!"<<endl;
                                if(RefMult_Class[cent9][1][i_track][4] > 0){    //если заряд +
//                                    if (TMath::Sqrt(RefMult_Class[cent9][1][i_track][0]) < 0.8){
                                        E_plus  = TMath::Sqrt(M_Kaon_sqr + RefMult_Class[cent9][1][i_track][0]);
                                        px1= RefMult_Class[cent9][1][i_track][1];
                                        py1= RefMult_Class[cent9][1][i_track][2];
                                        pz1= RefMult_Class[cent9][1][i_track][3];
//                                    }
/*                                    if (RefMult_Class[cent9][1][i_track][6] == 1 && RefMult_Class[cent9][1][i_track][5] != 0 && TMath::Sqrt(RefMult_Class[cent9][1][i_track][0]) > 0.8 ){
                                        Double_t Beta1 = RefMult_Class[cent9][1][i_track][5];
                                        M_sqr = RefMult_Class[cent9][1][i_track][0]*(1/(Beta1*Beta1) - 1);
                                        E_plus = TMath::Sqrt(M_sqr + RefMult_Class[cent9][1][i_track][0]);
                                        px1= RefMult_Class[cent9][1][i_track][1];
                                        py1= RefMult_Class[cent9][1][i_track][2];
                                        pz1= RefMult_Class[cent9][1][i_track][3];
                                    }
*/
                                    //cout<<"FOUND K+!!!"<<endl;
                                    //cout<<"Here his params: E_plus = "<<E_plus<<"; px1 = "<<px1<<"; py1 = "<<py1<<"; pz1 = "<< pz1 << endl;
                                    //cout<<"LOOKING FOR K-!"<<endl;
                                    for(int j_track = 0; j_track < Number_of_tracks_2; j_track++){
                                        if(RefMult_Class[cent9][0][j_track][4] < 0){
//                                            if (TMath::Sqrt(RefMult_Class[cent9][0][j_track][0]) < 0.8){
                                                E_minus  = TMath::Sqrt(M_Kaon_sqr + RefMult_Class[cent9][0][j_track][0]);
                                                px2 = RefMult_Class[cent9][0][j_track][1];
                                                py2 = RefMult_Class[cent9][0][j_track][2];
                                                pz2 = RefMult_Class[cent9][0][j_track][3];
//                                            }
/*                                            if (RefMult_Class[cent9][0][j_track][6] == 1 && RefMult_Class[cent9][0][j_track][5] != 0 && TMath::Sqrt(RefMult_Class[cent9][0][j_track][0]) > 0.8 ){
                                                Double_t Beta2 = RefMult_Class[cent9][0][j_track][5];
                                                M_sqr = RefMult_Class[cent9][0][j_track][0]*(1/(Beta2*Beta2) - 1);
                                                E_minus = TMath::Sqrt(M_sqr + RefMult_Class[cent9][0][j_track][0]);
                                                px2= RefMult_Class[cent9][0][j_track][1];
                                                py2= RefMult_Class[cent9][0][j_track][2];
                                                pz2= RefMult_Class[cent9][0][j_track][3];
                                            }
*/
                                            //cout<<"FOUND K-!!!"<<endl;
                                            //cout<<"Here his params: E_minus = "<<E_minus<<"; px2 = "<<px2<<"; py2 = "<<py2<<"; pz2 = "<< pz2 << endl;

                                        Summ_Mom_Mag2_1 = (px1 + px2)*(px1 + px2) + (py1 + py2)*(py1 + py2) + (pz1 + pz2)*(pz1 + pz2);      //K+ from 1 event, K- from 2 event
                                        Double_t M_inv_KK = TMath::Sqrt(pow((E_plus +E_minus), 2) - Summ_Mom_Mag2_1 );

        //Dip-angle cut -------------------------------------------------------------------------------------
                                        /*Double_t Dip_angle_theta;
                                        Dip_angle_theta = TMath::ACos((TMath::Sqrt(px1*px1+py1*py1)*TMath::Sqrt(px2*px2+py2*py2) + pz1*pz2)/(TMath::Sqrt(px1*px1+py1*py1+pz1*pz1)*TMath::Sqrt(px2*px2+py2*py2+pz2*pz2)));
                                        if(Dip_angle_theta < 0.04)      continue;*/

                                        Double_t Pt_KK = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));    //Pt для K+K-
                                        Int_t Pt_bin = 1;
                                        if      (Pt_KK > 5.)    {Pt_bin = 15;}    //До скольки нам нужны импульсы?  //сделать через switch
                                        else if (Pt_KK > 4.)    {Pt_bin = 15;}
                                        else if (Pt_KK > 3.)    {Pt_bin = 14;}
                                        else if (Pt_KK > 2.5)   {Pt_bin = 13;}
                                        else if (Pt_KK > 2.)    {Pt_bin = 12;}
                                        else if (Pt_KK > 1.7)   {Pt_bin = 11;}
                                        else if (Pt_KK > 1.3)   {Pt_bin = 10;}
                                        else if (Pt_KK > 1.)    {Pt_bin = 9;}
                                        else if (Pt_KK > 0.9)   {Pt_bin = 8;}
                                        else if (Pt_KK > 0.8)   {Pt_bin = 7;}
                                        else if (Pt_KK > 0.7)   {Pt_bin = 6;}
                                        else if (Pt_KK > 0.6)   {Pt_bin = 5;}
                                        else if (Pt_KK > 0.5)   {Pt_bin = 4;}
                                        else if (Pt_KK > 0.4)   {Pt_bin = 3;}
                                        else if (Pt_KK > 0.3)   {Pt_bin = 2;}

                                        hInvMass_background[cent9][Pt_bin-1]->Fill(M_inv_KK);
                                        //
                                        hInvMass_background_wide[cent9][Pt_bin-1]->Fill(M_inv_KK);

                                        hInvMass_background_cent9_Summ[Pt_bin-1]->Fill(M_inv_KK);
                                        hInvMass_background_Pt_Summ[cent9]->Fill(M_inv_KK);
                                        }
                                    }
                                }
                                if(RefMult_Class[cent9][1][i_track][4] < 0){    //если заряд -
//                                    if (TMath::Sqrt(RefMult_Class[cent9][1][i_track][0]) < 0.8){
                                        E_minus  = TMath::Sqrt(M_Kaon_sqr + RefMult_Class[cent9][1][i_track][0]);
                                        px1= RefMult_Class[cent9][1][i_track][1];
                                        py1= RefMult_Class[cent9][1][i_track][2];
                                        pz1= RefMult_Class[cent9][1][i_track][3];
//                                    }
/*                                    if (RefMult_Class[cent9][1][i_track][6] == 1 && RefMult_Class[cent9][1][i_track][5] != 0 && TMath::Sqrt(RefMult_Class[cent9][1][i_track][0]) > 0.8 ){
                                        Double_t Beta1 = RefMult_Class[cent9][1][i_track][5];
                                        M_sqr = RefMult_Class[cent9][1][i_track][0]*(1/(Beta1*Beta1) - 1);
                                        E_minus = TMath::Sqrt(M_sqr + RefMult_Class[cent9][1][i_track][0]);
                                        px1= RefMult_Class[cent9][1][i_track][1];
                                        py1= RefMult_Class[cent9][1][i_track][2];
                                        pz1= RefMult_Class[cent9][1][i_track][3];
                                    }
*/
                                    for(int j_track = 0; j_track < Number_of_tracks_2; j_track++){
                                        if(RefMult_Class[cent9][0][j_track][4] > 0){
//                                            if (TMath::Sqrt(RefMult_Class[cent9][0][j_track][0]) < 0.8){
                                                E_plus  = TMath::Sqrt(M_Kaon_sqr + RefMult_Class[cent9][0][j_track][0]);
                                                px2 = RefMult_Class[cent9][0][j_track][1];
                                                py2 = RefMult_Class[cent9][0][j_track][2];
                                                pz2 = RefMult_Class[cent9][0][j_track][3];
//                                            }
/*                                            if (RefMult_Class[cent9][0][j_track][6] == 1 && RefMult_Class[cent9][0][j_track][5] != 0 && TMath::Sqrt(RefMult_Class[cent9][0][j_track][0]) > 0.8 ){
                                                Double_t Beta2 = RefMult_Class[cent9][0][j_track][5];
                                                M_sqr = RefMult_Class[cent9][0][j_track][0]*(1/(Beta2*Beta2) - 1);
                                                E_plus = TMath::Sqrt(M_sqr + RefMult_Class[cent9][0][j_track][0]);
                                                px2= RefMult_Class[cent9][0][j_track][1];
                                                py2= RefMult_Class[cent9][0][j_track][2];
                                                pz2= RefMult_Class[cent9][0][j_track][3];
                                            }
*/
                                        Summ_Mom_Mag2_2 = (px1 + px2)*(px1 + px2) + (py1 + py2)*(py1 + py2) + (pz1 + pz2)*(pz1 + pz2);      //K+ from 2 event, K- from 1 event
                                        Double_t M_inv_KK = TMath::Sqrt(pow((E_plus +E_minus), 2) - Summ_Mom_Mag2_2 );

        //Dip-angle cut -------------------------------------------------------------------------------------
                                        /*Double_t Dip_angle_theta;
                                        Dip_angle_theta = TMath::ACos((TMath::Sqrt(px1*px1+py1*py1)*TMath::Sqrt(px2*px2+py2*py2) + pz1*pz2)/(TMath::Sqrt(px1*px1+py1*py1+pz1*pz1)*TMath::Sqrt(px2*px2+py2*py2+pz2*pz2)));
                                        if(Dip_angle_theta < 0.04)      continue;*/

                                        Double_t Pt_KK = TMath::Sqrt((px1+px2)*(px1+px2) + (py1+py2)*(py1+py2));    //Pt для K+K-
                                        Int_t Pt_bin = 1;
                                        if      (Pt_KK > 5.)    {Pt_bin = 15;}      //сделать через switch
                                        else if (Pt_KK > 4.)    {Pt_bin = 15;}
                                        else if (Pt_KK > 3.)    {Pt_bin = 14;}
                                        else if (Pt_KK > 2.5)   {Pt_bin = 13;}
                                        else if (Pt_KK > 2.)    {Pt_bin = 12;}
                                        else if (Pt_KK > 1.7)   {Pt_bin = 11;}
                                        else if (Pt_KK > 1.3)   {Pt_bin = 10;}
                                        else if (Pt_KK > 1.)    {Pt_bin = 9;}
                                        else if (Pt_KK > 0.9)   {Pt_bin = 8;}
                                        else if (Pt_KK > 0.8)   {Pt_bin = 7;}
                                        else if (Pt_KK > 0.7)   {Pt_bin = 6;}
                                        else if (Pt_KK > 0.6)   {Pt_bin = 5;}
                                        else if (Pt_KK > 0.5)   {Pt_bin = 4;}
                                        else if (Pt_KK > 0.4)   {Pt_bin = 3;}
                                        else if (Pt_KK > 0.3)   {Pt_bin = 2;}

                                        hInvMass_background[cent9][Pt_bin-1]->Fill(M_inv_KK);
                                        //
                                        hInvMass_background_wide[cent9][Pt_bin-1]->Fill(M_inv_KK);

                                        hInvMass_background_cent9_Summ[Pt_bin-1]->Fill(M_inv_KK);
                                        hInvMass_background_Pt_Summ[cent9]->Fill(M_inv_KK);
                                        }
                                    }
                                }
                            }

                        event_tracks.clear();
                        //cout<<"NEW EVENT!!!"<<endl<<endl;

                } 		// end iEvent
                        for(int i = 0; i < 9; i++)
                            for(int j = 0; j < 15; j++){
                                /*hInvMass_background[i]->Scale(hInvMass[i]->GetEntries() / hInvMass_background[i]->GetEntries());
                                //
                                hInvMass_background_wide[i]->Scale(hInvMass_wide[i]->GetEntries() / hInvMass_background_wide[i]->GetEntries());*/
                                hInvMass_background[i][j]->Scale(hInvMass[i][j]->Integral(60,100)/hInvMass_background[i][j]->Integral(60,100));
                                //
                                hInvMass_background_wide[i][j]->Scale(hInvMass_wide[i][j]->Integral(80,120)/hInvMass_background_wide[i][j]->Integral(80,120));

                                hDivide_InvMass_background[i][j]->Divide(hInvMass[i][j], hInvMass_background[i][j]);
                                //
                                hDivide_InvMass_background_wide[i][j]->Divide(hInvMass_wide[i][j], hInvMass_background_wide[i][j]);

                                hAdd_InvMass_minus_background[i][j]->Add(hInvMass[i][j], hInvMass_background[i][j], 1, -1);
                                //
                                hAdd_InvMass_minus_background_wide[i][j]->Add(hInvMass_wide[i][j], hInvMass_background_wide[i][j], 1, -1);
                            }
                        for(int i = 0; i < 9; i++){
                            hInvMass_background_Pt_Summ[i]->Scale(hInvMass_Pt_Summ[i]->Integral(60,100)/hInvMass_background_Pt_Summ[i]->Integral(60,100));
                            hAdd_InvMass_minus_background_Pt_Summ[i]->Add(hInvMass_Pt_Summ[i], hInvMass_background_Pt_Summ[i], 1, -1);
                        }
                        for(int j = 0; j < 15; j++){
                            hInvMass_background_cent9_Summ[j]->Scale(hInvMass_cent9_Summ[j]->Integral(60,100)/hInvMass_background_cent9_Summ[j]->Integral(60,100));
                            hAdd_InvMass_minus_background_cent9_Summ[j]->Add(hInvMass_cent9_Summ[j], hInvMass_background_cent9_Summ[j], 1, -1);
                        }
                //Filling Spectra--------------------------------------------------------------------------

                        Double_t Weight[15];
                        Double_t dPt[15], Pt_bin_center[15];
                        dPt[0] = 0.3;               Pt_bin_center[0] = 0.15;
                        dPt[1] = 0.1;               Pt_bin_center[1] = 0.35;
                        dPt[2] = 0.1;               Pt_bin_center[2] = 0.45;
                        dPt[3] = 0.1;               Pt_bin_center[3] = 0.55;
                        dPt[4] = 0.1;               Pt_bin_center[4] = 0.65;
                        dPt[5] = 0.1;               Pt_bin_center[5] = 0.75;
                        dPt[6] = 0.1;               Pt_bin_center[6] = 0.85;
                        dPt[7] = 0.1;               Pt_bin_center[7] = 0.95;
                        dPt[8] = 0.3;               Pt_bin_center[8] = 1.15;
                        dPt[9] = 0.4;               Pt_bin_center[9] = 1.5;
                        dPt[10] = 0.3;              Pt_bin_center[10] = 1.85;
                        dPt[11] = 0.5;              Pt_bin_center[11] = 2.25;
                        dPt[12] = 0.5;              Pt_bin_center[12] = 2.75;
                        dPt[13] = 1.;               Pt_bin_center[13] = 3.5;
                        dPt[14] = 1.;               Pt_bin_center[14] = 4.5;
                        Double_t dY = 1;
                        Double_t BR = 0.491;

                        for(int i = 0; i < 15; i++)
                            Weight[i] = 1/(2*TMath::Pi()*Pt_bin_center[i]*dY*dPt[i]*BR);
                        Double_t N_phi = 0;
                        for(int i = 0; i < 9; i++)
                            for(int j = 0; j < 15; j++){
                                N_phi = hAdd_InvMass_minus_background[i][j]->Integral(36,43);
                                hSpectra[i]->Fill(Pt_bin_center[j], N_phi*Weight[j]);
                            }



/*                        for(int i = 1; i < 15; i++){                                    //Заполнить через Fill
                            Double_t bin_center = hSpectra_bins->GetBinCenter(i);
                            Double_t bin_width = hSpectra_bins->GetBinWidth(i);
                            Pt_counter[i] = Pt_counter[i]/(2*TMath::Pi()*bin_center)/(1*bin_width*0.491);
                            hSpectra_bins->SetBinContent(i,Pt_counter[i]);
                        }
*/
                // Saving histograms to a file --------------------------------------------------------------------------------
                        //hBeta                                           ->Write();
                        //hSpectra                                        ->Write();
                        //hSpectra_bins                                   ->Write();

                        for(int i = 0; i < 9; i++){
                            TString name_dir = "cent9_" + to_string(i);
                            ofile.mkdir(name_dir);
                            ofile.cd();
                            ofile.cd(name_dir);
                            for(int j = 0; j < 15; j++){
                                TString name_dir2 = name_dir + "/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                                ofile.mkdir(name_dir2);
                            }
                            ofile.cd();
                        }
                        ofile.cd();

                        ofile.mkdir("Summ_over_Pt");
                        ofile.cd("Summ_over_Pt");
                        for(int i = 0; i < 9; i++){
                            TString name_dir = "Summ_over_Pt/cent9_" + to_string(i);
                            ofile.mkdir(name_dir);
                        }
                        ofile.cd();
                        ofile.mkdir("Summ_over_cent9");
                        ofile.cd("Summ_over_cent9");
                        for(int j = 0; j < 15; j++){
                            TString name_dir = "Summ_over_cent9/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                            ofile.mkdir(name_dir);
                        }
                        ofile.cd();
                        ofile.mkdir("Spectra");

                        for(int i = 0; i < 9; i++){
                            TString name_dir = "wide_cent9_" + to_string(i);
                            ofile.mkdir(name_dir);
                            ofile.cd();
                            ofile.cd(name_dir);
                            for(int j = 0; j < 15; j++){
                                TString name_dir2 = name_dir + "/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                                ofile.mkdir(name_dir2);
                            }
                            ofile.cd();
                        }


                        for(int i = 0; i < 9; i++){
                            ofile.cd();
                            TString name_dir = "cent9_" + to_string(i);
                            for(int j = 0; j < 15; j++){
                                TString name_dir2 = name_dir + "/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                                ofile.cd();
                                ofile.cd(name_dir2);

                                hInvMass[i][j]                              ->Write();
                                hInvMass_background[i][j]                   ->Write();

                                //hDivide_InvMass_background[i][j]            ->Write();

                                hAdd_InvMass_minus_background[i][j]         ->Write();


                        //ofile.Close();

                        //TFile ofile_wide(outFileName_wide,"RECREATE");

                        /*hInvMass_wide[i]                                ->Write();
                        hInvMass_background_wide[i]                     ->Write();


                        hDivide_InvMass_background_wide[i]              ->Write();


                        hAdd_InvMass_minus_background_wide[i]           ->Write();*/
                            }
                        }

                        for(int i = 0; i < 9; i++){
                            ofile.cd();
                            TString name_dir = "Summ_over_Pt/cent9_" + to_string(i);
                            ofile.cd(name_dir);
                            hInvMass_Pt_Summ[i]                            ->Write();
                            hInvMass_background_Pt_Summ[i]                 ->Write();
                            hAdd_InvMass_minus_background_Pt_Summ[i]       ->Write();
                        }

                        ofile.cd();
                        for(int j = 0; j < 15; j++){
                            ofile.cd();
                            TString name_dir2 = "Summ_over_cent9/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                            ofile.cd(name_dir2);
                            hInvMass_cent9_Summ[j]                         ->Write();
                            hInvMass_background_cent9_Summ[j]              ->Write();
                            hAdd_InvMass_minus_background_cent9_Summ[j]    ->Write();
                        }

                        ofile.cd();
                        ofile.cd("Spectra");
                        for(int i = 0; i < 9; i++){
                            hSpectra[i]                                 ->Write();
                        }

                        ofile.cd();

                        for(int i = 0; i < 9; i++){
                            ofile.cd();
                            TString name_dir = "wide_cent9_" + to_string(i);
                            for(int j = 0; j < 15; j++){
                                TString name_dir2 = name_dir + "/Pt_(" + to_string(Lower[j]) + ", " + to_string(Lower[j+1]) + ")";
                                ofile.cd();
                                ofile.cd(name_dir2);

                                hInvMass_wide[i][j]                              ->Write();
                                hInvMass_background_wide[i][j]                   ->Write();

                                //hDivide_InvMass_background[i][j]            ->Write();

                                hAdd_InvMass_minus_background_wide[i][j]         ->Write();


                        //ofile.Close();

                        //TFile ofile_wide(outFileName_wide,"RECREATE");

                        /*hInvMass_wide[i]                                ->Write();
                        hInvMass_background_wide[i]                     ->Write();


                        hDivide_InvMass_background_wide[i]              ->Write();


                        hAdd_InvMass_minus_background_wide[i]           ->Write();*/
                            }
                        }

                        //ofile_wide.Close();
                        //ofile.Write();        //записывает все гистограммы в файл
                        ofile.Close();
                 // Removing pointers ------------------------------------------------------------------------------------------
                        delete hBeta;
                        delete *hSpectra;
                        //delete hSpectra_bins;                        
                        delete *hInvMass_cent9_Summ;
                        delete *hInvMass_Pt_Summ;

                        delete *hInvMass_background_cent9_Summ;
                        delete *hInvMass_background_Pt_Summ;

                        delete *hAdd_InvMass_minus_background_cent9_Summ;
                        delete *hAdd_InvMass_minus_background_Pt_Summ;


                        delete **hInvMass;
                        delete **hInvMass_background;

                        delete **hDivide_InvMass_background;

                        delete **hAdd_InvMass_minus_background;

                        //
                        /*delete *hInvMass_wide;
                        delete *hInvMass_background_wide;

                        delete *hDivide_InvMass_background_wide;

                        delete *hAdd_InvMass_minus_background_wide;*/

// End of the program -----------------------------------------------------------------------------------------
        picoReader->Clear();
        picoReader->Finish();

        return 0;
}

