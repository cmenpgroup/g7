//ECCC_final.C
//
// 
// M. H. Wood
// University of Massachusetts, Amherst
//
// Notes: macro to make a pretty plot of EC or CC before and 
//        after cuts
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

gROOT->Reset();

Float_t q;

Float_t Lmar = 0.125;
Float_t Rmar = 0.125;
Float_t yoff = 1.5;

Int_t nP = 150;
Float_t Pl = 0.0;
Float_t Ph = 2.5;

Int_t nEC = 200;
Float_t ECl = 0.0;
Float_t ECh = 1.0;

Int_t nECP = 100;
Float_t ECPl = 0.0;
Float_t ECPh = 0.5;

Int_t nECi = 100;
Float_t ECil = 0.01;
Float_t ECih = 0.51;

Int_t nECo = 150;
Float_t ECol = 0.01;
Float_t ECoh = 0.31;

Int_t nCCnpe = 101;
Float_t CCnpel = -1.0;
Float_t CCnpeh = 50.0;

char item1[50];
char item2[50];
char item3[50];
char xycuts[100];
char qcuts[100];
char eccuts[100];
char cccuts[100];
char ecin_mip_cut[100];
char islep_cut[100];
char islepG7_cut[100];
char islep_ec_cut[100];
char islep_ect_cut[100];
char islep_cc_cut[100];
char islep_tof_cut[100];
char cuts[500];
char hname[50];
char plabel[50];
char title[50];
char cname[50];
char ctitle[50];
char xtitle1[50];
char ytitle1[50];
char xtitle2[50];
char ytitle2[50];
char ytitle3[50];

Int_t lcol[5] = {1,2,4,5,6};
char *fSame[3] = {"","same","same"};

// declare the chain
TChain *chain = new TChain("t2","chain");

void ECCC_final(char *fname="scanPID.lis", Int_t sFlag=0, Int_t iFlag=0)
{
  
  Int_t ncols;
  Int_t nfiles = 0;
  char rootFile[500];
  FILE *in2 = fopen(fname,"r");
  while (1) {
    ncols = fscanf(in2,"%s",rootFile);
    if (ncols<0) break;
    chain->Add(rootFile);
    nfiles++;
    if(!(nfiles % 100)) cerr << nfiles << "\r";
  }
  fclose(in2);
  cerr<<"Total number of files analyzed = "<<nfiles<<endl;

  switch(sFlag){
  case 0: EC_final(fname,iFlag); break;
  case 1: CC_final(fname,iFlag); break;
  case 2: CC_comp(fname); break;
  default: 
    cout<<"Unknown flag "<<sFlag<<". Try again, please"<<endl; 
    exit; 
    break;
  }
}

