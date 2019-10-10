//TestPiRejection.C
//
// macro to test pion/lepton rejection
// 
// M H Wood, University of Massachusetts, Amherst
//
// Notes:
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

void TestPiRejection(char *file="scanPID.root", Int_t tbit=4, Int_t tFlag=0)
{
  gROOT->Reset();
  char *fSame[5] = {"","same","same","same","same"};
  char *lepLabel[2] = {"e^{+}","e^{-}"};
  Int_t lcol[5] = {1,2,4,5,6};

  Int_t i, j;
  Int_t charge;
  Float_t max =0.0;

  Int_t ilogy = 0;
  Float_t zlo = -22.0;
  Float_t zhi = 5.0;
  Float_t thlo = 8.0;
  Float_t thhi = 50.0;
  Float_t rlim = 2.0;
  Float_t Plim = 0.5;
  Float_t Lmar = 0.125;
  Float_t Rmar = 0.125;
  Float_t yoff = 1.5;

  Int_t nPid = 16;
  Float_t Pidl = -0.5;
  Float_t Pidh = 15.5;

  char title[100];
  char xtitle[50];
  char ytitle[50];
  char hname[50];
  char cname[50];
  char ctitle[50];
  char item[50];
  char tName[50];
  
  char q_cut[100];
  char P_cut[100];
  char pid_cut[100];
  char th_cut[100];
  char vz_cut[100];
  char r_cut[100];
  char lep_cut[100];
  char trig_cut[100];
  char stdcuts[500];
  char cuts[500];

  TFile *f = new TFile(file,"READ");

  // set up the cuts
  if(tFlag){
    sprintf(tName,"incl%i",tbit);
    sprintf(trig_cut,"(trig&%i)",tbit);
  }else{
    sprintf(tName,"excl%i",tbit);
    sprintf(trig_cut,"(trig==%i)",tbit);
  }
  sprintf(vz_cut,"(vz>=%f && vz<=%f)",zlo,zhi);
  sprintf(th_cut,"(theta>=%f && theta<=%f)",thlo,thhi);
  sprintf(r_cut,"(sqrt(vx*vx+vy*vy)<=%f)",rlim);
  sprintf(P_cut,"(P>=%f)",Plim);
  sprintf(stdcuts,"%s && %s && %s && %s && %s",vz_cut,r_cut,trig_cut,th_cut,P_cut); 

  sprintf(xtitle,"Particle ID");
  sprintf(ytitle,"Counts");

  sprintf(cname,"c1");
  sprintf(ctitle,"g7 Data");
  TCanvas *c1 = new TCanvas(cname,ctitle,0,0,600,600);

  TH1F *hPid[2][2];
  TH1F *hPidRat[2];
  for(j=0;j<2;j++){
    charge = pow(-1,j);
    sprintf(q_cut,"(Q==%i)",charge);
    sprintf(lep_cut,"(IsLepG7==%i)",charge);
    for(i=0;i<2;i++){
      switch(i){
      case 0: 
	sprintf(cuts,"%s && %s",stdcuts,q_cut); 
	break;
      case 1: 
	sprintf(cuts,"%s && %s",stdcuts,lep_cut); 
	break;
      }
      sprintf(hname,"hPid_%i_%i",j,i);
      sprintf(title,"Particle ID, %s case %i, %5.2f<#theta<%5.2f",lepLabel[j],i,thlo,thhi);
      sprintf(item,"Pid");
      hPid[j][i] = new TH1F(hname,title,nPid,Pidl,Pidh);
      t2->Project(hname,item,cuts);
      
      //draw hist with vaious options
      gPad->SetLeftMargin(Lmar);
      gPad->SetRightMargin(Rmar);
      hPid[j][i]->SetLineWidth(2);
      hPid[j][i]->SetLineColor(lcol[i]);
      if(ilogy) gPad->SetLogy();
      hPid[j][i]->SetXTitle(xtitle);
      hPid[j][i]->GetXaxis()->CenterTitle();
      hPid[j][i]->SetYTitle(ytitle);
      hPid[j][i]->GetYaxis()->CenterTitle();
      hPid[j][i]->GetYaxis()->SetTitleOffset(yoff);
      hPid[j][i]->Draw(fSame[i+j]);
    }
    sprintf(hname,"hPidRat%i",j);
    hPidRat[j] = (TH1F*)hPid[j][1]->Clone(hname);
    sprintf(title,"Particle ID Ratio, %s, %5.2f<#theta<%5.2f",lepLabel[j],thlo,thhi);
    hPidRat[j]->SetTitle(title);
    hPidRat[j]->Divide(hPid[j][0]);
    if(hPidRat[j]->GetMaximum()>max) max = hPidRat[j]->GetMaximum();

    for(i=0;i<hPidRat[j]->GetNbinsX();i++){
      if(hPidRat[j]->GetBinContent(i+1)>0){
	cout<<hPidRat[j]->GetBinCenter(i+1)<<"\t"<<hPidRat[j]->GetBinContent(i+1)<<endl;
      }
    }
  }
  
  char OutCan[50];
  sprintf(OutCan,"TestPiRejection_%s_Pcut.gif",tName);
  c1->Print(OutCan);
  sprintf(OutCan,"TestPiRejection_%s_Pcut.eps",tName);
  c1->Print(OutCan);

  sprintf(cname,"c2");
  sprintf(ctitle,"g7 Data Ratio");
  TCanvas *c2 = new TCanvas(cname,ctitle,50,50,600,600);
  for(j=0;j<2;j++){
    gPad->SetLeftMargin(Lmar);
    gPad->SetRightMargin(Rmar);
    hPidRat[j]->SetLineWidth(2);
    hPidRat[j]->SetLineColor(lcol[0]);
    if(ilogy) gPad->SetLogy();
    hPidRat[j]->SetMaximum(max);
    hPidRat[j]->SetXTitle(xtitle);
    hPidRat[j]->GetXaxis()->CenterTitle();
    hPidRat[j]->SetYTitle(ytitle);
    hPidRat[j]->GetYaxis()->CenterTitle();
    hPidRat[j]->GetYaxis()->SetTitleOffset(yoff);
    hPidRat[j]->Draw(fSame[j]);
  }
  sprintf(OutCan,"TestPiRejection_ratio_%s_Pcut.gif",tName);
  c2->Print(OutCan);
  sprintf(OutCan,"TestPiRejection_ratio_%s_Pcut.eps",tName);
  c2->Print(OutCan);
}
