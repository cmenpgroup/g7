//IsLep_cuts.C
//
// 
// M. H. Wood
// University of South Carolina
//
// Notes: macro to study EC behavior versus momentum
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

int IsLep_cuts(char *fname="scanPID.root")
{
  gROOT->Reset();
  
  Int_t i, j, k;
  Int_t nmom = 200;
  Float_t moml = 0.0;        
  Float_t momh = 2.0;

  Int_t nec = 100;
  Float_t ecl = 0.0;        
  Float_t ech = 0.5;

  char xycuts[100];
  char qcuts[100];
  char lepcuts[100];
  char cuts[500];
  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle1[50];
  char ytitle2[50];

  TH2F *hECvsP;
  TH2F *hECoverP;

  sprintf(xtitle,"Momentum (GeV)");
  sprintf(ytitle1,"EC (GeV)");
  sprintf(ytitle2,"EC/P");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  // general cuts
  sprintf(xycuts,"(sqrt(vx*vx+vy*vy)<=1.2)");
  sprintf(qcuts,"(abs(Q)==1)");
  sprintf(lepcuts,"(abs(eCheckg7)==1)");

  sprintf(cuts,"%s && %s && %s",xycuts,qcuts,lepcuts);
  sprintf(hname,"hECvsP");
  sprintf(title,"Momentum vs EC");
  hECvsP = new TH2F(hname,title,nmom,moml,momh,nec,ecl,ech);
  t2->Project(hname,"EC:P",cuts);
  
  sprintf(hname,"hECoverP");
  sprintf(title,"Momentum vs EC/P");
  hECoverP = new TH2F(hname,title,nmom,moml,momh,nec,ecl,ech);
  t2->Project(hname,"EC/P:P",cuts);

  sprintf(cuts,"%s && %s && (CChit_stat>0)",xycuts,qcuts);
  sprintf(hname,"hNpe");
  sprintf(title,"Number of Photoelectrons");
  TH1F *hNpe = new TH1F(hname,title,100,0.0,50.0);
  t2->Project(hname,"CCnpe",cuts);

  sprintf(cuts,"%s && %s && (P<0.4)",xycuts,qcuts);
  sprintf(hname,"hMMsq");
  sprintf(title,"TOF MM2");
  TH1F *hMMsq = new TH1F(hname,title,100,-0.05,0.05);
  t2->Project(hname,"TOF_MassSq",cuts);

  gStyle->SetOptStat(0);

  TCanvas *c1= new TCanvas("c1","c1",0,0,800,800);
  hECvsP->Draw();
  hECvsP->SetXTitle(xtitle);
  hECvsP->GetXaxis()->CenterTitle();
  hECvsP->SetYTitle(ytitle1);
  hECvsP->GetYaxis()->CenterTitle();
  hECvsP->Draw("colz");
  c1->Print("ECvsP.gif");
   
  TCanvas *c2= new TCanvas("c2","c2",50,50,800,800);
  hECoverP->Draw();
  hECoverP->SetXTitle(xtitle);
  hECoverP->GetXaxis()->CenterTitle();
  hECoverP->SetYTitle(ytitle2);
  hECoverP->GetYaxis()->CenterTitle();
  hECoverP->Draw("colz");
  c2->Print("ECoverP.gif");

  TCanvas *c3= new TCanvas("c3","c3",100,100,800,800);
  gPad->SetLogy();
  hNpe->Draw();
  hNpe->SetXTitle("Number of PhotoElectrons");
  hNpe->GetXaxis()->CenterTitle();
  hNpe->SetYTitle("Counts");
  hNpe->GetYaxis()->CenterTitle();
  hNpe->SetFillColor(2);
  hNpe->Draw();
  c3->Print("Npe.gif");
  
  TCanvas *c4= new TCanvas("c4","c4",150,0,800,800);
  hMMsq->Draw();
  hMMsq->SetXTitle("TOF MM2");
  hMMsq->GetXaxis()->CenterTitle();
  hMMsq->SetYTitle("Counts");
  hMMsq->GetYaxis()->CenterTitle();
  hMMsq->SetFillColor(2);
  hMMsq->Draw();
  c4->Print("MMsq.gif");
  
  //  f->Close();

}