void EC_final(char *fname, Int_t iFlag)
{
  
  Int_t i, j, k;

  TH2F *hECvsP[2];
  TH2F *hECPvsP[2];
  TH2F *hECio[2];

  sprintf(item1,"EC:P");
  sprintf(item2,"ECout:ECin");
  sprintf(item3,"(EC/P):P");

  sprintf(xtitle1,"Momentum (GeV)");
  sprintf(ytitle1,"EC (GeV)");
  sprintf(xtitle2,"EC_{in} (GeV)");
  sprintf(ytitle2,"EC_{out} (GeV)");
  sprintf(ytitle3,"EC/P (GeV)");

  // general cuts
  sprintf(xycuts,"(sqrt(vx*vx+vy*vy)<=2.0)");
  sprintf(eccuts,"(ECisHit!=0)");

  TCanvas *cECvP[2];
  TCanvas *cECivECo[2];
  TCanvas *cECPvP[2];
  char outname[100];

  for(i=0;i<2;i++){

    q = pow(-1,i);
    sprintf(qcuts,"(Q==%i)",q);
    sprintf(islep_ec_cut,"(IsLepG7ec==%i)",q);
    sprintf(islep_ect_cut,"(IsLepG7ect==%i)",q);
    sprintf(islep_cc_cut,"(IsLepG7cc==%i)",q);
    sprintf(islep_tof_cut,"(IsLepG7mm2==%i)",q);
    sprintf(islep_cut,"(IsLep==%i)",q);
    sprintf(islepG7_cut,"(IsLepG7==%i)",q);
    sprintf(ecin_mip_cut,"(ECin>0.045)");
    sprintf(plabel,"Q = %i",q);

    switch(iFlag){
    case 0:
      sprintf(cuts,"%s && %s && %s",qcuts,eccuts,islep_cc_cut);
      break;
    case 1:
      sprintf(cuts,"%s && %s && %s",qcuts,islep_cc_cut,islep_ect_cut);
      break;
    case 2:
      sprintf(cuts,"%s && %s && !%s",qcuts,islep_cc_cut,ecin_mip_cut);
      break;
    case 3:
      sprintf(cuts,"%s && %s && %s",qcuts,islep_cc_cut,ecin_mip_cut);
      break;
    case 4:
      sprintf(cuts,"%s && %s && %s && %s",qcuts,islep_cc_cut,islep_ect_cut,ecin_mip_cut);
      break;
    }

    sprintf(hname,"cECvP%i",i+1);
    sprintf(ctitle,"cECvP%i",i+1);
    cECvP[i]= new TCanvas(hname,ctitle,0,i*25,600,600);
    cECvP[i]->SetBorderMode(0);  //Bordermode (-1=down, 0 = no border, 1=up)
    cECvP[i]->SetFillStyle(4000);
    
    sprintf(hname,"hECvsP%i",i+1);
    hECvsP[i] = new TH2F(hname,plabel,nP,Pl,Ph,nEC,ECl,ECh);
    chain->Project(hname,item1,cuts);

    gStyle->SetOptStat(0);
    gPad->SetFillColor(0);
    gPad->SetLogz();
    gPad->SetLeftMargin(Lmar);
    gPad->SetRightMargin(Rmar);
    hECvsP[i]->Draw();
    hECvsP[i]->SetXTitle(xtitle1);
    hECvsP[i]->GetXaxis()->CenterTitle();
    hECvsP[i]->SetYTitle(ytitle1);
    hECvsP[i]->GetYaxis()->CenterTitle();
    hECvsP[i]->GetYaxis()->SetTitleOffset(yoff);
    hECvsP[i]->Draw("colz");

    sprintf(outname,"ECcuts_final_ECvsP%i_%i.gif",i+1,iFlag);
    cECvP[i]->Print(outname);
    sprintf(outname,"ECcuts_final_ECvsP%i_%i.eps",i+1,iFlag);
    cECvP[i]->Print(outname);

    sprintf(cname,"cECivECo%i",i+1);
    sprintf(ctitle,"cECivECo%i",i+1);
    cECivECo[i]= new TCanvas(cname,ctitle,200,i*25,600,600);
    cECivECo[i]->SetBorderMode(0);  //Bordermode (-1=down, 0 = no border, 1=up)
    cECivECo[i]->SetFillStyle(4000);
    
    sprintf(hname,"hECio%i",i+1);
    hECio[i] = new TH2F(hname,plabel,nECi,ECil,ECih,nECo,ECol,ECoh);
    chain->Project(hname,item2,cuts);

    gPad->SetFillColor(0);
    gPad->SetLogz();
    gPad->SetLeftMargin(Lmar);
    gPad->SetRightMargin(Rmar);
    hECio[i]->Draw();
    hECio[i]->SetXTitle(xtitle2);
    hECio[i]->GetXaxis()->CenterTitle();
    hECio[i]->SetYTitle(ytitle2);
    hECio[i]->GetYaxis()->CenterTitle();
    hECio[i]->GetYaxis()->SetTitleOffset(yoff);
    hECio[i]->Draw("colz");

    sprintf(outname,"ECcuts_final_ECivsECo%i_%i.gif",i+1,iFlag);
    cECivECo[i]->Print(outname);
    sprintf(outname,"ECcuts_final_ECivsECo%i_%i.eps",i+1,iFlag);
    cECivECo[i]->Print(outname);

    sprintf(cname,"cECPvP%i",i+1);
    sprintf(ctitle,"cECPvP%i",i+1);
    cECPvP[i]= new TCanvas(cname,ctitle,400,i*25,600,600);
    cECPvP[i]->SetBorderMode(0);  //Bordermode (-1=down, 0 = no border, 1=up)
    cECPvP[i]->SetFillStyle(4000);
    
    sprintf(hname,"hECPvsP%i",i+1);
    hECPvsP[i] = new TH2F(hname,plabel,nP,Pl,Ph,nECP,ECPl,ECPh);
    chain->Project(hname,item3,cuts);

    gPad->SetFillColor(0);
    gPad->SetLogz();
    gPad->SetLeftMargin(Lmar);
    gPad->SetRightMargin(Rmar);
    hECPvsP[i]->Draw();
    hECPvsP[i]->SetXTitle(xtitle1);
    hECPvsP[i]->GetXaxis()->CenterTitle();
    hECPvsP[i]->SetYTitle(ytitle3);
    hECPvsP[i]->GetYaxis()->CenterTitle();
    hECPvsP[i]->GetYaxis()->SetTitleOffset(yoff);
    hECPvsP[i]->Draw("colz");

    sprintf(outname,"ECcuts_final_ECPvsP%i_%i.gif",i+1,iFlag);
    cECPvP[i]->Print(outname);
    sprintf(outname,"ECcuts_final_ECPvsP%i_%i.eps",i+1,iFlag);
    cECPvP[i]->Print(outname);
  }
}

