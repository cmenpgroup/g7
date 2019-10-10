//stats_IsLep.C
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

int stats_IsLep(char *fname="g7Stats.root", Int_t ntrks=2)
{
  gROOT->Reset();
  
  Int_t i;
  Int_t NLEP = 10;
  Int_t NMAYBE = 10;

  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle[50];

  char cuts[500];
  sprintf(cuts,"Tracks>=%i",ntrks);

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  Float_t nEvts = t2->GetEntries();
  
  //  gStyle->SetOptStat(0);

  cout<<"**************************************************************"<<endl;
  cout<<"Fraction of events:"<<endl;

  TCanvas *c1 = new TCanvas("c1","c1",0,0,800,800);
  c1->Divide(2,2);

  c1->cd(1);
  gPad->SetLogy();

  sprintf(xtitle,"Number of IsLepton");
  sprintf(ytitle,"Fraction of Events");

  sprintf(hname,"hIsLepton1");
  sprintf(title,"IsLepton");
  hIsLepton  = new TH1F(hname,title,NLEP,-0.5,NLEP+.5);
  t2->Project(hname,"nIsLep","");
  
  hIsLepton1->Scale(1/nEvts);
  hIsLepton1->Draw();
  hIsLepton1->SetXTitle(xtitle);
  hIsLepton1->GetXaxis()->CenterTitle();
  hIsLepton1->SetYTitle(ytitle);
  hIsLepton1->GetYaxis()->CenterTitle();
  hIsLepton1->Draw();

  Float_t Ilep1 = hIsLepton1->Integral(2,NLEP+1);
  Float_t Ilep2 = hIsLepton1->Integral(3,NLEP+1);
  cout<<"\tw/o cut on tracks ->\t"<<Ilep1<<" "<<Ilep2<<endl;

  c1->cd(2);
  gPad->SetLogy();

  sprintf(hname,"hIsLepton2");
  hIsLepton2  = new TH1F(hname,title,NLEP,-0.5,NLEP+.5);
  t2->Project(hname,"nIsLep",cuts);
  
  hIsLepton2->Scale(1/nEvts);
  hIsLepton2->Draw();
  hIsLepton2->SetXTitle(xtitle);
  hIsLepton2->GetXaxis()->CenterTitle();
  hIsLepton2->SetYTitle(ytitle);
  hIsLepton2->GetYaxis()->CenterTitle();
  hIsLepton2->Draw();

  Float_t Ilepc1 = hIsLepton2->Integral(2,NLEP+1);
  Float_t Ilepc2 = hIsLepton2->Integral(3,NLEP+1);
  cout<<"\tw/ # tracks >= "<<ntrks<<" ->\t "<<Ilepc1<<" "<<Ilepc2<<endl;

  c1->cd(3);
  gPad->SetLogy();

  sprintf(xtitle,"Number of MaybeLepton");

  sprintf(hname,"hMaybeLepton1");
  sprintf(title,"MaybeLepton");
  hMaybeLepton1 = new TH1F(hname,title,NMAYBE+1,-0.5,NMAYBE+0.5);
  t2->Project(hname,"nMaybeLep","");
  
  hMaybeLepton1->Scale(1/nEvts);
  hMaybeLepton1->Draw();
  hMaybeLepton1->SetXTitle(xtitle);
  hMaybeLepton1->GetXaxis()->CenterTitle();
  hMaybeLepton1->SetYTitle(ytitle);
  hMaybeLepton1->GetYaxis()->CenterTitle();
  hMaybeLepton1->Draw();

  Float_t Imay1 = hMaybeLepton1->Integral(2,NMAYBE+1);
  Float_t Imay2 = hMaybeLepton1->Integral(3,NMAYBE+1);
  cout<<"\tw/o cut on tracks ->\t"<<Imay1<<" "<<Imay2<<endl;

  c1->cd(4);
  gPad->SetLogy();

  sprintf(hname,"hMaybeLepton2");
  sprintf(title,"MaybeLepton");
  hMaybeLepton2 = new TH1F(hname,title,NMAYBE+1,-0.5,NMAYBE+0.5);
  t2->Project(hname,"nMaybeLep",cuts);
  
  hMaybeLepton2->Scale(1/nEvts);
  hMaybeLepton2->Draw();
  hMaybeLepton2->SetXTitle(xtitle);
  hMaybeLepton2->GetXaxis()->CenterTitle();
  hMaybeLepton2->SetYTitle(ytitle);
  hMaybeLepton2->GetYaxis()->CenterTitle();
  hMaybeLepton2->Draw();

  Float_t Imayc1 = hMaybeLepton2->Integral(2,NMAYBE+1);
  Float_t Imayc2 = hMaybeLepton2->Integral(3,NMAYBE+1);
  cout<<"\tw/ # tracks >= "<<ntrks<<" ->\t "<<Imayc1<<" "<<Imayc2<<endl;
  cout<<"**************************************************************"<<endl;

  c1->Print("stats_IsLep.gif");
  c1->Print("stats_IsLep.ps");

  //  f->Close();

}
