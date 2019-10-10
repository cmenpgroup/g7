//beta_P.C
//
// 
// M. H. Wood
// University of South Carolina
//
// Notes: macro to study beta vs momentum
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

gROOT->Reset();

Int_t nbeta = 240;
Float_t betal = 0.0;        
Float_t betah = 1.2;

Int_t nmom = 300;
Float_t moml = 0.0;      
Float_t momh = 1.5;

char pid_cuts[100];
char eCh_cuts[100];
char cuts[500];
char hname[50];
char title[50];
char xtitle[50];
char ytitle[50];

void Test_Beta_P(char *fname="scanPID.root", Int_t iFlag=0)
{
  switch(iFlag){
  case 0: beta_P(fname); break;
  case 1: beta_PxQ(fname); break;
  default: 
    cout<<"Wrong function flag selected: "<<iFlag<<endl;
    exit(-1);
    break;
  }
}

void beta_P(char *fname="scanPID.root")
{
  Int_t i;

  sprintf(xtitle,"Momentum (GeV)");
  sprintf(ytitle,"#beta");

  TH2F *hbvsp_pid[4];
  TH2F *hbvsp_pid_eCh[4];

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  TH2F *hbvsp = new TH2F("hbvsp","No cuts",nmom,moml,momh,nbeta,betal,betah);
  t2->Project("hbvsp","beta:P","");

  for(i=0;i<4;i++){
    if(i==0){
      sprintf(pid_cuts,"(Pid==2 || Pid==3)");
      sprintf(title,"Pid=2 or 3");
    }
    if(i==1){
      sprintf(pid_cuts,"(Pid==8 || Pid==9)");
      sprintf(title,"Pid=8 or 9");
    }
    if(i==2){
      sprintf(pid_cuts,"(Pid==11 || Pid==12)");
      sprintf(title,"Pid=11 or 12");
    }
    if(i==3){
      sprintf(pid_cuts,"(Pid==14 || Pid==15)");
      sprintf(title,"Pid=14 or 15");
    }
    sprintf(hname,"hbvsp_pid%i",i+1);
    hbvsp_pid[i] = new TH2F(hname,title,nmom,moml,momh,nbeta,betal,betah);
    t2->Project(hname,"beta:P",pid_cuts);
  }

  for(i=0;i<4;i++){
    if(i==0){
      sprintf(pid_cuts,"(Pid==2 || Pid==3)");
      sprintf(title,"Pid=2 or 3 && IsLepton");
    }
    if(i==1){
      sprintf(pid_cuts,"(Pid==8 || Pid==9)");
      sprintf(title,"Pid=8 or 9 && IsLepton");
    }
    if(i==2){
      sprintf(pid_cuts,"(Pid==11 || Pid==12)");
      sprintf(title,"Pid=11 or 12 && IsLepton");
    }
    if(i==3){
      sprintf(pid_cuts,"(Pid==14 || Pid==15)");
      sprintf(title,"Pid=14 or 15 && IsLepton");
    }
    sprintf(cuts,"%s && (abs(eCheck)==1)",pid_cuts);

    sprintf(hname,"hbvsp_pid_eCh%i",i+1);
    hbvsp_pid_eCh[i] = new TH2F(hname,title,nmom,moml,momh,nbeta,betal,betah);
    t2->Project(hname,"beta:P",cuts);
  }

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","Photon Timing",0,0,600,600);

  gPad->SetLogz();
  hbvsp->Draw();
  hbvsp->SetXTitle(xtitle);
  hbvsp->GetXaxis()->CenterTitle();
  hbvsp->SetYTitle(ytitle);
  hbvsp->GetYaxis()->CenterTitle();
  hbvsp->Draw("colz");

  c1->Print("beta_P.gif");
  
  TCanvas *c2 = new TCanvas("c2","c2",50,50,700,700);
  c2->Divide(2,2);

  for(i=0;i<4;i++){
    c2->cd(i+1);
    gPad->SetLogz();
    hbvsp_pid[i]->Draw();
    hbvsp_pid[i]->SetXTitle(xtitle);
    hbvsp_pid[i]->GetXaxis()->CenterTitle();
    hbvsp_pid[i]->SetYTitle(ytitle);
    hbvsp_pid[i]->GetYaxis()->CenterTitle();
    hbvsp_pid[i]->Draw("colz");
  }

  c2->Print("beta_P_pid.gif");
  
  TCanvas *c3 = new TCanvas("c3","c3",100,100,700,700);
  c3->Divide(2,2);

  for(i=0;i<4;i++){
    c3->cd(i+1);
    gPad->SetLogz();
    hbvsp_pid_eCh[i]->Draw();
    hbvsp_pid_eCh[i]->SetXTitle(xtitle);
    hbvsp_pid_eCh[i]->GetXaxis()->CenterTitle();
    hbvsp_pid_eCh[i]->SetYTitle(ytitle);
    hbvsp_pid_eCh[i]->GetYaxis()->CenterTitle();
    hbvsp_pid_eCh[i]->Draw("colz");
  }

  c3->Print("beta_P_pid_eCh.gif");

}

