//Msq_beta.C
//
// 
// M. H. Wood
// University of South Carolina
//
// Notes: macro to study lepton id by momentum
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

gROOT->Reset();
  
void Test_Msq(char *fname="scanPID.root", Int_t iFlag=0)
{
  switch(iFlag){
  case 0: Msq_beta(fname); break;
  case 1: Msq_q_tgt(fname); break;
  case 2: Msq_stat(fname); break;
  case 3: momLep(fname); break;
  default: 
    cout<<"Wrong function flag selected: "<<iFlag<<endl;
    exit(-1);
    break;
  }
}

void Msq_beta(char *fname)
{
  Int_t i;

  Int_t nbeta = 300;
  Float_t betal = 0.6;        
  Float_t betah = 1.3;

  Int_t nMsq = 400;
  Float_t Msql = -0.05;        
  Float_t Msqh = 0.05;

  Float_t Ph = 0.4;

  char qcuts[500];
  char cuts[500];
  char hname[10];
  char title[50];
  char xtitle[50];
  char ytitle1[50];
  char ytitle2[50];

  sprintf(xtitle,"Mass Squared (GeV^{2})");
  sprintf(ytitle1,"counts");
  sprintf(ytitle2,"#beta");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  sprintf(qcuts,"Q!=0"); // only consider charged particles

  sprintf(title,"Q!=0");
  TH1F *hMsq1 = new TH1F("hMsq1",title,nMsq,Msql,Msqh);
  TH2F *hMsq_b1 = new TH2F("hMsq_b1",title,nMsq,Msql,Msqh,nbeta,betal,betah);
  sprintf(cuts,"%s",qcuts);
  t2->Project("hMsq1","TOF_MassSq",cuts);
  t2->Project("hMsq_b1","beta:TOF_MassSq",cuts);

  sprintf(title,"Q!=0 && P<%5.3f",Ph);
  TH1F *hMsq2 = new TH1F("hMsq2",title,nMsq,Msql,Msqh);
  TH2F *hMsq_b2 = new TH2F("hMsq_b2",title,nMsq,Msql,Msqh,nbeta,betal,betah);
  sprintf(cuts,"%s && (P < %f)",qcuts,Ph);
  t2->Project("hMsq2","TOF_MassSq",cuts);
  t2->Project("hMsq_b2","beta:TOF_MassSq",cuts);

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","Momentum Studies",0,0,800,800);
  c1->Divide(2,2);

  c1->cd(1);
  hMsq_b1->Draw("colz");
  hMsq_b1->SetXTitle(xtitle);
  hMsq_b1->GetXaxis()->CenterTitle();
  hMsq_b1->SetYTitle(ytitle2);
  hMsq_b1->GetYaxis()->CenterTitle();
  hMsq_b1->Draw("colz");

  c1->cd(2);
  hMsq_b2->Draw("colz");
  hMsq_b2->SetXTitle(xtitle);
  hMsq_b2->GetXaxis()->CenterTitle();
  hMsq_b2->SetYTitle(ytitle2);
  hMsq_b2->GetYaxis()->CenterTitle();
  hMsq_b2->Draw("colz");

  c1->cd(3);
  hMsq1->Draw();
  hMsq1->SetXTitle(xtitle);
  hMsq1->GetXaxis()->CenterTitle();
  hMsq1->SetYTitle(ytitle1);
  hMsq1->GetYaxis()->CenterTitle();
  hMsq1->Draw();
  
  c1->cd(4);
  hMsq2->Draw();
  hMsq2->SetXTitle(xtitle);
  hMsq2->GetXaxis()->CenterTitle();
  hMsq2->SetYTitle(ytitle1);
  hMsq2->GetYaxis()->CenterTitle();
  hMsq2->Draw();
  
  c1->Print("Msq_beta.gif");

}