void CC_final(char *fname, Int_t iFlag)
{
  
  Int_t i, j, k;

  TH1F *hCCnpe[2];

  sprintf(item1,"CCnpe");

  sprintf(xtitle1,"CC Number of PhotoElectrons");
  sprintf(ytitle1,"Counts");

  // general cuts
  sprintf(xycuts,"(sqrt(vx*vx+vy*vy)<=2.0)");
  sprintf(cccuts,"(CChit_stat!=0)");

  TCanvas *CCcan[2];
  char outname[100];

  for(i=0;i<2;i++){

    q = pow(-1,i);
    sprintf(qcuts,"(Q==%i)",q);
    sprintf(islep_ec_cut,"(IsLepG7ec==%i)",q);
    sprintf(islep_ect_cut,"(IsLepG7ect==%i)",q);
    sprintf(islep_cc_cut,"(IsLepG7cc==%i)",q);
    sprintf(islep_tof_cut,"(IsLepG7mm2==%i)",q);
    sprintf(islep_cut,"(IsLep==%i)",q);
    sprintf(islepG7_cut,"(IsLepG7==%i)",q);
    sprintf(ecin_mip_cut,"(ECin>0.045)");
    sprintf(plabel,"Q = %i",q);

    switch(iFlag){
    case 0:
      sprintf(cuts,"%s && %s && %s && %s",qcuts,cccuts,islep_ec_cut,ecin_mip_cut);
      break;
    case 1:
      sprintf(cuts,"%s && %s && %s && %s",qcuts,islep_cc_cut,islep_ec_cut,ecin_mip_cut);
      break;
    }

    sprintf(hname,"CCcan%i",i+1);
    sprintf(ctitle,"CCcan%i",i+1);
    CCcan[i]= new TCanvas(hname,ctitle,0,i*25,600,600);
    
    sprintf(hname,"hCCnpe%i",i+1);
    hCCnpe[i] = new TH1F(hname,plabel,nCCnpe,CCnpel,CCnpeh);
    chain->Project(hname,item1,cuts);

    gStyle->SetOptStat(0);
    //    gPad->SetLogy();
    gPad->SetLeftMargin(Lmar);
    gPad->SetRightMargin(Rmar);
    hCCnpe[i]->Draw();
    hCCnpe[i]->SetXTitle(xtitle1);
    hCCnpe[i]->GetXaxis()->CenterTitle();
    hCCnpe[i]->SetYTitle(ytitle1);
    hCCnpe[i]->GetYaxis()->CenterTitle();
    hCCnpe[i]->GetYaxis()->SetTitleOffset(yoff);
    hCCnpe[i]->SetFillColor(2);
    hCCnpe[i]->Draw();

    sprintf(outname,"CCcuts_final_npe%i_%i.gif",i+1,iFlag);
    CCcan[i]->Print(outname);
    sprintf(outname,"CCcuts_final_npe%i_%i.eps",i+1,iFlag);
    CCcan[i]->Print(outname);

  }
}

