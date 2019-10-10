#include <iostream>
#include <fstream>

extern "C" {
  
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <ntypes.h>
#include <bostypes.h>
#include <clas_cern.h>
#include <particleType.h>
#include <kinematics.h>
#include <pdgutil.h>
#include <pid.h>
#include <scalers.h>
#include <utility.h>
#include <printBOS.h>
#include <ec.h>
#include <PartUtil.h>
#include <dataIO.h>
#include <itape.h>
#include <vertex.h>
#include <sc.h>
}

#include <Vec.h>
#include <lorentz.h>
#include <pputil.h>
#include <clasEvent.h> 
#include <matrix.h>
#include <g7Stats.h>

/*bitwise drop flags*/
#define DROP_RAW BIT(0)
#define DROP_DC0 BIT(1)
#define DROP_DC1 BIT(2)
#define DROP_HBLA BIT(3)
#define DROP_TBLA BIT(4)
#define DROP_HBTB BIT(5)
#define DROP_SC BIT(6)
#define DROP_EC BIT(7)
#define DROP_HBID BIT(8)
#define DROP_CL01 BIT(9)
#define DROP_SEB BIT(10)
#define DROP_TBID BIT(11)
#define DROP_HDPL BIT(12)
#define DROP_LAC BIT(13)
#define DROP_CC BIT(14)
#define DROP_ST BIT(15)
#define DROP_DHCL BIT(16)
#define DROP_TAGR BIT(17)

#define BUFSIZE 200000
#define PI 3.1415927
#define OMEGA_MASS 0.78194

//kinematic cuts
#define EBEAM_LO_CUT 0.0    // photon energy lower limit
#define EBEAM_HI_CUT 4.5    // photon energy upper limit
#define XVERT_LOW -3.0     // x-vertex lower limit    
#define XVERT_HI  3.0      // x-vertex upper limit 
#define YVERT_LOW -3.0     // y-vertex lower limit 
#define YVERT_HI  3.0      // y-vertex upper limit 
#define ZVERT_LOW -22.0    // LH2 target lower limit
#define ZVERT_HI  -14.0    // LH2 target upper limit
//#define ZVERT_LOW -100.0 // All targets lower limit
//#define ZVERT_HI  20.0   // All targets upper limit
#define TIMING_CUT 5.0     // particle timing cut

Nt_com nt_;
Nt_com_g7Stats ntp_;
Nt_com_vert ntv_;

TH1F *h201;
TH1F *h202;
TH1F *h203;
TH1F *h316;

extern "C" {
  void bnames_(int *);
  int SetVerbose(int);
}

int StartRun(int);
int ProcessEvent(clasEvent &event);
int DropList(int DropFlag);
void PrintUsage(char *processName);
double Vcosangle(fourVec a, fourVec b);
double Vangle(fourVec a, fourVec b);
void g7Stats_tree();
fourVec SetTarget4V(int RunNumber, float zpos);

int CurrentRun = 0;
int CurrentEvent = 0;
int partbank0 = 1;
int partbank = 0;
int Npart0 = 0;
int Npart1 = 0;

static int count = 0;

extern particleDataTable PDGtable;

int StartRun(int runNo)
{  
  if (CurrentRun != runNo) {
    vertex_brun(runNo);
    make_SCG_banks(runNo);
    CurrentRun = runNo;
  }
  return 0;
}


int DropList(int DropFlag)
{
  /* Mask off banks according to DropFlag*/

  if (DropFlag & DROP_RAW) bankList(&bcs_, "E-", "R");  
  if (DropFlag & DROP_DC0) bankList(&bcs_, "E-", "DC0 ");
  if (DropFlag & DROP_DC1) bankList(&bcs_, "E-", "DC1 ");
  if (DropFlag & DROP_HBLA) bankList(&bcs_, "E-", "HBLA");
  if (DropFlag & DROP_TBLA) bankList(&bcs_, "E-", "TBLA");
  if (DropFlag & DROP_HBTB) bankList(&bcs_, "E-", "HBTB");
  if (DropFlag & DROP_SC) bankList(&bcs_, "E-", SC_BANKS);
  if (DropFlag & DROP_EC) bankList(&bcs_, "E-", EC_BANKS);
  if (DropFlag & DROP_HBID) bankList(&bcs_, "E-", "HBID");
  if (DropFlag & DROP_CL01) bankList(&bcs_, "E-", "CL01");
  if (DropFlag & DROP_SEB) bankList(&bcs_, "E-", SEB_BANKS);
  if (DropFlag & DROP_TBID) bankList(&bcs_, "E-", "TBIDPARTTBERTBTR");
  if (DropFlag & DROP_HDPL) bankList(&bcs_, "E-", "HDPL");
  if (DropFlag & DROP_LAC) bankList(&bcs_, "E-", "EC1R");
  if (DropFlag & DROP_CC) bankList(&bcs_, "E-", CC_BANKS);
  if (DropFlag & DROP_ST) bankList(&bcs_, "E-", ST_BANKS);
  if (DropFlag & DROP_DHCL) bankList(&bcs_, "E-", "DHCL");
  if (DropFlag & DROP_TAGR) bankList(&bcs_, "E-", TAGGER_BANKS);
 
  return(0);
}


