#include <iostream>
#include <vector>
#include <TNamed.h>
#include <TString.h>
#include <TH1.h>
#include <math.h>

using namespace std;

void Vort22(const Int_t events2analyze = 200) {
 
  TString inname          =    "/home/sergey/rootExamples/*.root";   //urqmd-AuAu-4.0GeV-mb-eos0-5k-10.f14.root"; // .root
  //TString inname          =    "/media/alexey/Seagate Expansion Drive/3.4-eos0/urqmd-AuAu-4.0GeV-mb-eos0-5k-0.f14.root";
  
	Double_t sigM = 2.0, sigE = 2.0, energy = 4., koef = 1.; /// n-sigma bands for PID selection
	TString Generator = "URQMD", Tracking = "CF";
	MpdPid *pid = new MpdPid(sigM, sigE, energy, koef, Generator, Tracking, "pikapr");
	
	cout.precision(4);

	Int_t xbins = 600, ybins = 600,  zbins = 600;            //momentum
	Int_t xbins1 = 100, ybins1 = 100, zbins1 = 100;          //coordinate		

	Double_t xlow=-3., xup=3., ylow=-3., yup=3., zlow=-3., zup=3.;
	Double_t xlow1=-300., xup1=300., ylow1=-300., yup1=300., zlow1=-300., zup1=300.; 
	
	Int_t mcNtracks, mcTrackId, pdg, pdgmc, mother, charge, fNtracks, ID, ii, tofFlag, maxloca, nentries, Nka=0, noret=0, noprobcut=0;   //, y;
	Double_t px, py ,pz, mpdPt, mpdP, mcPt, dedx, m2, Eta, AbsEta, Theta, pion, kaon, proton, electron, maxprob, NumChargeTracks=0, mass;
	//Int_t xbin, ybin, zbin;
	TVector3 VCounter[10][10][10], Velocity[10][10][10];
	
	TH2F* VelocityXY = new TH2F("Velocity", "V(X,Y)", 10, xlow1, xup1, 10, ylow1, yup1);
	
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
	
	
	Double_t Vx, Vy, Vz, Energy, VortY[10-1][10][10-1], EEE[10][10][10];	
	
	for (int i=0; i<10; i++)
		for(int j = 0; j<10; j++)
			for(int k = 0; k<10; k++)
			{
			EEE[i][j][k] = 0;
			Velocity[i][j][k](0) = 0;
			Velocity[i][j][k](1) = 0;
			Velocity[i][j][k](2) = 0;
			}

	for (int i=0; i<9; i++)
		for(int j = 0; j<10; j++)
			for(int k = 0; k<9; k++)
			{
			VortY[i][j][k] = 0;
			}

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
	TVector3 Pulse[10][10][10];
	Double_t EE[10][10][10];
	for (int i = 0; i<10; i++)
		for(int j = 0; j<10; j++)
			for(int k = 0; k<10; k++)
			{
			EE[i][j][k] = 0;
			}
		for(Int_t iTrack = 0; iTrack < mcNtracks; iTrack++) //tracks loop
		{
		mpdtrack = (MpdTrack*) mpdTracks->UncheckedAt(iTrack);
		//ID = mpdtrack->GetID();
		//mctrack = (FairMCTrack*) mcTracks->UncheckedAt(ID);
		mctrack = (FairMCTrack*) mcTracks->UncheckedAt(iTrack);

		mcPt = mctrack->GetPt();
		pdgmc = mctrack->GetPdgCode();

		//mother = mctrack->GetMotherId();

		Double_t Px = mctrack->GetPx();
		Double_t Py = mctrack->GetPy();
		Double_t Pz = mctrack->GetPz();
		Double_t Pfull = mctrack->GetPt();
		//if (Pfull < 0.1)  continue;
		Double_t StartX = mctrack->GetStartX();
		Double_t StartY = mctrack->GetStartY();
		Double_t StartZ = mctrack->GetStartZ();
		mass = mctrack->GetMass();
		Energy = mctrack->GetEnergy();
		Int_t xbin = floor((StartX+300)/60);   
		Int_t ybin = floor((StartY+300)/60);
		Int_t zbin = floor((StartZ+300)/60);
		if(zbin <=9 && zbin>=0){     //нужно это условие тк есть единичные события выходящие за пределы [-300;300] и ломающие границы
		Pulse[xbin][ybin][zbin].SetXYZ(Pulse[xbin][ybin][zbin](0) + Px, Pulse[xbin][ybin][zbin](1) + Py, Pulse[xbin][ybin][zbin](2) + Pz);
		EE[xbin][ybin][zbin] += Energy;	
					}

		} //tracks end
	
	for(int i = 0; i<10; i++)
		for(int j = 0; j<10; j++)
			for(int k = 0; k<10; k++)
			{
			if(EE[i][j][k] != 0) 	{ 
						Velocity[i][j][k](0) += Pulse[i][j][k](0)/EE[i][j][k]; if(Pulse[i][j][k](0)/EE[i][j][k] != 0) { VCounter[i][j][k](0)++; Pulse[i][j][k](0) = 0; }
						Velocity[i][j][k](1) += Pulse[i][j][k](1)/EE[i][j][k]; if(Pulse[i][j][k](1)/EE[i][j][k] != 0) { VCounter[i][j][k](1)++; Pulse[i][j][k](1) = 0; }
						Velocity[i][j][k](2) += Pulse[i][j][k](2)/EE[i][j][k]; if(Pulse[i][j][k](2)/EE[i][j][k] != 0) { VCounter[i][j][k](2)++; Pulse[i][j][k](2) = 0; }
						}
			}
	
	}   // entries end
	for (int i = 0; i<10; i++)
		for(int j = 0; j<10; j++)
			for(int k = 0; k<10; k++)
			{
			if ( VCounter[i][j][k](0) != 0) { Velocity[i][j][k](0) = Velocity[i][j][k](0)/VCounter[i][j][k](0); }
			if ( VCounter[i][j][k](1) != 0) { Velocity[i][j][k](1) = Velocity[i][j][k](1)/VCounter[i][j][k](1); }
			if ( VCounter[i][j][k](2) != 0) { Velocity[i][j][k](2) = Velocity[i][j][k](2)/VCounter[i][j][k](2); }
			if(k == 5) 
			{ VelocityXY->SetBinContent(i+1,j+1, TMath::Sqrt(Velocity[i][j][k](0)*Velocity[i][j][k](0) + Velocity[i][j][k](1)*Velocity[i][j][k](1) + Velocity[i][j][k](2)*Velocity[i][j][k](2))); }	
			}

		
	int WinX=2000, WinY=1500;		//     РАЗМЕР ОКОН

	TCanvas* cVelocity = new TCanvas("Velocity", "V", WinX, WinY);
	cVelocity->Divide(1,1);
	cVelocity->cd(1); VelocityXY->Draw("LEGO");
	cVelocity->SaveAs("Velocity.gif");

}
