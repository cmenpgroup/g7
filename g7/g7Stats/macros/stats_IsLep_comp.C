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

int stats_IsLep_comp(char *fname="g7Stats.root", Int_t ntrks=2)
{
  gROOT->Reset();
  
  Int_t i, j;
  Int_t NLEP = 7;

  Float_t SumTOF, SumAll, SumECCC;

  char item[100];
  char hname[50];
  char title[50];
  char xtitle[50];
  char ytitle[50];

  char trk_cut[100];
  char Nep_cut[100];
  char Nem_cut[100];
  char cuts[500];
  sprintf(trk_cut,"Tracks>=%i",ntrks);
  sprintf(Nep_cut,"NposLep==1");
  sprintf(Nem_cut,"NnegLep==1");

  Float_t box[4][4] = {3.5,3.5,3.5,6.5,3.5,3.5,6.5,3.5,6.5,3.5,6.5,6.5,3.5,6.5,6.5,6.5};
  TLine *lin[4];
  for(i=0;i<4;i++){
    lin[i] = new TLine(box[i][0],box[i][1],box[i][2],box[i][3]);
    lin[i]->SetLineWidth(2);
    lin[i]->SetLineColor(2);
  }

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","c1",0,0,600,600);

  sprintf(xtitle,"e- Cuts : -1*(EC + 2*CC + 4*TOF)");
  sprintf(ytitle,"e+ Cuts : (EC + 2*CC + 4*TOF)");
  sprintf(item,"(posLepEC+2*posLepCC+4*posLepTOF):(-1*(negLepEC+2*negLepCC+4*negLepTOF))");

  sprintf(hname,"hIsLep1");
  sprintf(title,"e+/e- IsLeptonG7 Comp.");
  TH2F *hIsLep1  = new TH2F(hname,title,NLEP+1,-0.5,NLEP+0.5,NLEP+1,-0.5,NLEP+0.5);
  sprintf(cuts,"%s",trk_cut);
  t2->Project(hname,item,cuts);
  
  hIsLep1->Draw();
  hIsLep1->SetXTitle(xtitle);
  hIsLep1->GetXaxis()->CenterTitle();
  hIsLep1->SetYTitle(ytitle);
  hIsLep1->GetYaxis()->CenterTitle();
  hIsLep1->SetMarkerSize(1.5);
  hIsLep1->Draw("text");

  for(i=0;i<4;i++) lin[i]->Draw();

  SumTOF = hIsLep1->Integral(5,7,5,7); 
  SumAll = hIsLep1->Integral(4,8,4,8); 
  SumECCC = SumAll - SumTOF;
  cout<<"No cuts : ";
  cout<<"Sums - TOF="<<SumTOF<<" EC/CC="<<SumECCC<<endl;

  char tdtitle[50];
  TLatex ltexA[3];
  for(i=0;i<3;i++){
    if(i==0) sprintf(tdtitle,"N(All)=%7.0f",SumAll);
    if(i==1) sprintf(tdtitle,"N(EC/CC)=%7.0f",SumECCC);
    if(i==2) sprintf(tdtitle,"N(TOF)=%7.0f",SumTOF);
    ltexA[i]->SetTextSize(0.03);
    ltexA[i]->SetTextColor(4);
    ltexA[i]->DrawLatex(6.0,8.25-i*0.35,tdtitle);
  }

  c1->Print("stats_IsLep_compA.gif");
  c1->Print("stats_IsLep_compA.ps");

  TCanvas *c2 = new TCanvas("c2","c2",50,50,600,600);

  sprintf(hname,"hIsLep2");
  sprintf(title,"e+/e- IsLeptonG7 Comp. (N(e+)=N(e-)=1)");
  TH2F *hIsLep2  = new TH2F(hname,title,NLEP+1,-0.5,NLEP+0.5,NLEP+1,-0.5,NLEP+0.5);
  sprintf(cuts,"%s && %s && %s",trk_cut,Nep_cut,Nem_cut);
  t2->Project(hname,item,cuts);
  
  hIsLep2->Draw();
  hIsLep2->SetXTitle(xtitle);
  hIsLep2->GetXaxis()->CenterTitle();
  hIsLep2->SetYTitle(ytitle);
  hIsLep2->GetYaxis()->CenterTitle();
  hIsLep2->SetMarkerSize(1.5);
  hIsLep2->Draw("text");

  for(i=0;i<4;i++) lin[i]->Draw();

  SumTOF = hIsLep2->Integral(5,7,5,7); 
  SumAll = hIsLep2->Integral(4,8,4,8); 
  SumECCC = SumAll - SumTOF;
  cout<<"Cut on (N(e+)=N(e-)=1) : ";
  cout<<"Sums - TOF="<<SumTOF<<" EC/CC="<<SumECCC<<endl;

  TLatex ltexB[3];
  for(i=0;i<3;i++){
    if(i==0) sprintf(tdtitle,"N(All)=%7.0f",SumAll);
    if(i==1) sprintf(tdtitle,"N(EC/CC)=%7.0f",SumECCC);
    if(i==2) sprintf(tdtitle,"N(TOF)=%7.0f",SumTOF);
    ltexB[i]->SetTextSize(0.03);
    ltexB[i]->SetTextColor(4);
    ltexB[i]->DrawLatex(6.0,8.25-i*0.35,tdtitle);
  }

  c2->Print("stats_IsLep_compB.gif");
  c2->Print("stats_IsLep_compB.ps");

  //  f->Close();

}