void beta_PxQ(char *fname="scanPID.root")
{
  Int_t i;

  sprintf(xtitle,"Momentum*Charge (GeV)");
  sprintf(ytitle,"#beta");

  TH2F *hbvsp_pid[2];
  TH2F *hbvsp_pid_eCh[2];

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  TH2F *hbvsp = new TH2F("hbvsp","Q!=0",nmom,-momh,momh,nbeta,betal,betah);
  t2->Project("hbvsp","beta:P*Q","Q!=0");

  for(i=0;i<2;i++){
    if(i==0){
      sprintf(pid_cuts,"(Pid<=3)");
      sprintf(title,"e+/e-");
    }
    if(i==1){
      sprintf(pid_cuts,"(Pid>3)");
      sprintf(title,"Hadrons");
    }
    sprintf(hname,"hbvsp_pid%i",i+1);
    hbvsp_pid[i] = new TH2F(hname,title,nmom,-momh,momh,nbeta,betal,betah);
    sprintf(cuts,"%s && (Q!=0)",pid_cuts);
    t2->Project(hname,"beta:P*Q",cuts);
  }

  for(i=0;i<2;i++){
    if(i==0){
      sprintf(pid_cuts,"(Pid<=3)");
      sprintf(title,"e+/e- && IsLepton");
    }
    if(i==1){
      sprintf(pid_cuts,"(Pid>3)");
      sprintf(title,"Hadrons && IsLepton");
    }
    sprintf(cuts,"%s && (abs(eCheck)==1) && (Q!=0)",pid_cuts);

    sprintf(hname,"hbvsp_pid_eCh%i",i+1);
    hbvsp_pid_eCh[i] = new TH2F(hname,title,nmom,-momh,momh,nbeta,betal,betah);
    t2->Project(hname,"beta:P*Q",cuts);
  }

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","beta vs PxQ",0,0,600,600);

  hbvsp->Draw();
  hbvsp->SetXTitle(xtitle);
  hbvsp->GetXaxis()->CenterTitle();
  hbvsp->SetYTitle(ytitle);
  hbvsp->GetYaxis()->CenterTitle();
  hbvsp->Draw("colz");

  c1->Print("beta_PxQ.gif");
  
  TCanvas *c2 = new TCanvas("c2","beta vs PxQ",50,50,900,900);
  c2->Divide(2,2);

  for(i=0;i<2;i++){
    c2->cd(i+1);
    hbvsp_pid[i]->Draw();
    hbvsp_pid[i]->SetXTitle(xtitle);
    hbvsp_pid[i]->GetXaxis()->CenterTitle();
    hbvsp_pid[i]->SetYTitle(ytitle);
    hbvsp_pid[i]->GetYaxis()->CenterTitle();
    hbvsp_pid[i]->Draw("colz");
  }

  for(i=0;i<2;i++){
    c2->cd(i+3);
    hbvsp_pid_eCh[i]->Draw();
    hbvsp_pid_eCh[i]->SetXTitle(xtitle);
    hbvsp_pid_eCh[i]->GetXaxis()->CenterTitle();
    hbvsp_pid_eCh[i]->SetYTitle(ytitle);
    hbvsp_pid_eCh[i]->GetYaxis()->CenterTitle();
    hbvsp_pid_eCh[i]->Draw("colz");
  }

  c2->Print("beta_PxQ_pid.gif");

}
