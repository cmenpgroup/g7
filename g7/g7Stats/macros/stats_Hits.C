//stats_Hits.C
//
// 
// M. H. Wood
// University of Massachusetts, Amherst
//
// Notes: 
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

int stats_Hits(char *fname="g7Stats.root", Int_t ntrks=2)
{
  gROOT->Reset();
  
  Int_t i;
  Int_t NHITS = 15;
  Int_t NTRKS = 15;
  Int_t NPID = 20;

  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle[50];
  char item[50];

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  Float_t nEvts = t2->GetEntries();

  //  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",0,0,800,800);
  c1->Divide(2,2);

  c1->cd(1);
  sprintf(xtitle,"Number of Hits");
  sprintf(ytitle,"Fraction of Events");

  sprintf(hname,"hHits");
  sprintf(title,"Hits");
  hHits = new TH1F(hname,title,NHITS,-0.5,NHITS+.5);
  t2->Project(hname,"Hits","");
  
  hHits->Scale(1/nEvts);
  hHits->Draw();
  hHits->SetXTitle(xtitle);
  hHits->GetXaxis()->CenterTitle();
  hHits->SetYTitle(ytitle);
  hHits->GetYaxis()->CenterTitle();
  hHits->Draw();

  c1->cd(2);
  sprintf(xtitle,"Number of Tracks");

  sprintf(hname,"hTracks");
  sprintf(title,"Tracks");
  hTracks = new TH1F(hname,title,NTRKS+1,-0.5,NTRKS+0.5);
  t2->Project(hname,"Tracks","");
  
  hTracks->Scale(1/nEvts);
  hTracks->Draw();
  hTracks->SetXTitle(xtitle);
  hTracks->GetXaxis()->CenterTitle();
  hTracks->SetYTitle(ytitle);
  hTracks->GetYaxis()->CenterTitle();
  hTracks->Draw();

  Float_t sum = hTracks->Integral(ntrks+1,NTRKS+1);
  cout<<"Fraction of events with # tracks >= "<<ntrks<<" is "<<sum<<endl;

  // tracks label
  char tlabel[30]; 
  sprintf(tlabel,"Tracks>=%i, frac=%1.4f",ntrks,sum);
  TPaveLabel *tl = new TPaveLabel(5.0,0.4,15.0,0.45,tlabel);
  tl->SetTextSize(0.4);
  tl->Draw();

  c1->cd(3);
  sprintf(xtitle,"PID");

  sprintf(hname,"hPid");
  sprintf(title,"Pid");
  hPid = new TH1F(hname,title,NPID+1,-0.5,NPID+0.5);
  t2->Project(hname,"Pid","");

  Float_t Npid = hPid->Integral();
  cout<<Npid<<endl;
  hPid->Scale(1/Npid);  
  hPid->Draw();
  hPid->SetXTitle(xtitle);
  hPid->GetXaxis()->CenterTitle();
  hPid->SetYTitle(ytitle);
  hPid->GetYaxis()->CenterTitle();
  hPid->Draw();

  c1->cd(4);
  sprintf(xtitle,"Trigger Bit");

  sprintf(hname,"htbit");
  sprintf(title,"tbit");
  htbit = new TH1F(hname,title,NPID+1,-0.5,NPID+0.5);
  t2->Project(hname,"tbit","");

  Float_t Ntbit = htbit->Integral();
  cout<<Ntbit<<endl;
  htbit->Scale(1/Ntbit);
  htbit->Draw();
  htbit->SetXTitle(xtitle);
  htbit->GetXaxis()->CenterTitle();
  htbit->SetYTitle(ytitle);
  htbit->GetYaxis()->CenterTitle();
  htbit->Draw();

  c1->Print("stats_Hits.gif");
  c1->Print("stats_Hits.ps");

  //  f->Close();

}
