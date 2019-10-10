//Test_EC.C
//
// 
// M. H. Wood
// University of Massachusetts, AMherst
//
// Notes: macro to study EC cuts for electron ID for scanPID.root
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

gROOT->Reset();

Float_t Lmar = 0.15;
Float_t Rmar = 0.1;
Float_t yoff = 1.75;

void Test_LAC(char *fname="scanPID.root",Int_t charge=1)
{
  Int_t i;

  Float_t zlo = -22.0;
  Float_t zhi = 5.0;

  Int_t nP = 250;
  Float_t Pl = 0.0;
  Float_t Ph = 1.5;

  Int_t nLAC = 100;
  Float_t LACl = 0.0;
  Float_t LACh = 0.5;

  Int_t nLAC_P = 100;
  Float_t LAC_Pl = 0.0;
  Float_t LAC_Ph = 0.5;

  Int_t nLACi = 100;
  Float_t LACil = 0.0;
  Float_t LACih = 0.2;

  Int_t nLACo = 100;
  Float_t LACol = 0.0;
  Float_t LACoh = 0.15;

  Int_t nLACit = 240;
  Float_t LACitl = 0.0;
  Float_t LACith = 1.2;

  char stat_cut[100];
  char q_cut[100];
  char vz_cut[100];
  char pid_cut[100];
  char sec_cut[100];
  char ec_xy_cut[100];
  char cuts[500];
  char cuts1[500];
  char cuts2[500];
  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle[50];
  char item[50];

  char OutCan[50];

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  sprintf(stat_cut,"LACstat>0");
  sprintf(q_cut,"Q==%i",charge);
  sprintf(vz_cut,"(vz>=%f && vz<=%f)",zlo,zhi);
  sprintf(cuts,"%s && %s && %s",vz_cut,q_cut,stat_cut);

  TCanvas *c1 = new TCanvas("c1","c1",0,0,800,800);
  c1->Divide(2,2);

  gStyle->SetOptStat(0);

  c1->cd(1);
  gPad->SetLeftMargin(Lmar);
  gPad->SetRightMargin(Rmar);
  gPad->SetFillColor(0);
  sprintf(hname,"hLACvsP");
  sprintf(title,"LAC vs P, Q=%i",charge);
  sprintf(item,"LAC:P");
  TH2F *hLACvsP = new TH2F(hname,title,nP,Pl,Ph,nLAC,LACl,LACh);
  t2->Project(hname,item,cuts);
  
  sprintf(xtitle,"P (GeV)");
  sprintf(ytitle,"LAC");

  hLACvsP->Draw();
  hLACvsP->SetXTitle(xtitle);
  hLACvsP->GetXaxis()->CenterTitle();
  hLACvsP->SetYTitle(ytitle);
  hLACvsP->GetYaxis()->CenterTitle();
  hLACvsP->Draw("colz");

  TF1 *ecf1 = new TF1("ecf1","0.23*(x-0.3)+0.071*(x-0.3)*(x-0.3)-0.032*(x-0.3)*(x-0.3)*(x-0.3)",0.1,1.3);
  ecf1->SetLineColor(2);
  ecf1->Draw("same");
  TF1 *ecf2 = new TF1("ecf2","0.272*(x-0.3)",1.2,Ph);
  ecf2->SetLineColor(2);
  ecf2->Draw("same");

  c1->cd(2);
  gPad->SetLeftMargin(Lmar);
  gPad->SetRightMargin(Rmar);
  gPad->SetFillColor(0);
  sprintf(hname,"hLACoverP");
  sprintf(title,"LAC/P vs P, Q=%i",charge);
  sprintf(item,"LAC/P:P");
  TH2F *hLACoverP = new TH2F(hname,title,nP,Pl,Ph,nLAC_P,LAC_Pl,LAC_Ph);
  t2->Project(hname,item,cuts);
  
  sprintf(xtitle,"P (GeV)");
  sprintf(ytitle,"LAC/P");

  hLACoverP->Draw();
  hLACoverP->SetXTitle(xtitle);
  hLACoverP->GetXaxis()->CenterTitle();
  hLACoverP->SetYTitle(ytitle);
  hLACoverP->GetYaxis()->CenterTitle();
  hLACoverP->Draw("colz");

  TF1 *ecf3 = new TF1("ecf3","0.23+0.071*(x-0.3)-0.032*(x-0.3)*(x-0.3)",0.1,1.3);
  ecf3->SetLineColor(2);
  //  ecf3->Draw("same");
  TF1 *ecf4 = new TF1("ecf4","0.272",1.2,Ph);
  ecf4->SetLineColor(2);
  //  ecf4->Draw("same");

  c1->cd(3);
  gPad->SetLeftMargin(Lmar);
  gPad->SetRightMargin(Rmar);
  gPad->SetFillColor(0);
  sprintf(hname,"hLACi_LAC");
  sprintf(title,"LACin/LAC vs P, Q=%i",charge);
  sprintf(item,"LACin/LAC:P");
  TH2F *hLACi_LAC = new TH2F(hname,title,nP,Pl,Ph,nLACit,LACitl,LACith);
  t2->Project(hname,item,cuts);

  sprintf(xtitle,"P (GeV)");
  sprintf(ytitle,"LACin/LAC");

  hLACi_LAC->Draw();
  hLACi_LAC->SetXTitle(xtitle);
  hLACi_LAC->GetXaxis()->CenterTitle();
  hLACi_LAC->SetYTitle(ytitle);
  hLACi_LAC->GetYaxis()->CenterTitle();
  hLACi_LAC->Draw("colz");

  c1->cd(4);
  gPad->SetLeftMargin(Lmar);
  gPad->SetRightMargin(Rmar);
  gPad->SetFillColor(0);
  sprintf(hname,"hLACi_LACo");
  sprintf(title,"LACin vs LACout, Q=%i",charge);
  sprintf(item,"LAC-LACin:LACin");
  TH2F *hLACi_LACo = new TH2F(hname,title,nLACi,LACil,LACih,nLACo,LACol,LACoh);
  t2->Project(hname,item,cuts);
  
  sprintf(xtitle,"LACin");
  sprintf(ytitle,"LACout");

  hLACi_LACo->Draw();
  hLACi_LACo->SetXTitle(xtitle);
  hLACi_LACo->GetXaxis()->CenterTitle();
  hLACi_LACo->SetYTitle(ytitle);
  hLACi_LACo->GetYaxis()->CenterTitle();
  hLACi_LACo->Draw("colz");

  char qLabel[20];
  if(charge>0){
    sprintf(qLabel,"pos");
  }else if(charge<0){
    sprintf(qLabel,"neg");
  }else{
    sprintf(qLabel,"neutral");
  }

  sprintf(OutCan,"Test_LAC_%s.gif",qLabel);
  c1->Print(OutCan);
  sprintf(OutCan,"Test_LAC_%s.eps",qLabel);
  c1->Print(OutCan);

  return;

}

