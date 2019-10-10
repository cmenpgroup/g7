//vert_g7.C
//
// Plot the vertex x, y, and z positions for the g7 targets
//
// M. H. Wood
// University of South Carolina
//
// Notes:
//
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

void vert_g7(char *fname="scanPID.root",Float_t vxyl=1.2)
{
  gROOT->Reset();

  Int_t i,stats;
  Float_t xMean,xRMS,yMean,yRMS;

  Float_t nVx = 100;
  Float_t Vxl = -5.0;
  Float_t Vxh = 5.0;
  Float_t nVy = 100;
  Float_t Vyl = -5.0;
  Float_t Vyh = 5.0;
  Float_t nVz = 100;
  Float_t Vzl = -30.0;
  Float_t Vzh = 10.0;

  char hname[50];        // histogram name
  char title[50];       // histogram title
  char vxtitle[50];     // histogram x-vertex title
  char vytitle[50];     // histogram y-vertex title
  char vztitle[50];     // histogram z-vertex title
  char ytitle[50];      // histogram y-axis title
  char target[100];     // target name

  char cuts[500];       // must use "real space" w/ sprintf
  char cuts_mess[500];  // must use "real space" w/ sprintf
  char vxycuts[100];    // x,y vertex cuts
  char vzcuts[100];     // z vertex cuts

  Int_t ntgt = 9;       // number of target (z-vertex) choices
  TH2F *hVxy[9];

  FILE *fout;
  char fname1[100];
  sprintf(fname1,"vert_g7_1.dat");
  fout = fopen(fname1,"w");
  
  sprintf(vxtitle,"x (cm)");
  sprintf(vytitle,"y (cm)");
  sprintf(vztitle,"z (cm)");
  sprintf(ytitle,"counts");

  // data files contain the trees
  TFile *f = new TFile(fname,"READ");

  // set up cuts for projection from tree
  sprintf(vxycuts,"(sqrt(vx*vx + vy*vy) < %f)",vxyl);

  //create canvas (name,title,x,y,width,height)
  TCanvas *Vertex1 = new TCanvas("Vertex1","Vertex1",0,0,1000,1000);

  Vertex1->Divide(3,3);

  for(i=0;i<ntgt;i++){
    
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
    case 6:
      sprintf(target,"Upstream");
      sprintf(vzcuts,"(vz<-22.0)");
      break;
    case 7:
      sprintf(target,"Downstream");
      sprintf(vzcuts,"(vz>=5.0)");
      break;
    case 8:
      sprintf(target,"No z cut");
      sprintf(vzcuts,"");
      break;
    default:
      cout << "Unrecognized target number: " << i << endl;;
      sprintf(target,"Unknown");
      sprintf(vzcuts,"(vz<-22.0)");
      break;
    }

    // Fill histograms for x,y vertex 
    sprintf(hname,"hVxy%d",i);
    sprintf(title,"%s",target);
    hVxy[i] = new TH2F(hname,title,nVx,Vxl,Vxh,nVy,Vyl,Vyh);
    printf("%s Cuts %s \n",target,cuts);    // print cuts to the screen
    t2->Project(hname,"vy:vx",vzcuts);

    Vertex1->cd(i+1);
    hVxy[i]->Draw();
    hVxy[i]->SetXTitle(vxtitle);
    hVxy[i]->GetXaxis()->CenterTitle();
    hVxy[i]->SetYTitle(vytitle);
    hVxy[i]->GetYaxis()->CenterTitle();
    hVxy[i]->Draw("col");
    stats = hVxy[i]->GetEntries();
    xMean = hVxy[i]->GetMean(1);
    xRMS = hVxy[i]->GetRMS(1);
    yMean = hVxy[i]->GetMean(2);
    yRMS = hVxy[i]->GetRMS(2);
    fprintf(fout,"%s : %i\t%f\t%f\t%f\t%f\n",target,stats,xMean,xRMS,yMean,yRMS);

  }
  fclose(fout);
  Vertex1->Print("vert_g7_1.gif");


  //create canvas (name,title,x,y,width,height)
  TCanvas *Vertex2 = new TCanvas("Vertex2","Vertex2",500,0,700,900);

  Vertex2->Divide(1,2);

  // Fill histograms for x,z vertex 
  sprintf(hname,"hVxz");
  sprintf(title,"Vertex x vs z");
  TH2F *hVyz = new TH2F(hname,title,nVx,Vxl,Vxh,nVz,Vzl,Vzh);
  t2->Project(hname,"vz:vx","");

  // Fill histograms for y,z vertex 
  sprintf(hname,"hVyz");
  sprintf(title,"Vertex y vs z");
  TH2F *hVyz = new TH2F(hname,title,nVy,Vyl,Vyh,nVz,Vzl,Vzh);
  t2->Project(hname,"vz:vy","");

  Vertex2->cd(1);
  hVxz->Draw();
  hVxz->SetXTitle(vxtitle);
  hVxz->GetXaxis()->CenterTitle();
  hVxz->SetYTitle(vztitle);
  hVxz->GetYaxis()->CenterTitle();
  hVxz->Draw("col");

  Vertex2->cd(2);
  hVyz->Draw();
  hVyz->SetXTitle(vytitle);
  hVyz->GetXaxis()->CenterTitle();
  hVyz->SetYTitle(vztitle);
  hVyz->GetYaxis()->CenterTitle();
  hVyz->Draw("col");

  Vertex2->Print("vert_g7_2.gif");
}
