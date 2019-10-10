#include "TROOT.h"
#include "TRint.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"

//kinematic cuts
#define EGAM_LO_CUT 0.0
#define EGAM_HI_CUT 6.0
#define XVERT_LOW -3.0
#define XVERT_HI  3.0
#define YVERT_LOW -3.0
#define YVERT_HI  3.0
#define ZVERT_LOW -100.0
#define ZVERT_HI  20.0
//#define TIMING_CUT 1.0
#define TIMING_CUT 5.0
#define TRIG_LOW 3
#define TRIG_HI 64

//lepton LAC and TOF cuts
#define LACTHRESHOLD 0.1
#define EOVERPLO 0.22
#define EOVERPHI 0.44
#define MOMTHRESHOLD 0.4
#define MASSSQLO -0.0025
#define MASSSQHI  0.0025

/*
structure definitions
*/
typedef struct ntupcom{
  float egam;
  int Event;
  int Tracks;
  int trig;
} Nt_com;
extern Nt_com nt_;

#define MHITS 20
typedef struct ntupcom2{
  float tpho;
  int HitsEVNT;
  int Pid_EVNT_pdg[MHITS];
  int Pid_EVNT_geant[MHITS];
  int Hits;
  int Pid[MHITS];
  float beta[MHITS];
  float beta_Pid[MHITS];
  float Q[MHITS];
  float Mass[MHITS];
  float P[MHITS];
  float Pz[MHITS];
  float Sec[MHITS];
  float tprop[MHITS];
  float tpho_v2[MHITS];
  float phi[MHITS];
  float theta[MHITS];
  int fidcut[MHITS];
  int trigPart[MHITS];
  int IsLep[MHITS];
  int IsLepG7[MHITS];
  int IsLepG7ec[MHITS];
  int IsLepG7ect[MHITS];
  int IsLepG7ecio[MHITS];
  int IsLepG7ecr1[MHITS];
  int IsLepG7ecr2[MHITS];
  int IsLepG7cc[MHITS];
  int IsLepG7mm2[MHITS];
  int MaybeLepG7[MHITS];
  int TOFstat[MHITS];
  float SC_time[MHITS];
  float scvT[MHITS];
  float scvT_Pid[MHITS];
  float scvT_Lep[MHITS];
  float scLen[MHITS];
  int scId[MHITS];
  float TOF_MassSq[MHITS];
  int ECstat[MHITS];
  float EC_time[MHITS];
  float EC[MHITS];
  float ECin[MHITS];
  float ECout[MHITS];
  float ECx[MHITS];
  float ECy[MHITS];
  float ECz[MHITS];
  int ECfid[MHITS];
  int ECisHit[MHITS];
  float ECpre[MHITS];
  float ECiopre[MHITS];
  float sigEC_cut[MHITS];
  float sigECio_cut[MHITS];
  int LACstat[MHITS];
  float LAC[MHITS];
  float LACin[MHITS];
  int CCstat[MHITS];
  float CC_time[MHITS];
  int CChit_stat[MHITS];
  float CCnpe[MHITS];
  float CCtheta[MHITS];
  float CC_QF[MHITS];
  float CC_CS[MHITS];
  int CCseg[MHITS];
  int CCpmt[MHITS];
  int CCphimatch[MHITS];
} Nt_com_lep;
extern Nt_com_lep ntp_;

#define MAX_VERT 10                               
typedef struct ntupcom3{
  float vx;
  float vy;
  float vz;
} Nt_com_vert;
extern Nt_com_vert ntv_;

/* some extra global variables */

extern  TFile lep_f;
//extern  TTree nt_t1;
extern  TTree ntp_t2;
//extern  TTree ntv_t3;

extern TH1F *h110;
extern TH1F *h201;
extern TH1F *h202;
extern TH1F *h203;
extern TH1F *h316;
extern TH1F *h317;
extern TH1F *h350;
extern TH1F *h370;
extern TH1F *h400;
extern TH1F *h401;
extern TH1F *h402;
extern TH1F *h403;
extern TH1F *h410;
extern TH1F *h411;
extern TH1F *h412;
extern TH1F *h413;






