//PrintPART_tgt.C
//
// macro to print PID yields for each g7 target
//
// 
// M H Wood, University of South Carolina
//
// Notes:
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

int PrintStats(char *file="scanPID.root",Int_t maxpid=16)
{
  gROOT->Reset();

  Int_t yld,qsum;
  Int_t maxTrks = 10;

  char name[10];
  char title[20];

  TFile *f = new TFile(file,"READ");

  char fname[30];

  // set up the cuts
  sprintf(fname,"PrintStats.dat");

  cout<<"\tPART summary"<<endl;
  cout<<"--------------------------------"<<endl;

  TH1F *hQ=new TH1F("hQ","charge",5,-2.5,2.5);
  t2->Project("hQ","Q","");

  cout<<"q<0\t"<<hQ->Integral(2,2)<<endl;
  cout<<"q=0\t"<<hQ->Integral(3,3)<<endl;
  cout<<"q>0\t"<<hQ->Integral(4,4)<<endl;
  
  qsum = hQ->Integral(2,4);

  cout<<"--------------------------------\n"<<endl;
  cout<<"Total\t"<<qsum<<endl;
  
  sprintf(name,"hTrks");
  sprintf(title,"Number of tracks per event");
  TH1F *hTrks = new TH1F(name,title,maxTrks+1,-0.5,maxTrks+0.5);
  t2->Project(name,"Tracks","");
  
  for(Int_t i=0;i<maxTrks;i++){
    yld = hTrks->Integral(i+1,i+1);
    cout<<"Num Tracks "<<i<<"\t"<<yld<<endl;
  }

  sprintf(name,"hp");
  sprintf(title,"part type");
  TH1F *hp = new TH1F(name,title,maxpid+1,-0.5,0.5+maxpid);
  t2->Project(name,"Pid","");
  
  for(Int_t i=0;i<maxpid;i++){
    yld = hp->Integral(i+1,i+1);
    cout<<"pid = "<<i<<"\t"<<yld<<endl;
  }

  sprintf(name,"hlep");
  sprintf(title,"IsLeptonG7");
  TH1F *hlep = new TH1F(name,title,5,-2.5,2.5);
  t2->Project(name,"IsLepG7","");
  
  for(Int_t i=0;i<5;i++){
    yld = hlep->Integral(i+1,i+1);
    cout<<"Bin Num = "<<i<<"\t"<<yld<<endl;
  }

  //  f->Close();
}
