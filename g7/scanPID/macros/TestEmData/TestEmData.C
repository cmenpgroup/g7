//TestEmData.C
//
// 
// M. H. Wood
// University of Massachusetts, Amherst
//
// Notes: macro to test IsLepton with electron data
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

int TestEmData(char *fname="scanPID.root")
{
  gROOT->Reset();
  
  Int_t i;

  char islep_tof_true[100];
  char islep_tof_false[100];
  char islep_ec_true[100];
  char islep_ec_false[100];
  char islep_cc_true[100];
  char islep_cc_false[100];
  char pid_cuts[100];
  char cuts[500];

  char hname[30];
  char title[50];
  char xtitle[50];
  char ytitle[50];

  sprintf(pid_cuts,"Pid==3");
  sprintf(islep_tof_true,"eCheckG7mm2==-1");
  sprintf(islep_tof_false,"eCheckG7mm2==0");
  sprintf(islep_ec_true,"eCheckG7ec==-1");
  sprintf(islep_ec_false,"eCheckG7ec==0");
  sprintf(islep_cc_true,"eCheckG7cc==-1");
  sprintf(islep_cc_false,"eCheckG7cc==0");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","Particle ID",0,0,1000,500);
  c1->Divide(3,1);

  sprintf(hname,"hpart");
  sprintf(title,"PART Bank");
  TH1F *hpart = new TH1F(hname,title,16,-0.5,15.5);
  t2->Project(hname,"Pid","");

  sprintf(hname,"hevnt1");
  sprintf(title,"EVNT Bank (pdg)");
  TH1F *hevnt1 = new TH1F(hname,title,2601,-300.5,2300.5);
  t2->Project(hname,"Pid_EVNT_pdg","");

  sprintf(hname,"hevnt2");
  sprintf(title,"EVNT Bank (geant)");
  TH1F *hevnt2 = new TH1F(hname,title,16,-0.5,15.5);
  t2->Project(hname,"Pid_EVNT_geant","");

  sprintf(xtitle,"Particle ID");
  sprintf(ytitle,"counts");

  c1->cd(1);
  hpart->Draw();
  hpart->SetXTitle(xtitle);
  hpart->GetXaxis()->CenterTitle();
  hpart->SetYTitle(ytitle);
  hpart->GetYaxis()->CenterTitle();
  hpart->Draw();
  Float_t em1 = hpart->GetBinContent(4);

  c1->cd(2);
  hevnt1->Draw();
  hevnt1->SetXTitle(xtitle);
  hevnt1->GetXaxis()->CenterTitle();
  hevnt1->SetYTitle(ytitle);
  hevnt1->GetYaxis()->CenterTitle();
  hevnt1->Draw();

  c1->cd(3);
  hevnt2->Draw();
  hevnt2->SetXTitle(xtitle);
  hevnt2->GetXaxis()->CenterTitle();
  hevnt2->SetYTitle(ytitle);
  hevnt2->GetYaxis()->CenterTitle();
  hevnt2->Draw();
  Float_t em3 = hevnt2->GetBinContent(4);

  cout<<"Electron yield-> PART="<<em1<<" ,EVNT="<<em3<<endl;

  c1->Print("TestEmData_ylds.gif");

  TCanvas *c2 = new TCanvas("c2","TOF Information",50,50,500,800);
  c2->Divide(1,2);

  sprintf(cuts,"%s && %s",pid_cuts,islep_tof_true);
  sprintf(hname,"hmom_true");
  sprintf(title,"Momentum");
  TH1F *hmom_true = new TH1F(hname,title,250,0.0,2.5);
  t2->Project(hname,"P",cuts);

  sprintf(cuts,"%s && %s",pid_cuts,islep_tof_false);
  sprintf(hname,"hmom_false");
  TH1F *hmom_false = new TH1F(hname,title,250,0.0,2.5);
  t2->Project(hname,"P",cuts);

  sprintf(cuts,"%s && %s",pid_cuts,islep_tof_true);
  sprintf(hname,"hbvsP_true");
  sprintf(title,"Momentum vs #beta");
  TH2F *hbvsP_true = new TH2F(hname,title,250,0.0,2.5,200,0.4,1.2);
  t2->Project(hname,"beta:P",cuts);

  sprintf(cuts,"%s && %s",pid_cuts,islep_tof_false);
  sprintf(hname,"hbvsP_false");
  sprintf(title,"Momentum vs #beta");
  TH2F *hbvsP_false = new TH2F(hname,title,250,0.0,2.5,200,0.4,1.2);
  t2->Project(hname,"beta:P",cuts);

  sprintf(xtitle,"Momentum (GeV)");
  sprintf(ytitle,"counts");

  c2->cd(1);
  hmom_false->Draw();
  hmom_false->SetXTitle(xtitle);
  hmom_false->GetXaxis()->CenterTitle();
  hmom_false->SetYTitle(ytitle);
  hmom_false->GetYaxis()->CenterTitle();
  hmom_false->SetFillColor(2);
  hmom_false->Draw();

  hmom_true->SetFillColor(4);
  hmom_true->Draw("same");

  c2->cd(2);
  sprintf(ytitle,"#beta");
  hbvsP_true->Draw();
  hbvsP_true->SetXTitle(xtitle);
  hbvsP_true->GetXaxis()->CenterTitle();
  hbvsP_true->SetYTitle(ytitle);
  hbvsP_true->GetYaxis()->CenterTitle();
  hbvsP_true->SetMarkerColor(4);
  hbvsP_true->Draw();

  hbvsP_false->SetMarkerColor(2);
  hbvsP_false->Draw("same");

  c2->Print("TestEmData_tof.gif");

  TCanvas *c3 = new TCanvas("c3","EC Information",100,50,800,800);
  c3->Divide(2,2);

  TH2F *hECvsP[6], *hECoverP[6];

  for(i=0;i<6;i++){
    if(i==0) sprintf(cuts,"%s && %s",pid_cuts,islep_ec_true);
    if(i==1) sprintf(cuts,"%s && %s",pid_cuts,islep_ec_false);
    if(i==2) sprintf(cuts,"%s && %s && %s",pid_cuts,islep_ec_true,islep_cc_true);
    if(i==3) sprintf(cuts,"%s && %s && %s",pid_cuts,islep_ec_true,islep_cc_false);
    if(i==4) sprintf(cuts,"%s && %s && %s",pid_cuts,islep_ec_false,islep_cc_true);
    if(i==5) sprintf(cuts,"%s && %s && %s",pid_cuts,islep_ec_false,islep_cc_false);

    sprintf(title,"Momentum vs EC Energy");
    sprintf(hname,"hECvsP%i",i+1);
    hECvsP[i] = new TH2F(hname,title,250,0.0,2.5,200,0.0,0.8);
    t2->Project(hname,"EC:P",cuts);

    sprintf(title,"Momentum vs EC/P");
    sprintf(hname,"hECoverP%i",i+1);
    hECoverP[i] = new TH2F(hname,title,250,0.0,2.5,200,0.0,0.8);
    t2->Project(hname,"EC/P:P",cuts);
  }

  sprintf(xtitle,"Momentum (GeV)");
  sprintf(ytitle,"EC Energy (GeV)");

  c3->cd(1);
  hECvsP[0]->Draw();
  hECvsP[0]->SetXTitle(xtitle);
  hECvsP[0]->GetXaxis()->CenterTitle();
  hECvsP[0]->SetYTitle(ytitle);
  hECvsP[0]->GetYaxis()->CenterTitle();
  hECvsP[0]->SetMarkerColor(4);
  hECvsP[0]->Draw();

  hECvsP[1]->SetMarkerColor(2);
  hECvsP[1]->Draw("same");

  c3->cd(2);
  hECvsP[2]->Draw();
  hECvsP[2]->SetXTitle(xtitle);
  hECvsP[2]->GetXaxis()->CenterTitle();
  hECvsP[2]->SetYTitle(ytitle);
  hECvsP[2]->GetYaxis()->CenterTitle();
  hECvsP[2]->SetMarkerColor(4);
  hECvsP[2]->Draw();

  hECvsP[3]->SetMarkerColor(3);
  hECvsP[3]->Draw("same");
  hECvsP[4]->SetMarkerColor(5);
  hECvsP[4]->Draw("same");
  hECvsP[5]->SetMarkerColor(2);
  hECvsP[5]->Draw("same");

  c3->cd(3);
  sprintf(ytitle,"EC/P");
  hECoverP[0]->Draw();
  hECoverP[0]->SetXTitle(xtitle);
  hECoverP[0]->GetXaxis()->CenterTitle();
  hECoverP[0]->SetYTitle(ytitle);
  hECoverP[0]->GetYaxis()->CenterTitle();
  hECoverP[0]->SetMarkerColor(4);
  hECoverP[0]->Draw();

  hECoverP[1]->SetMarkerColor(2);
  hECoverP[1]->Draw("same");

  c3->cd(4);
  hECoverP[2]->Draw();
  hECoverP[2]->SetXTitle(xtitle);
  hECoverP[2]->GetXaxis()->CenterTitle();
  hECoverP[2]->SetYTitle(ytitle);
  hECoverP[2]->GetYaxis()->CenterTitle();
  hECoverP[2]->SetMarkerColor(4);
  hECoverP[2]->Draw();

  hECoverP[3]->SetMarkerColor(3);
  hECoverP[3]->Draw("same");
  hECoverP[4]->SetMarkerColor(5);
  hECoverP[4]->Draw("same");
  hECoverP[5]->SetMarkerColor(2);
  hECoverP[5]->Draw("same");

  c3->Print("TestEmData_ec.gif");

  //  f->Close();
}