void PrintUsage(char *processName)
{
  cerr << processName << " <options> <filename>\n";
  cerr << "\toptions are:\n";
  cerr << "\t-b#\tSet beam type for RUNC bank (default=1).\n";
  cerr << "\t-T\tRemake the TBID banks.\n";
  cerr << "\t-t#\ttrigger mask.\n";
  cerr << "\t-D#\tDrop flag.\n";
  cerr << "\t-o<filename>\tBOS output file.\n";
  cerr << "\t-p#\tSet PART group number (default=1).\n";
  cerr << "\t-M#\tprocess maximum # of events.\n";
  cerr << "\t-i\tquiet mode (no counter).\n";
  cerr << "\t-h\tprint the above" << endl;
}


/* -------Root stuff must be global in SCOPE ------------*/

  TFile g7Stats_f("g7Stats.root","RECREATE");
//  TTree nt_t1("t1","part info");
  TTree ntp_t2("t2","Particle kin");
//  TTree ntv_t3("t3","vertex data");


// ------------------------------------------------
// ------------------ Main program ----------------
// ---------- To be modified to fit need ----------
// ------------------------------------------------

int main(int argc,char **argv)
{

  extern char *optarg;
  int c; 
  extern int optind;

  int i;
  int max = 0;
  int verbose = 0;
  int ret = 1;
  int Nevents = 0;
  int Nproc = 0;
  int Nwrite = 0;
  char *argptr;
  int Dispatch = 0;
  unsigned int triggerMask = 0;
  int writeFlag = 0;
  int remakeTBID = 0;
  int BeamType = 1;

  // bos stuff
  clasOutput coutput;
  char *BOSoutfile = "g7Stats.bos";
  int OutputUnitNo = 9;
  int MaxBanks = 1000; 
  char  out[300];
  char Elist[5 * MaxBanks];

  int DropFlag = 0x0;
  bool bBatchMode = false;    // events counter is on by default

  for (i = 0; i < argc; ++i) cerr << argv[i] << " "; cerr << endl;
  while ((c = getopt(argc,argv, "b:Tio:D:M:p:t:h")) != -1 ) {
    switch (c) {
      
    case 'b':
      BeamType = atoi(optarg);
      break;

    case 'T':
      remakeTBID = 1;
      break;
      
    case 't':
      triggerMask = strtoul(optarg,NULL,0);
      break;
      
    case 'p':
      partbank0 = atoi(optarg);
      cerr << "Using PART bank " << partbank0 << " for analysis" << endl;
      break;
      
    case 'D':
      DropFlag = strtoul(optarg,NULL,0);
      break;
      
    case 'o':
      BOSoutfile = optarg;
      if(BOSoutfile) {
	unlink(BOSoutfile);
	coutput.open(BOSoutfile,OutputUnitNo);
	cerr << "Output file: " << BOSoutfile << endl;
      }

      break;
      
    case 'M':
      max = atoi(optarg);
      break;
	
    case 'i':
      bBatchMode = true;
      break; 
      
    case 'h':
      PrintUsage(argv[0]);
      exit(0);
      break;
      
    default:
      cerr << "Unrecognized argument: " << argptr << endl;;
      break;
    }
  }

  PDGtable.initialize();

  // Initialize BOS
  bnames_(&MaxBanks);
  initbos();
  configure_banks(stderr,0);

  g7Stats_tree(); // create ROOT tree and histograms

  set_beam_type(BeamType); // set beam type for RUNC bank

  for (i = optind; i < argc; ++i) {
    argptr = argv[i];
    // process all arguments on command line.
    if (*argptr != '-') {
      // we have a file to process
      clasEvent event(argptr,&bcs_,partbank0,0);
      
      cerr << "initialize:\t" << argptr << endl;
      
      Dispatch = isDispatcher(argptr);
      if (event.status()) {
	ret = 1;
	while ((max ? Nevents < max : 1) && ret) {
	  // process every event in the file
	  ret = event.read(partbank0);
	  if (ret == DISIO_DATA) {
	    
	    Nevents++;
	    if (!bBatchMode && ((Nevents % 100) == 0)){
              cerr << Nevents << "\r";
            }
                  
	    if (event.status()) {
	      if (triggerMask ? (triggerMask & event.trig()) : 1) {
		int runno = event.run();	
		StartRun(runno);
		InitEloss(runno);

		if(remakeTBID) event.buildTBID(partbank0);

		writeFlag = ProcessEvent(event) ;
		if (coutput.status() && writeFlag) {
		  if (DropFlag) {
		    DropList(DropFlag);
		  }
		  coutput.write(&bcs_);
		  Nwrite++;
		}
	      }
	    }
	  }else if (ret == DISIO_COMMAND) {
 	    cerr << "Message from Giant Head: " << getBuffer() << endl;;
      	  }
	  event.clean();
	  Nproc++;
	}
	cerr << "\nTotal number of itape records read:\t" << Nevents << endl;
	cerr << "Total number of events processed:\t" << Nproc << endl;
	cerr << "\tTotal number of records written:\t" << Nwrite << endl;
	cerr << "\tTotal number of PART bank 0:\t" << Npart0 <<endl;
	cerr << "\tTotal number of PART bank 1:\t" << Npart1 <<endl;
      }else{
	cerr << "Unable to open " << argptr << endl;
      }
    }
  }
  
  if (Dispatch) {
    disIO_command("FINISHED");
  }
  
  cerr << "\nTotal number of itape records:\t" << Nevents << endl;

  // close root file
  g7Stats_f.Write();
  g7Stats_f.Close();

  // Write and close the BOS output file
  if(coutput.status()) {
    coutput.write(&bcs_,"0");
    coutput.close();
  }

  return (0);
}


