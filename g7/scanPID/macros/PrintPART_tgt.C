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

int PrintPART_tgt(char *file="scanPID.root",Int_t itgt=5,Int_t maxpid=16)
{
  gROOT->Reset();

  Int_t yld,qsum;

  char name[10];
  char target[10];
  char title[20];

  char vxycuts[100];  // xy vertex cuts
  char vzcuts[100];     // z vertex cuts cut
  char ecuts[100];   // photon energy cuts
  char cuts[500];
  char acuts[500];

  Float_t elow=0.0;  //cut on egam
  Float_t ehi=6.0;
  Float_t vxyl = 1.2;  //cut on xy vertex

  TFile *f = new TFile(file,"READ");

  TH1F *hp[50];

  FILE *fout;
  char fname[30];

  // set up the cuts
  sprintf(ecuts,"(egam> %f && egam< %f)",elow,ehi);
  sprintf(vxycuts,"(sqrt(vx*vx + vy*vy)<= %f)",vxyl);

  switch (itgt) {
    
  case 0:
    sprintf(target,"D2");
    sprintf(vzcuts,"(vz>=-22.0 && vz<-14.0)");
    break;
  case 1:
    sprintf(target,"C12");
    sprintf(vzcuts,"((vz>-14.0 && vz<-11.0) || (vz>-8.5 && vz<-6.0) || (vz>-3.5 && vz<-1.0) || (vz>1.5 && vz<4.0))");
    break;
  case 2:
    sprintf(target,"Ti48");
    sprintf(vzcuts,"((vz>-1.0) && (vz<1.5))");
    break;
  case 3:
    sprintf(target,"Fe56");
    sprintf(vzcuts,"((vz>-11.0) && (vz<-8.5))");
    break;
  case 4:
    sprintf(target,"Pb208");
    sprintf(vzcuts,"((vz>-6.0) && (vz<-3.5))");
    break;
  case 5:
    sprintf(target,"All");
    sprintf(vzcuts,"(vz>=-22.0 && vz<5.0)");
    break;
  default:
    cout << "Unrecognized target number: " << i << endl;;
    sprintf(target,"Unknown");
    sprintf(vzcuts,"(vz<-22.0)");
    break;
  }

  sprintf(cuts,"%s && %s && %s",vxycuts,ecuts,vzcuts);

  sprintf(fname,"PrintPART_%s.dat",target);
  fout = fopen(fname,"w");

  fprintf(fout,"\tPART summary for %s\n",target);
  fprintf(fout,"--------------------------------\n");

  sprintf(acuts,"%s && (Q>0)",cuts);
  TH1F *hpos=new TH1F("hpos","pos charge",15,0.,1.5);
  t2->Project("hpos","Q",acuts);

  fprintf(fout,"q>0\t%i\n",hpos->Integral());

  sprintf(acuts,"%s && (Q<0)",cuts);
  TH1F *hneg=new TH1F("hneg","neg charge",15,-1.5,0.);
  t2->Project("hneg","Q",acuts);

  fprintf(fout,"q<0\t%i\n",hneg->Integral());

  sprintf(acuts,"%s && (Q==0)",cuts);
  TH1F *hneut=new TH1F("hneut","neut charge",11,-0.5,0.5);
  t2->Project("hneut","Q",acuts);

  fprintf(fout,"q=0\t%i\n",hneut->Integral());

  qsum = hpos->Integral() +  hneg->Integral() + hneut->Integral();

  fprintf(fout,"--------------------------------\n");
  fprintf(fout,"Total\t%i\n\n",qsum);
  
  for(Int_t i=0;i<maxpid;i++){
    sprintf(name,"hp%d",i);
    sprintf(title,"part type %d",i);
    hp[i] = new TH1F(name,title,11,-0.5+i,0.5+i);

    sprintf(acuts,"%s && (Pid==%d)",cuts,i);
    printf("%s\n",acuts);
    t2->Project(name,"Pid",acuts);

    yld = hp[i]->Integral();
    fprintf(fout,"pid = %i\t%i\n",i,yld);
  }
  //  f->Close();
}
