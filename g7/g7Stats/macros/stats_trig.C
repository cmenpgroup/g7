//stats_trig.C
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

int stats_trig(char *fname="g7Stats.root")
{
  gROOT->Reset();
  
  Int_t i, ibin;
  Int_t NTRIG = 64;
  Int_t NCOMB = 10;
  Int_t NBIT = 8;

  Float_t xbin, xval;
  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle[50];

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  Float_t nEvts = t2->GetEntries();

  //  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",0,0,1200,500);
  c1->Divide(3,1);

  c1->cd(1);
  sprintf(xtitle,"Triggers");
  sprintf(ytitle,"Fraction of Events");

  sprintf(hname,"htrig");
  sprintf(title,"Triggers");
  htrig = new TH1F(hname,title,NTRIG+1,-0.5,NTRIG+0.5);
  t2->Project(hname,"trig","");
  
  Float_t Ntrig = htrig->Integral();
  htrig->Scale(1/Ntrig);
  htrig->Draw();
  htrig->SetXTitle(xtitle);
  htrig->GetXaxis()->CenterTitle();
  htrig->SetYTitle(ytitle);
  htrig->GetYaxis()->CenterTitle();
  htrig->Draw();

  cout<<"Trig\tFraction"<<endl;
  cout<<"============================="<<endl;
  for(ibin=1;ibin<=NTRIG+1;ibin++){
    xbin = htrig->GetBinCenter(ibin);
    xval = htrig->GetBinContent(ibin);
    if(xval>0) cout<<xbin<<"\t"<<xval<<endl;
  }
  cout<<"============================="<<endl<<endl;

  c1->cd(2);
  sprintf(xtitle,"Trigger Bit Combinations");

  sprintf(hname,"htcomb");
  sprintf(title,"Combinations");
  htcomb = new TH1F(hname,title,NCOMB+1,-0.5,NCOMB+0.5);
  t2->Project(hname,"ntrig","");
  
  Float_t Ntcomb = htcomb->Integral();
  htcomb->Scale(1/Ntcomb);
  htcomb->Draw();
  htcomb->SetXTitle(xtitle);
  htcomb->GetXaxis()->CenterTitle();
  htcomb->SetYTitle(ytitle);
  htcomb->GetYaxis()->CenterTitle();
  htcomb->Draw();

  c1->cd(3);
  sprintf(xtitle,"Trigger Bits");

  sprintf(hname,"htbit");
  sprintf(title,"tbit");
  htbit = new TH1F(hname,title,NBIT+1,-0.5,NBIT+0.5);
  t2->Project(hname,"tbit","");

  Float_t Ntbit = htbit->Integral();
  htbit->Scale(1/Ntbit);
  htbit->Draw();
  htbit->SetXTitle(xtitle);
  htbit->GetXaxis()->CenterTitle();
  htbit->SetYTitle(ytitle);
  htbit->GetYaxis()->CenterTitle();
  htbit->Draw();

  cout<<"Bit #\tFraction"<<endl;
  cout<<"============================="<<endl;
  for(ibin=1;ibin<=NBIT+1;ibin++){
    xbin = htbit->GetBinCenter(ibin);
    xval = htbit->GetBinContent(ibin);
    if(xval>0) cout<<xbin<<"\t"<<xval<<endl;
  }
  cout<<"============================="<<endl<<endl;

  c1->Print("stats_trig.gif");
  c1->Print("stats_trig.ps");

  //  f->Close();

}