// ------------------------------------------------
// ---------------- ProcessEvent ------------------
// ------------ Make all analysis here ------------
// ------------------------------------------------

int ProcessEvent(clasEvent &event)
{
  int j;

  fourVec beam;

  int q;
  int ret = 0;
  int vert_x_cut = 0;
  int vert_y_cut = 0;
  int vert_z_cut = 0;
  int vert_cut = 0;
  int time_cut = 0;
  int ebeam_cut = 0;

  memset(&nt_,0,sizeof(nt_));
  memset(&ntp_,0,sizeof(ntp_));
  memset(&ntv_,0,sizeof(ntv_));

  // timing cut
  //    time_cut = (fabs(event.vtime() - event.stVtime()) <= TIMING_CUT);
  time_cut = 1;
  
  // vertex cuts
  vert_x_cut = cut(event.x(),XVERT_LOW,XVERT_HI);
  vert_y_cut = cut(event.y(),YVERT_LOW,YVERT_HI);
  vert_z_cut = cut(event.z(),ZVERT_LOW,ZVERT_HI);
  //  vert_cut = vert_x_cut && vert_y_cut && vert_z_cut;
  vert_cut = 1;

  // beam energy cut
  //  clasRUNC_t *RUNC = (clasRUNC_t *)getBank(&wcs_,"RUNC");
  //  if(RUNC){
  //    nt_.ebeam = RUNC->runc.beam.energy.val.f[0];
  //  }
  //  ebeam_cut = cut(nt_.ebeam,EBEAM_LO_CUT,EBEAM_HI_CUT);

  // photon beam energy cut
  // ebeam_cut = cut(event.beam().get4P().t(),EBEAM_LO_CUT,EBEAM_HI_CUT);
  ebeam_cut = 1;

  if(time_cut){//timing cut
    if(vert_cut){//vertex cut
      if(ebeam_cut){//beam energy cut
	
	clasPART_t *PART0 = (clasPART_t *)getGroup(&bcs_,"PART",0);
	clasPART_t *PART1 = (clasPART_t *)getGroup(&bcs_,"PART",1);

	if(PART0){// thrown event analysis here:
	  Npart0++;
	  if(PART1){
	    Npart1++;
	  }else{
	    cout<<event.run()<<" "<<event.event()<<endl;
	  }
	}
	
	ntp_.nEvents = event.event();

	ntv_.vx = event.x(); // x vertex
	ntv_.vy = event.y(); // y vertex
	ntv_.vz = event.z(); // z vertex
	
	// fill vertex histograms
	h201->Fill(ntv_.vx);
	h202->Fill(ntv_.vy);
	h203->Fill(ntv_.vz);
	
	beam = event.beam().get4P(); // beam 4V
	nt_.ebeam = beam.t();         // beam energy
	//	beam.set(nt_.ebeam,threeVec(0.0,0.0,nt_.ebeam));
	h316->Fill(nt_.ebeam);
	
        ntp_.Hits = event.N(); // number of hits for event      
        ntp_.Tracks = event.nTracks(); // number of tracks      

        for(j=0;j<ntp_.Hits;j++){
	  ntp_.Q[j] = event.cp(j+1).Q(); // particle charge
          ntp_.Pid[j] = event.cp(j+1).pid(); // particle id
          ntp_.IsLep[j] = event.cp(j+1).IsLeptonG7(); // lepton check
          ntp_.MaybeLep[j] = event.cp(j+1).MaybeLeptonG7(); // maybe lepton 
	  if(ntp_.IsLep[j]) ntp_.nIsLep++;
	  if(ntp_.MaybeLep[j]) ntp_.nMaybeLep++;
	  //	  cout<<ntp_.nEvents<<"\t"<<j<<"\t"<<event.cp(j+1).IsLeptonG7()<<"\t";
	  //	  cout<<event.cp(j+1).IsLeptonG7_EC()<<"\t";
	  //	  cout<<event.cp(j+1).IsLeptonG7_CC()<<"\t";
	  //	  cout<<event.cp(j+1).IsLeptonG7_MM2()<<endl;
	}

	q = Q(Electron);
	ntp_.NnegLep = event.nIsLeptonG7(q);
	for(j=0;j<ntp_.NnegLep;j++){
	  ntp_.negSec[j] = event.g7Lepton(q,j+1).sec();
	  ntp_.negLepEC[j] = event.g7Lepton(q,j+1).IsLeptonG7_EC();
	  ntp_.negLepCC[j] = event.g7Lepton(q,j+1).IsLeptonG7_CC();
	  ntp_.negLepTOF[j] = event.g7Lepton(q,j+1).IsLeptonG7_MM2();
	  ntp_.negGold[j] = (ntp_.negLepEC[j]==q && ntp_.negLepCC[j]==q);
	}

	q = Q(Positron);
	ntp_.NposLep = event.nIsLeptonG7(q);
	for(j=0;j<ntp_.NposLep;j++){
	  ntp_.posSec[j] = event.g7Lepton(q,j+1).sec();
	  ntp_.posLepEC[j] = event.g7Lepton(q,j+1).IsLeptonG7_EC();
	  ntp_.posLepCC[j] = event.g7Lepton(q,j+1).IsLeptonG7_CC();
	  ntp_.posLepTOF[j] = event.g7Lepton(q,j+1).IsLeptonG7_MM2();
	  ntp_.posGold[j] = (ntp_.posLepEC[j]==q && ntp_.posLepCC[j]==q);
	}

        ntp_.trig = event.trig(); // event trigger bit
	for(j=0;j<12;j++){
	  if((ntp_.trig >> j) & 0x1){
	    ntp_.tbit[ntp_.ntrig++] = j+1;
	  }
	}
	//	cout<<"The Trig "<<ntp_.trig<<" "<<ntp_.ntrig<<endl;

	if(ntp_.nIsLep || ntp_.nMaybeLep) ret = 1;
	ntp_t2.Fill(); 
	
      }//end of beam energy cut
    }//end of vertex selection
  }//end of time selection
  return (ret);
}

