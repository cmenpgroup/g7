#include "TROOT.h"
#include "TRint.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"
/*
structure definitions
*/
#define MHITS 20                               
typedef struct ntupcom{
  float ebeam;
} Nt_com;
extern Nt_com nt_;

typedef struct ntupcom2{
  int nEvents; 
  int Hits; 
  int Tracks;
  int Q[MHITS];
  int Pid[MHITS];
  int IsLep[MHITS];
  int MaybeLep[MHITS];
  int nIsLep;
  int nMaybeLep;
  int NnegLep;
  int negSec[MHITS];
  int negLepEC[MHITS];
  int negLepCC[MHITS];
  int negLepTOF[MHITS];
  int negGold[MHITS];
  int NposLep;
  int posSec[MHITS];
  int posLepEC[MHITS];
  int posLepCC[MHITS];
  int posLepTOF[MHITS];
  int posGold[MHITS];
  int trig;
  int ntrig;
  int tbit[MHITS];
} Nt_com_g7Stats;
extern Nt_com_g7Stats ntp_;

#define MAX_VERT 10                               
typedef struct ntupcom3{
  float vx;
  float vy;
  float vz;
} Nt_com_vert;
extern Nt_com_vert ntv_;

extern  TFile g7Stats_f;
//extern  TTree nt_t1;
extern  TTree ntp_t2;
//extern  TTree ntv_t3;

extern TH1F *h201;
extern TH1F *h202;
extern TH1F *h203;
extern TH1F *h316;