void Msq_q_tgt(char *fname)
{
  Int_t i;

  Int_t nMsq = 400;
  Float_t Msql = -0.05;        
  Float_t Msqh = 0.05;

  Int_t nVz = 100;
  Float_t Vzl = -30.0;        
  Float_t Vzh = 20.0;

  Float_t Ph = 0.4;    // momentum limit
  Float_t vxyl = 1.2;  //cut on xy vertex

  char vzcuts[100];     // z vertex cuts cut
  char vxycuts[100];    // x,y vertex cuts cut
  char pQcuts[100];     // pos. charge cut
  char nQcuts[100];     // neg. charg cut
  char Pcuts[100];      // momentum cut
  char cuts[500];       // total cut
  char hname[100];      // histogram name
  char title[100];      // histogram title
  char xtitle[50];      // x-axis title
  char ytitle[50];      // y-axis title
  char target[50];      // target name
  char outgif[100];     // generic output gif filename

  TH1F *hMsqP[6];
  TH1F *hMsqN[6];

  sprintf(xtitle,"Mass Squared (GeV^{2})");
  sprintf(ytitle,"counts");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  // set up cuts
  sprintf(pQcuts,"Q==1"); // only consider positively-charged particles
  sprintf(nQcuts,"Q==-1"); // only consider negatively-charged particles
  sprintf(Pcuts,"(P<%f)",Ph); // momentum cuts
  sprintf(vxycuts,"(sqrt(vx*vx+vy*vy)<=%f)",vxyl); // xy-vertex cut

  // plot TOF mass squared versus z-vertex
  sprintf(cuts,"%s && %s && %s",Pcuts,nQcuts,vxycuts);
  TH2F *hMsqN_vz = new TH2F("hMsqN_vz","Q=-1",nVz,Vzl,Vzh,nMsq,Msql,Msqh);
  t2->Project("hMsqN_vz","TOF_MassSq:vz",cuts);

  sprintf(cuts,"%s && %s && %s",Pcuts,pQcuts,vxycuts);
  TH2F *hMsqP_vz = new TH2F("hMsqP_vz","Q=1",nVz,Vzl,Vzh,nMsq,Msql,Msqh);
  t2->Project("hMsqP_vz","TOF_MassSq:vz",cuts);

  for(i=0;i<6;i++){
    
    switch (i) {
      
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

    // Fill histograms for neg. particles
    sprintf(hname,"hMsqN%d",i);
    sprintf(title,"%s: Q=-1 && P < %5.3f",target,Ph);
    hMsqN[i] = new TH1F(hname,title,nMsq,Msql,Msqh);
    sprintf(cuts,"%s && %s && %s && %s",Pcuts,nQcuts,vzcuts,vxycuts);
    printf("%s Cuts %s \n",target,cuts);    // print cuts to the screen
    t2->Project(hname,"TOF_MassSq",cuts);
    
    // Fill histograms for pos. particles
    sprintf(hname,"hMsqP%d",i);
    sprintf(title,"%s: Q=1 && P < %5.3f",target,Ph);
    hMsqP[i] = new TH1F(hname,title,nMsq,Msql,Msqh);
    sprintf(cuts,"%s && %s && %s && %s",Pcuts,pQcuts,vzcuts,vxycuts);
    printf("%s Cuts %s \n",target,cuts);    // print cuts to the screen
    t2->Project(hname,"TOF_MassSq",cuts);

  }

  gStyle->SetOptStat(0);

  TCanvas *can = new TCanvas("can","Vertex vs Mass",500,0,500,700);
  can->Divide(1,2);
  can->cd(1);
  hMsqN_vz->Draw();
  hMsqN_vz->SetXTitle("z (cm)");
  hMsqN_vz->GetXaxis()->CenterTitle();
  hMsqN_vz->SetYTitle(xtitle);
  hMsqN_vz->GetYaxis()->CenterTitle();
  hMsqN_vz->Draw();
  can->cd(2);
  hMsqP_vz->Draw();
  hMsqP_vz->SetXTitle("z (cm)");
  hMsqP_vz->GetXaxis()->CenterTitle();
  hMsqP_vz->SetYTitle(xtitle);
  hMsqP_vz->GetYaxis()->CenterTitle();
  hMsqP_vz->Draw();

  sprintf(outgif,"Msq_q_tgt.gif");
  Ncan->Print(outgif);

  TCanvas *Ncan = new TCanvas("Ncan","Neg. Particles",0,0,700,900);
  Ncan->Divide(2,3);

  for(i=0;i<6;i++){
    Ncan->cd(i+1);
    hMsqN[i]->Draw();
    hMsqN[i]->SetXTitle(xtitle);
    hMsqN[i]->GetXaxis()->CenterTitle();
    hMsqN[i]->SetYTitle(ytitle);
    hMsqN[i]->GetYaxis()->CenterTitle();
    hMsqN[i]->Draw();
  }

  sprintf(outgif,"MsqN_tgt.gif");
  Ncan->Print(outgif);

  TCanvas *Pcan = new TCanvas("Pcan","Pos. Particles",300,0,700,900);
  Pcan->Divide(2,3);

  for(i=0;i<6;i++){
    Pcan->cd(i+1);
    hMsqP[i]->Draw();
    hMsqP[i]->SetXTitle(xtitle);
    hMsqP[i]->GetXaxis()->CenterTitle();
    hMsqP[i]->SetYTitle(ytitle);
    hMsqP[i]->GetYaxis()->CenterTitle();
    hMsqP[i]->Draw();
  }

  sprintf(outgif,"MsqP_tgt.gif");
  Pcan->Print(outgif);
}

void Msq_stat(char *fname)
{
  Int_t i;

  Int_t nMsq = 400;
  Float_t Msql = -0.05;        
  Float_t Msqh = 0.05;

  Float_t thl = 8.;
  Float_t thh = 60.;
  Float_t Ph = 0.4;

  char pQcuts[100];
  char nQcuts[100];
  char th1_cuts[100];
  char th2_cuts[100];
  char Pcuts[100];
  char TOFon[100];
  char ECon[100];
  char ECoff[100];
  char CCon[100];
  char CCoff[100];
  char cutsP[500];
  char hnameP[50];
  char titleP[100];
  char cutsN[500];
  char hnameN[50];
  char titleN[100];
  char xtitle[50];
  char ytitle[50];

  TH1F *hMsqP[6];
  TH1F *hMsqN[6];

  sprintf(xtitle,"Mass Squared (GeV^{2})");
  sprintf(ytitle,"counts");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  sprintf(pQcuts,"Q==1"); // only consider positively-charged particles
  sprintf(nQcuts,"Q==-1"); // only consider negatively-charged particles
  sprintf(TOFon,"TOFstat==1"); // TOF hit
  sprintf(ECon,"ECstat==1"); // EC hit
  sprintf(ECoff,"ECstat==0"); // no EC hit
  sprintf(CCon,"CCstat>0"); // CC hit
  sprintf(CCoff,"CCstat==0"); // no CC hit
  sprintf(th1_cuts,"(theta>=%f && theta<=%f)",thl,thh); // EC,CC theta cuts
  sprintf(th2_cuts,"(theta>%f)",thh); // theta cuts outside EC,CC
  sprintf(Pcuts,"(P<%f)",Ph); // momentum cuts

  for(Int_t i=0;i<6;i++){
    sprintf(hnameP,"hMsqP%i",i+1);
    sprintf(hnameN,"hMsqN%i",i+1);
    switch(i){
    case 0:
      sprintf(cutsP,"%s && %s",pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f",Ph);
      sprintf(cutsN,"%s && %s",nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f",Ph);
      break;
    case 1:
      sprintf(cutsP,"%s && %s && %s && %s",ECon,CCon,pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f && EC=1 && CC=1",Ph);
      sprintf(cutsN,"%s && %s && %s && %s",ECon,CCon,nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f && EC=1 && CC=1",Ph);
      break;
    case 2:
      sprintf(cutsP,"%s && %s && %s && %s",ECon,CCoff,pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f && EC=1 && CC=0",Ph);
      sprintf(cutsN,"%s && %s && %s && %s",ECon,CCoff,nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f && EC=1 && CC=0",Ph);
      break;
    case 3:
      sprintf(cutsP,"%s && %s && %s && %s",ECoff,CCon,pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f && EC=0 && CC=1",Ph);
      sprintf(cutsN,"%s && %s && %s && %s",ECoff,CCon,nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f && EC=0 && CC=1",Ph);
      break;
    case 4:
      sprintf(cutsP,"%s && %s && %s",th1_cuts,pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f && %6.2f<=#theta_{Lab}<=%6.2f",Ph,thl,thh);
      sprintf(cutsN,"%s && %s && %s",th1_cuts,nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f && %6.2f<=#theta_{Lab}<=%6.2f",Ph,thl,thh);
      break;
    case 5:
      sprintf(cutsP,"%s && %s && %s",th2_cuts,pQcuts,Pcuts);
      sprintf(titleP,"Q=1 && P<%5.3f && #theta_{Lab}>%6.2f",Ph,thh);
      sprintf(cutsN,"%s && %s && %s",th2_cuts,nQcuts,Pcuts);
      sprintf(titleN,"Q=-1 && P<%5.3f && #theta_{Lab}>%6.2f",Ph,thh);
      break;
    default:
      break;
    }
    hMsqP[i] = new TH1F(hnameP,titleP,nMsq,Msql,Msqh);
    t2->Project(hnameP,"TOF_MassSq",cutsP);
    hMsqN[i] = new TH1F(hnameN,titleN,nMsq,Msql,Msqh);
    t2->Project(hnameN,"TOF_MassSq",cutsN);
  }

  gStyle->SetOptStat(0);

  TCanvas *cP = new TCanvas("cP","Momentum Studies",0,0,700,900);
  cP->Divide(2,3);

  for(Int_t i=0;i<6;i++){
    cP->cd(i+1);
    hMsqP[i]->Draw();
    hMsqP[i]->SetXTitle(xtitle);
    hMsqP[i]->GetXaxis()->CenterTitle();
    hMsqP[i]->SetYTitle(ytitle);
    hMsqP[i]->GetYaxis()->CenterTitle();
    hMsqP[i]->Draw();
  } 
  cP->Print("MsqP_stat.gif");

  TCanvas *cN = new TCanvas("cN","Momentum Studies",0,300,700,900);
  cN->Divide(2,3);

  for(Int_t i=0;i<6;i++){
    cN->cd(i+1);
    hMsqN[i]->Draw();
    hMsqN[i]->SetXTitle(xtitle);
    hMsqN[i]->GetXaxis()->CenterTitle();
    hMsqN[i]->SetYTitle(ytitle);
    hMsqN[i]->GetYaxis()->CenterTitle();
    hMsqN[i]->Draw();
  } 
  cN->Print("MsqN_stat.gif");
}

void momLep(char *fname)
{
  Int_t i;

  Int_t nmom = 300;
  Float_t moml = 0.0;        
  Float_t momh = 1.5;

  Int_t nMsq = 400;
  Float_t Msql1 = -0.5;        
  Float_t Msqh1 = 1.5;
  Float_t Msql2 = -0.05;        
  Float_t Msqh2 = 0.05;

  Float_t betal = 0.9;
  Float_t Ph = 0.4;
  Float_t vzl = -22.0;  // z vertex lower limit
  Float_t vzh = 5.0;    // z vertex higher limit
  Float_t vxyl = 1.2;   // cut on xy vertex
  Float_t Msqlim = 0.0025;  //cut on TOF mass squared

  char qcuts[500];      // charge cut
  char vcuts[100];      // total vertex cuts
  char vzcuts[100];     // z vertex cuts
  char vxycuts[100];    // x,y vertex cuts
  char pQcuts[100];     // pos. charge cut
  char nQcuts[100];     // neg. charg cut
  char Pcuts[100];      // momentum cut
  char bcuts[100];      // beta cuts
  char Msqcuts[100];    // TOF mass squared cuts
  char eCh[100];        // IsLepton cuts
  char ECon[100];       // EC stat ON cuts
  char ECoff[100];      // EC stat OFF cuts
  char cuts[500];       // total cuts
  char hname[10];
  char title[50];
  char xtitle1[50];
  char ytitle1[50];
  char xtitle2[50];
  char ytitle2[50];

  sprintf(xtitle1,"Momentum (GeV)");
  sprintf(ytitle1,"counts");
  sprintf(xtitle2,"Mass Squared (GeV^{2})");
  sprintf(ytitle2,"counts");

  // data files contain the trees
  printf("Analyzing file %s\n",fname);  
  TFile *f = new TFile(fname,"READ");

  // set up cuts
  sprintf(pQcuts,"Q==1"); // only consider positively-charged particles
  sprintf(nQcuts,"Q==-1"); // only consider negatively-charged particles
  sprintf(Pcuts,"(P<%f)",Ph); // momentum cuts
  sprintf(bcuts,"(beta>%f)",betal); // beta cuts
  sprintf(qcuts,"Q!=0"); // only consider charged particles
  sprintf(eCh,"(abs(eCheck)==1)"); // IsLepton check
  sprintf(ECon,"(ECstat==1)"); // hit in EC
  sprintf(ECoff,"(ECstat==0)"); // no hit in EC
  sprintf(Msqcuts,"(abs(TOF_MassSq)<=%f)",Msqlim); // TOF mas squared cut
  sprintf(vxycuts,"(sqrt(vx*vx+vy*vy)<=%f)",vxyl); // xy-vertex cut
  sprintf(vzcuts,"(vz>=%f && vz<%f)",vzl,vzh); // z-vertex cut
  sprintf(vcuts,"%s && %s",vxycuts,vzcuts); // xyz-vertex cuts

  TH1F *htotp = new TH1F("htotp","Q!=0",nmom,moml,momh);
  sprintf(cuts,"%s && %s",qcuts,vcuts);
  t2->Project("htotp","P",cuts);

  sprintf(title,"#beta > %5.3f",betal);
  TH1F *hp1 = new TH1F("hp1",title,nmom,moml,momh);
  sprintf(cuts,"%s && %s && %s",qcuts,bcuts,vcuts);
  t2->Project("hp1","P",cuts);

  TH1F *hMsq = new TH1F("hMsq","Q!=0",nMsq,Msql1,Msqh1);
  t2->Project("hMsq","TOF_MassSq",cuts);

  sprintf(title,"Q!=0 && P < %5.3f",Ph);
  TH1F *hMsq1 = new TH1F("hMsq1",title,nMsq,Msql1,Msqh1);
  sprintf(cuts,"%s && %s && %s",qcuts,Pcuts,vcuts);
  t2->Project("hMsq1","TOF_MassSq",cuts);

  TH1F *hMsq2 = new TH1F("hMsq2",title,nMsq,Msql2,Msqh2);
  t2->Project("hMsq2","TOF_MassSq",cuts);

  TH1F *hMsq2a = new TH1F("hMsq2a",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s && %s",qcuts,Pcuts,Msqcuts,vcuts);
  t2->Project("hMsq2a","TOF_MassSq",cuts);

  TH1F *hMsq2b = new TH1F("hMsq2b",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s && %s && %s",qcuts,Pcuts,Msqcuts,ECoff,vcuts);
  t2->Project("hMsq2b","TOF_MassSq",cuts);

  TH1F *hMsq3a = new TH1F("hMsq3a",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s",qcuts,eCh,vcuts);
  t2->Project("hMsq3a","TOF_MassSq",cuts);

  TH1F *hMsq3b = new TH1F("hMsq3b",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s && %s",qcuts,eCh,ECon,vcuts);
  t2->Project("hMsq3b","TOF_MassSq",cuts);

  TH1F *hMsq3c = new TH1F("hMsq3c",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s && %s",qcuts,eCh,ECoff,vcuts);
  t2->Project("hMsq3c","TOF_MassSq",cuts);

  TH1F *hMsq4 = new TH1F("hMsq4",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && (beta > 0.95) && %s",qcuts,Pcuts,vcuts);
  t2->Project("hMsq4","TOF_MassSq",cuts);

  sprintf(title,"Q=-1 && P < %5.3f",Ph);
  TH1F *hMsqNeq = new TH1F("hMsqNeg",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s",nQcuts,Pcuts,vcuts);
  t2->Project("hMsqNeg","TOF_MassSq",cuts);

  sprintf(title,"Q=1 && P < %5.3f",Ph);
  TH1F *hMsqPos = new TH1F("hMsqPos",title,nMsq,Msql2,Msqh2);
  sprintf(cuts,"%s && %s && %s",pQcuts,Pcuts,vcuts);
  t2->Project("hMsqPos","TOF_MassSq",cuts);

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1","Momentum Studies",0,0,600,900);
  c1->Divide(1,2);

  c1->cd(1);
  htotp->Draw();
  htotp->SetXTitle(xtitle1);
  htotp->GetXaxis()->CenterTitle();
  htotp->SetYTitle(ytitle1);
  htotp->GetYaxis()->CenterTitle();
  htotp->Draw();

  c1->cd(2);
  hp1->Draw();
  hp1->SetXTitle(xtitle1);
  hp1->GetXaxis()->CenterTitle();
  hp1->SetYTitle(ytitle1);
  hp1->GetYaxis()->CenterTitle();
  hp1->Draw();

  c1->Print("momLep.gif");

  TCanvas *c2 = new TCanvas("c2","Momentum Studies",50,50,800,800);
  c2->Divide(2,2);

  c2->cd(1);
  hMsq->Draw();
  hMsq->SetXTitle(xtitle2);
  hMsq->GetXaxis()->CenterTitle();
  hMsq->SetYTitle(ytitle2);
  hMsq->GetYaxis()->CenterTitle();
  hMsq->Draw();

  c2->cd(2);
  hMsq1->Draw();
  hMsq1->SetXTitle(xtitle2);
  hMsq1->GetXaxis()->CenterTitle();
  hMsq1->SetYTitle(ytitle2);
  hMsq1->GetYaxis()->CenterTitle();
  hMsq1->Draw();

  c2->cd(3);
  hMsq2->Draw();
  hMsq2->SetXTitle(xtitle2);
  hMsq2->GetXaxis()->CenterTitle();
  hMsq2->SetYTitle(ytitle2);
  hMsq2->GetYaxis()->CenterTitle();
  hMsq2->Draw();
  hMsq2a->SetFillColor(2);
  hMsq2a->Draw("same");
  hMsq2b->SetFillColor(4);
  hMsq2b->Draw("same");
  
  c2->cd(4);
  hMsq2->Draw();
  hMsq2->SetXTitle(xtitle2);
  hMsq2->GetXaxis()->CenterTitle();
  hMsq2->SetYTitle(ytitle2);
  hMsq2->GetYaxis()->CenterTitle();
  hMsq2->Draw();
  hMsq3a->SetFillColor(2);
  hMsq3a->Draw("same");
  hMsq3b->SetFillColor(4);
  hMsq3b->Draw("same");
  hMsq3c->SetFillColor(3);
  hMsq3c->Draw("same");
  
  c2->Print("MsqLep.gif");

  TCanvas *c3 = new TCanvas("c3","Momentum Studies",100,100,700,500);
  hMsq2->Draw();
  hMsq2->SetXTitle(xtitle2);
  hMsq2->GetXaxis()->CenterTitle();
  hMsq2->SetYTitle(ytitle2);
  hMsq2->GetYaxis()->CenterTitle();
  hMsq2->Draw();
  hMsq4->SetLineColor(2);
  hMsq4->Draw("same");

  c3->Print("MsqLep_beta.gif");

  TCanvas *c4 = new TCanvas("c4","Momentum Studies",200,0,600,900);
  c4->Divide(1,2);

  c4->cd(1);
  hMsqNeg->Draw();
  hMsqNeg->SetXTitle(xtitle2);
  hMsqNeg->GetXaxis()->CenterTitle();
  hMsqNeg->SetYTitle(ytitle2);
  hMsqNeg->GetYaxis()->CenterTitle();
  hMsqNeg->Draw();

  c4->cd(2);
  hMsqPos->Draw();
  hMsqPos->SetXTitle(xtitle2);
  hMsqPos->GetXaxis()->CenterTitle();
  hMsqPos->SetYTitle(ytitle2);
  hMsqPos->GetYaxis()->CenterTitle();
  hMsqPos->Draw();

  c4->Print("momLep_q.gif");
}