void CC_comp(char *fname)
{
  
  Int_t i, j;
  Int_t k = 0;

  TH1F *hCCnpe[4];

  sprintf(item1,"CCnpe");

  sprintf(xtitle1,"CC Number of PhotoElectrons");
  sprintf(ytitle1,"Counts");

  // general cuts
  sprintf(xycuts,"(sqrt(vx*vx+vy*vy)<=2.0)");
  sprintf(cccuts,"(CChit_stat!=0)");

  TCanvas *CCcan[2];
  TCanvas *CCcan_rat[2];
  char outname[100];

  for(i=0;i<2;i++){

    q = pow(-1,i);
    sprintf(qcuts,"(Q==%i)",q);
    sprintf(islep_ec_cut,"(IsLepG7ec==%i)",q);
    sprintf(islep_ect_cut,"(IsLepG7ect==%i)",q);
    sprintf(islep_cc_cut,"(IsLepG7cc==%i)",q);
    sprintf(islep_tof_cut,"(IsLepG7mm2==%i)",q);
    sprintf(islep_cut,"(IsLep==%i)",q);
    sprintf(islepG7_cut,"(IsLepG7==%i)",q);
    sprintf(ecin_mip_cut,"(ECin>0.045)");
    sprintf(plabel,"Q = %i",q);

    sprintf(cname,"CCcan%i",i+1);
    sprintf(ctitle,"CCcan%i",i+1);
    CCcan[i]= new TCanvas(cname,ctitle,0,i*25,600,600);
      
    for(j=0;j<2;j++){
      switch(j){
      case 0:
	sprintf(cuts,"%s && %s && %s && %s",qcuts,cccuts,islep_ec_cut,ecin_mip_cut);
	break;
      case 1:
	sprintf(cuts,"%s && %s && %s && %s",qcuts,islep_cc_cut,islep_ec_cut,ecin_mip_cut);
	break;
      }

      sprintf(hname,"hCCnpe%i",k+1);
      hCCnpe[k] = new TH1F(hname,plabel,nCCnpe,CCnpel,CCnpeh);
      chain->Project(hname,item1,cuts);
      
      gStyle->SetOptStat(0);
      //    gPad->SetLogy();
      gPad->SetLeftMargin(Lmar);
      gPad->SetRightMargin(Rmar);
      hCCnpe[k]->SetXTitle(xtitle1);
      hCCnpe[k]->GetXaxis()->CenterTitle();
      hCCnpe[k]->SetYTitle(ytitle1);
      hCCnpe[k]->GetYaxis()->CenterTitle();
      hCCnpe[k]->GetYaxis()->SetTitleOffset(yoff);
      hCCnpe[k]->SetLineWidth(2);
      hCCnpe[k]->SetLineColor(lcol[j]);
      hCCnpe[k]->Draw(fSame[j]);
      k++;
    }

    sprintf(outname,"CCcuts_comp_npe%i.gif",i+1);
    CCcan[i]->Print(outname);
    sprintf(outname,"CCcuts_comp_npe%i.eps",i+1);
    CCcan[i]->Print(outname);

    sprintf(cname,"CCcan_rat%i",i+1);
    sprintf(ctitle,"CCcan_rat%i",i+1);
    CCcan_rat[i]= new TCanvas(cname,ctitle,(i+1)*100,i*25,600,600);

    sprintf(hname,"hCCrat%i",i);
    TH1F *hCCrat = (TH1F*)hCCnpe[k-1]->Clone(hname);
    hCCrat->Divide(hCCnpe[k-2]);
    hCCrat->Draw();
    
    sprintf(outname,"CCcuts_rat_npe%i.gif",i+1);
    CCcan_rat[i]->Print(outname);
    sprintf(outname,"CCcuts_rat_npe%i.eps",i+1);
    CCcan_rat[i]->Print(outname);
  }
}
