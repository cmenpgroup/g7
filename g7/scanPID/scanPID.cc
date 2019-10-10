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
#include <scanPID.h>

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

#define SIGMA_ETRK  0.01; /* momentum resolution for eletron ID*/
#define ETOT_EL_CUT  3.0; /* number of sigmas for EC total electron cut */
#define EIO_EL_CUT  3.0; /* number of sigmas for EC (in-out) electron cut */

Nt_com nt_;
Nt_com_lep ntp_;
Nt_com_vert ntv_;

TH1F *h110;
TH1F *h201;
TH1F *h202;
TH1F *h203;
TH1F *h316;
TH1F *h317;
TH1F *h350;
TH1F *h370;
TH1F *h400;
TH1F *h401;
TH1F *h402;
TH1F *h403;
TH1F *h410;
TH1F *h411;
TH1F *h412;
TH1F *h413;

extern "C" {
  void bnames_(int *);
  int SetVerbose(int);
}

int StartRun(int);
int ProcessEvent(clasEvent &event, bool PrintTiming);
int DropList(int DropFlag);
void PrintUsage(char *processName);
double Vcosangle(fourVec a, fourVec b);
double Vangle(fourVec a, fourVec b);
void scanPID_tree();
float PredictE(float p);
float PredictEInOut(float p);
float PredictSigmaE(float p);
float PredictSigmaEInOut(float p);
void PrintSummary(int Nevents);

int CurrentRun = 0;
int CurrentEvent = 0;
int partbank0 = 1;
int partbank = 0;
int tdiff_ep_em=0;
int tdiff_ep_gam=0;
int tdiff_em_gam=0;
int tdiff_ep_new=0;
int tdiff_em_new=0;
int tpho_diff_ep=0;
int tpho_diff_em=0;
int tpho_diff_ep_em=0;

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
  cerr << "\t-b#\t\tSet beam type for RUNC bank (default=1).\n";
  cerr << "\t-T\t\tRemake the TBID banks.\n";
  cerr << "\t-t#\t\ttrigger mask.\n";
  cerr << "\t-D#\t\tDrop flag.\n";
  cerr << "\t-p#\t\tSelect PART bank group number (default=1).\n";
  cerr << "\t-o<filename>\tBOS output file.\n";
  cerr << "\t-M#\t\tprocess maximum # of events.\n";
  cerr << "\t-i\t\tquiet mode (no counter).\n";
  cerr << "\t-s\t\tprint out timing information.\n";
  cerr << "\t-h\t\tprint the above" << endl;
}


/* -------Root stuff must be global in SCOPE ------------*/

  TFile lep_f("scanPID.root","RECREATE");