//********************** cos(angle) between two vectors in degrees *****************
double Vcosangle(fourVec a, fourVec b)
{
 double s,m,t;
 s=a*b;
 m=~a.V()*~b.V();
 t=a.t()*b.t();
 return (t-s)/m;
}
//**********************************************************************************

//********************** Angle between two vectors in degrees **********************
double Vangle(fourVec a, fourVec b)
{
  return (RAD2DEG*acos(Vcosangle(a,b)));
}
//**********************************************************************************

//********************** Calculate Target 4-Vector *********************************
fourVec SetTarget4V(int RunNumber, float zpos)
{

  fourVec tgt;

  // set target mass by z-postion
  if(zpos>=-22.0 && zpos<=4.0){
    if(zpos>=-22.0 && zpos<-14.0){
      if((RunNumber>=35466) && (RunNumber<=35563)){
        tgt.set(PROTON_MASS,threeVec(0.0,0.0,0.0));
      } else{
        tgt.set(DEUTERON_MASS,threeVec(0.0,0.0,0.0));
      }
    }
    if(zpos>-11.0 && zpos<-8.5){
      tgt.set(FE56_MASS,threeVec(0.0,0.0,0.0));
    }
    if(zpos>-1.0 && zpos<1.5){
      tgt.set(TI48_MASS,threeVec(0.0,0.0,0.0));
    }
    if(zpos>-6.0 && zpos<-3.5){
      tgt.set(PB208_MASS,threeVec(0.0,0.0,0.0));
    }
    if((zpos>-14.0 && zpos<-11.0) || (zpos>-8.5 && zpos<-6.0) || (zpos>-3.5 && zpos<-1.0) || (zpos>1.5 && zpos<4.0)){
      tgt.set(C12_MASS,threeVec(0.0,0.0,0.0));
    }
  }
  else{
    tgt.set(PROTON_MASS,threeVec(0.0,0.0,0.0));
  }
  
  return(tgt);
}
//**********************************************************************************