//  TTree nt_t1("t1","part info");
  TTree ntp_t2("t2","scanPID kin");
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
  char *BOSoutfile = "scanPID.bos";
  int OutputUnitNo = 9;
  int MaxBanks = 1000; 
  char  out[300];
  char Elist[5 * MaxBanks];

  int DropFlag = 0x0;
  bool bBatchMode = false;    // events counter is on by default
  bool PrintTiming = false;   // flag to print out timing info

  for (i = 0; i < argc; ++i) cerr << argv[i] << " "; cerr << endl;
  while ((c = getopt(argc,argv, "b:Tio:D:M:p:t:sh")) != -1 ) {
    switch (c) {
      
    case 'b':
      BeamType = atoi(optarg);
      set_beam_type(BeamType); // set beam type for RUNC bank
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
      
    case 's':
      PrintTiming = true;
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

  scanPID_tree();

  for (i = optind; i < argc; ++i) {
    argptr = argv[i];
    // process all arguments on command line.
    if (*argptr != '-') {
      // we have a file to process
      clasEvent event(argptr,&bcs_,partbank0,0);
      event.verbose(verbose);

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
                  
           if(event.type()<10){
	     if(event.triggerMask(triggerMask)){
	       int runno = event.run();	
	       StartRun(runno);
	       InitEloss(runno);

	       if(remakeTBID) event.buildTBID(partbank0);
	       
	       writeFlag = ProcessEvent(event,PrintTiming) ;
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
      }else {
	cerr << "Unable to open " << argptr << endl;
      }
    }
  }
  
  if (Dispatch) {
    disIO_command("FINISHED");
  }
  
  cerr << "\nTotal number of itape records:\t" << Nevents << endl;

  PrintSummary(Nevents);

  /*  close root file  */
  lep_f.Write();
  lep_f.Close();

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

int ProcessEvent(clasEvent &event, bool PrintTiming)
{
  fourVec beam, target;

  unsigned int myTrig = 0x10;

  int i,j,k;
  int ret = 0;
  int OutOfTime = 0;
  int time_cut = 0;
  int vert_x_cut = 0;
  int vert_y_cut = 0;
  int vert_z_cut = 0;
  int vert_cut = 0;
  int egam_cut = 0;
  int CCphi_cut = 0;
  int trig_cut = 0;

  float vT_ep, vT_em, tpho_ep, tpho_em, tprop_ep, tprop_em;
  float ztgt = 0.0;
  float MassSq, Beta2;
  float diff, best_diff;
  float new_phi, dphi_sec;

  float sigma_e_whole, sigma_e_in_out, sigma_mo;

  memset(&nt_,0,sizeof(nt_));
  memset(&ntp_,0,sizeof(ntp_));
  memset(&ntv_,0,sizeof(ntv_));

  clasTGEO_t *TGEO = (clasTGEO_t *)getBank(&wcs_,"TGEO");
  clasTAGR_t *TAGR = (clasTAGR_t *)getBank(&bcs_,"TAGR");
  clasEVNT_t *EVNT = (clasEVNT_t *)getBank(&bcs_,"EVNT");

  // photon timing cut
  //    time_cut = (fabs(event.vtime() - event.stVtime()) < TIMING_CUT);
  time_cut = 1;

  // vertex cuts
  ntv_.vx = event.x(); // x vertex
  ntv_.vy = event.y(); // y vertex
  ntv_.vz = event.z(); // z vertex
  vert_x_cut = cut(ntv_.vx,XVERT_LOW,XVERT_HI);
  vert_y_cut = cut(ntv_.vy,YVERT_LOW,YVERT_HI);
  vert_z_cut = cut(ntv_.vz,ZVERT_LOW,ZVERT_HI);
  //  vert_cut = (vert_x_cut && vert_y_cut && vert_z_cut);
  vert_cut = 1;

  // photon beam energy cut
  beam = event.beam().get4P(); // photon beam 4V
  nt_.egam = beam.t();         // photon beam energy
  // egam_cut = cut(nt_.egam,EGAM_LO_CUT,EGAM_HI_CUT);
  egam_cut = 1;

  nt_.trig = event.trig(); // event trigger bit
  trig_cut = cut(nt_.trig,TRIG_LOW,TRIG_HI);

  nt_.Event = event.event(); // HEAD bank event number
  nt_.Tracks = event.nTracks();
  h110->Fill(nt_.Tracks);

  if(trig_cut){ //trigger cut
    if(egam_cut){ //beam energy cut
      if(time_cut){//timing cut
	if(vert_cut){//vertex cut
	  
	  if(TGEO){  // z-offset for target center
	    ztgt = TGEO->tgeo[0].z;  
	  }
	  	  
	  // fill vertex histograms
	  h201->Fill(ntv_.vx);
	  h202->Fill(ntv_.vy);
	  h203->Fill(ntv_.vz);
	  
	  //event.eLoss("g7a"); //energy loss correction, this may be redundant
	  h316->Fill(nt_.egam);
	  
	  // get tpho for each event
	  ntp_.tpho = event.tpho();     
	  h317->Fill(ntp_.tpho);	    
	  
	  if(PrintTiming){
	    // print out THE PHOTON info
	    cout << "\nEvent " << event.event();
	    cout << "\tEg " << nt_.egam; 
	    cout << " \ttpho " << ntp_.tpho << endl;
	    cout << "--------------------------------------------------------" << endl;
	    // TAGR info
	    if(TAGR){
	      cout <<"Tid "<<"\tEid"<<"\tEg"<<"\tTpho"<<"\tStat"<<endl;
	      for(i=0;i<TAGR->bank.nrow;i++){
		cout << TAGR->tagr[i].t_id;
		cout << "\t" << TAGR->tagr[i].e_id;
		cout.precision(5);
		cout << "\t" << TAGR->tagr[i].erg;
		cout << "\t" << TAGR->tagr[i].tpho;
		cout << "\t" << TAGR->tagr[i].stat << endl;
	      }
	    }
	    cout << "\nParticle timing:" << endl;
	    cout << "Pid" << "\tBeta" << "\tscTime" << "\tscLen";
	    cout << "\tvTime" << "\tvTime(Mass)" << "\tvTime(Lep)";
	    cout << "\tTprop" << "\t\ttpho_v2" << "\t\tIsLep" << endl;
	  }
	  
	  if(EVNT){
	    ntp_.HitsEVNT = EVNT->bank.nrow; // number of hits for EVNT bank
	    for(j=0;j<ntp_.HitsEVNT;j++){
	      ntp_.Pid_EVNT_pdg[j] = EVNT->evnt[j].id;
	      ntp_.Pid_EVNT_geant[j] = sebPID(ntp_.Pid_EVNT_pdg[j]);
	    }
	  }
	  
	  ntp_.Hits = event.N(); // number of hits for event	
	  for(j=0;j<ntp_.Hits;j++){
	    clasParticle cp = event.cp(j+1);
	    ntp_.Pid[j] = cp.pid(); // particle id
	    ntp_.Mass[j] = cp.mass(); // mass
	    ntp_.P[j] = ~cp.p().V(); //  momentum
	    ntp_.Pz[j] = cp.p().V().z(); //  z-component momentum
	    ntp_.Q[j] = cp.Q(); // charge
	    ntp_.Sec[j] = cp.sec(); // sector number
	    ntp_.tprop[j] = cp.tprop(); // prop time

	    ntp_.beta[j] = cp.beta(); // beta
	    // particle angle theta
	    ntp_.theta[j] = RAD2DEG*cp.p().V().theta(); 
	    // particle angle phi
	    ntp_.phi[j] = RAD2DEG*cp.p().V().phi(); 
	    ntp_.fidcut[j] = cp.FidCut(); // fiducial cuts
	    // trigger particle finder
	    ntp_.trigPart[j] = cp.triggerParticle(myTrig); 
	    h350->Fill(ntp_.phi[j]);
	    
	    // Lepton checks
            ntp_.IsLep[j] = cp.IsLepton();
            ntp_.IsLepG7[j] = cp.IsLeptonG7();
            ntp_.IsLepG7ec[j] = cp.IsLeptonG7_EC();
            ntp_.IsLepG7ect[j] = cp.IsLeptonG7_ECtot();
            ntp_.IsLepG7ecio[j] = cp.IsLeptonG7_ECio();
            ntp_.IsLepG7ecr1[j] = cp.IsLeptonG7_ECin2EC(0.95,1.05);
            ntp_.IsLepG7ecr2[j] = cp.IsLeptonG7_ECin2EC(0.8,100.0);
            ntp_.IsLepG7cc[j] = cp.IsLeptonG7_CC();
            ntp_.IsLepG7mm2[j] = cp.IsLeptonG7_MM2();
            ntp_.MaybeLepG7[j] = cp.MaybeLeptonG7();
	    if(ntp_.Q[j]!=0) h370->Fill(ntp_.IsLepG7[j]);

	    // SC stuff
	    ntp_.SC_time[j] = cp.sc_time(); // SC time
	    ntp_.scvT[j] = cp.scVertexTime(); // SC vertex time
	    ntp_.scvT_Pid[j] = cp.scVertexTime(M((Particle_t)ntp_.Pid[j]));
	    ntp_.scLen[j] = cp.sc_len(); // SC path length
	    ntp_.scId[j] = cp.scPaddleId(); // SC paddle id
	  
	    // beta from scvT_Pid
	    ntp_.beta_Pid[j] = ntp_.scLen[j]/(LIGHT_SPEED*(ntp_.SC_time[j]-ntp_.scvT_Pid[j]));
	    // calculate vertex time with beta=1
	    ntp_.scvT_Lep[j] = ntp_.SC_time[j] - ntp_.scLen[j]/LIGHT_SPEED;
	    
	    // EC stuff
	    ntp_.ECstat[j] = cp.ecStatus();  // EC status
	    ntp_.EC_time[j] = cp.ecTime(); // EC time
	    ntp_.EC[j] = cp.ecEnergy(); // EC total energy
	    ntp_.ECin[j] = cp.ecEnergyIn(); // EC inner energy
	    ntp_.ECout[j] = cp.ecEnergyOut(); // EC outer energy
	    ntp_.ECx[j] = cp.ECpos().x(); // EC x-pos from TDPL
	    ntp_.ECy[j] = cp.ECpos().y(); // EC y-pos from TDPL
	    ntp_.ECz[j] = cp.ECpos().z(); // EC z-pos from TDPL
	    ntp_.ECfid[j] = cp.ECfidcut(); // EC fiducial cut
	    ntp_.ECisHit[j] = cp.isEChit();  // EC hit check
	    
	    //information on SEB style lepton ID
	    ntp_.ECpre[j] = PredictE(ntp_.P[j]);
	    ntp_.ECiopre[j] = PredictEInOut(ntp_.P[j]);
	    sigma_mo = ntp_.P[j]*SIGMA_ETRK;
	    sigma_e_whole = PredictSigmaE(ntp_.P[j]);
	    ntp_.sigEC_cut[j] = sqrt(pow(sigma_mo,2) + pow(sigma_e_whole,2))*ETOT_EL_CUT;
	    sigma_e_in_out = PredictSigmaEInOut(ntp_.P[j]);
	    ntp_.sigECio_cut[j] = sqrt(pow(sigma_mo,2) + pow(sigma_e_in_out,2))*EIO_EL_CUT;
	    
	    // LAC stuff
	    ntp_.LACstat[j] = cp.lacStatus(); // LAC status
	    ntp_.LAC[j] = cp.lacEtot(); // LAC total energy
	    ntp_.LACin[j] = cp.lacEin();  // LAC inner energy
	    
	    // CC stuff
	    ntp_.CC_time[j] = cp.ccTime(); // CC time
	    ntp_.CCstat[j] = cp.ccStatus();  // CC status
	    clasCChit CCx = cp.CChit();
	    ntp_.CChit_stat[j] = CCx.status(); // CChit class status
	    ntp_.CCnpe[j] = CCx.npe(); // CC number of photoelectrons
	    ntp_.CCtheta[j] = CCx.theta(); // CC polar angle
	    ntp_.CC_QF[j] = cp.ccQF(); // CC quality factor
	    ntp_.CC_CS[j] = cp.CS(); // CC Kossov cut
	    ntp_.CCpmt[j] = CCx.nrPhi(); // left or right side of CC sector
	    ntp_.CCseg[j] = CCx.MeanSegment(); // CC Mean Segment
	    ntp_.CCphimatch[j] = cp.ccPhiMatch(); // CC Phi Match
	    
	    if(TAGR){
	      best_diff = 5.0;
	      for(i=0;i<TAGR->bank.nrow;i++){
		diff = fabs(ntp_.scvT_Pid[j]-TAGR->tagr[i].tpho-ntp_.tprop[j]);
		if(diff<best_diff){
		  best_diff = diff;
		  ntp_.tpho_v2[j] = TAGR->tagr[i].tpho;
		}
	      }
	    }
	    
	    if(ntp_.Pid[j]==2){
	      vT_ep = ntp_.scvT_Pid[j];
	      tpho_ep = ntp_.tpho_v2[j];
	      tprop_ep = ntp_.tprop[j];
	      h400->Fill(ntp_.P[j]);
	      h402->Fill(ntp_.Pz[j]);
	      if(fabs(vT_ep-ntp_.tpho-tprop_ep)>2.0){
		h401->Fill(ntp_.P[j]);
		h403->Fill(ntp_.Pz[j]);
	      }
	    }
	    if(ntp_.Pid[j]==3){
	      vT_em = ntp_.scvT_Pid[j];
	      tpho_em = ntp_.tpho_v2[j];
	      tprop_em = ntp_.tprop[j];
	      h410->Fill(ntp_.P[j]);
	      h412->Fill(ntp_.Pz[j]);
	      if(fabs(vT_em-ntp_.tpho-tprop_em)>2.0){
		h411->Fill(ntp_.P[j]);
		h413->Fill(ntp_.Pz[j]);
	      }	
	    }
	    
	    if(PrintTiming){
	      cout << ntp_.Pid[j];
	      cout.precision(5);
	      cout << "\t" << ntp_.beta[j];
	      cout << "\t" << ntp_.SC_time[j];
	      cout << "\t" << ntp_.scLen[j];
	      cout << "\t" << ntp_.scvT[j];
	      cout << "\t" << ntp_.scvT_Pid[j];
	      cout << "\t\t" << ntp_.scvT_Lep[j];
	      cout << "\t\t" << ntp_.tprop[j];
	      cout << "\t" << ntp_.tpho_v2[j];
	      cout << "\t\t" << ntp_.IsLepG7[j] << endl;
	    }

	    // TOF mass squared check
	    ntp_.TOF_MassSq[j] = cp.scMassSquared();
	    ntp_.TOFstat[j] = ((ntp_.P[j]<MOMTHRESHOLD) && cut(MassSq,MASSSQLO,MASSSQHI));
	    
	    // select lepton events with out-of-time beam buckets
	    if((abs(ntp_.IsLepG7[j])==1) && cut(ntp_.beta[j],0.7,0.925)){
	      OutOfTime = 1;
	    }
	  }	  	    
	  
	  if(fabs(vT_ep - vT_em)>2.0) tdiff_ep_em++;
	  if(fabs(vT_ep - ntp_.tpho - tprop_ep)>2.0) tdiff_ep_gam++;
	  if(fabs(vT_em - ntp_.tpho - tprop_em)>2.0) tdiff_em_gam++;
	  if(fabs(tpho_ep - ntp_.tpho)>2.0){
	    tpho_diff_ep++;
	    if(fabs(vT_ep - tpho_ep - tprop_ep)>2.0) tdiff_ep_new++;
	  }
	  if(fabs(tpho_em - ntp_.tpho)>2.0){
	    tpho_diff_em++;
	    if(fabs(vT_em - tpho_em - tprop_em)>2.0) tdiff_em_new++;
	  }
	  if(fabs(tpho_ep - tpho_em)>2.0) tpho_diff_ep_em++;
	  
	  if(ntp_.Hits){
	    ret = 1;
	    ntp_t2.Fill();  // Fill the tree
	  }
	}//end of vertex selection
      }//end of time selection
    }//end of beam energy cut
  }//end of trig_cut
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

//***************** predicted value of electron energy from EC total ***************
float PredictE(float p){
  if (p < 1.0){
    return(.23*p + .071*p*p - .032*p*p*p);
  }
  return(p/EC_MAGIC_NUMBER);  
}
//**********************************************************************************

//***************** predicted value of electron energy from EC inner and outer *****
float PredictEInOut(float p){
  return(.03 + .105*p - .025*p*p);
}
//**********************************************************************************

//********* predicted value of sigma ofelectron energy from EC total ***************
float PredictSigmaE(float p){
  return(sqrt(p)*(.08 + .0013*p));
}
//**********************************************************************************

//********** predicted value of sigma of electron energy from EC inner and outer ***
float PredictSigmaEInOut(float p){
  return(.0634*p - .00022*p*p);
}
//**********************************************************************************

//****************** print tpho summary ********************************************
void PrintSummary(int Nevents)
{

  cout << "Summary of timing differences:" << endl;
  cout << "dvT(e+,e-)\t" << tdiff_ep_em << "\t" << (float)tdiff_ep_em/(float)Nevents << endl;
  cout << "dvT(e+,old tpho)\t" << tdiff_ep_gam << "\t" << (float)tdiff_ep_gam/(float)Nevents << endl;
  cout << "dvT(e-,old tpho)\t" << tdiff_em_gam << "\t" << (float)tdiff_em_gam/(float)Nevents << endl;
  cout << "dTpho(e+)\t" << tpho_diff_ep << "\t" << (float)tpho_diff_ep/(float)Nevents << endl;
  cout << "dTpho(e-)\t" << tpho_diff_em << "\t" << (float)tpho_diff_em/(float)Nevents << endl;
  cout << "dTpho(e+,e-)\t" << tpho_diff_ep_em << "\t" << (float)tpho_diff_ep_em/(float)Nevents << endl;
  cout<<endl;
  cout << "dvT(e+,new tpho)\t" << tdiff_ep_new << "\t" << (float)tdiff_ep_new/(float)Nevents << endl;
  cout << "dvT(e-,new tpho)\t" << tdiff_em_new << "\t" << (float)tdiff_em_new/(float)Nevents << endl;
  
}
//**********************************************************************************
