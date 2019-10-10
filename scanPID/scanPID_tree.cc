extern "C" {
#include <stdlib.h>                                 /* standard header files */
#include <stdio.h>
#include <math.h>
}
#include <scanPID.h>

void scanPID_tree(){

  int i;
  char name[100];
  char title[100];

  ntp_t2.Branch("egam",&nt_.egam,"egam/F");
  ntp_t2.Branch("Event",&nt_.Event,"Event/I");
  ntp_t2.Branch("Tracks",&nt_.Tracks,"Tracks/I");
  ntp_t2.Branch("trig",&nt_.trig,"trig/I");

  ntp_t2.Branch("tpho",&ntp_.tpho,"tpho/F");
  ntp_t2.Branch("HitsEVNT",&ntp_.HitsEVNT,"HitsEVNT/I");
  ntp_t2.Branch("Pid_EVNT_pdg",&ntp_.Pid_EVNT_pdg,"Pid_EVNT_pdg[HitsEVNT]/I");
  ntp_t2.Branch("Pid_EVNT_geant",&ntp_.Pid_EVNT_geant,"Pid_EVNT_geant[HitsEVNT]/I");

  ntp_t2.Branch("Hits",&ntp_.Hits,"Hits/I");
  ntp_t2.Branch("Pid",&ntp_.Pid,"Pid[Hits]/I");
  ntp_t2.Branch("beta",&ntp_.beta,"beta[Hits]/F");
  ntp_t2.Branch("beta_Pid",&ntp_.beta_Pid,"beta_Pid[Hits]/F");
  ntp_t2.Branch("Q",&ntp_.Q,"Q[Hits]/F");
  ntp_t2.Branch("Mass",&ntp_.Mass,"Mass[Hits]/F");
  ntp_t2.Branch("P",&ntp_.P,"P[Hits]/F");
  ntp_t2.Branch("Pz",&ntp_.Pz,"Pz[Hits]/F");
  ntp_t2.Branch("Sec",&ntp_.Sec,"Sec[Hits]/F");
  ntp_t2.Branch("tprop",&ntp_.tprop,"tprop[Hits]/F");
  ntp_t2.Branch("tpho_v2",&ntp_.tpho_v2,"tpho_v2[Hits]/F");
  ntp_t2.Branch("phi",&ntp_.phi,"phi[Hits]/F");
  ntp_t2.Branch("theta",&ntp_.theta,"theta[Hits]/F");
  ntp_t2.Branch("fidcut",&ntp_.fidcut,"fidcut[Hits]/I");
  ntp_t2.Branch("trigPart",&ntp_.trigPart,"trigPart[Hits]/I");
  ntp_t2.Branch("IsLep",&ntp_.IsLep,"IsLep[Hits]/I");
  ntp_t2.Branch("IsLepG7",&ntp_.IsLepG7,"IsLepG7[Hits]/I");
  ntp_t2.Branch("IsLepG7ec",&ntp_.IsLepG7ec,"IsLepG7ec[Hits]/I");
  ntp_t2.Branch("IsLepG7ect",&ntp_.IsLepG7ect,"IsLepG7ect[Hits]/I");
  ntp_t2.Branch("IsLepG7ecio",&ntp_.IsLepG7ecio,"IsLepG7ecio[Hits]/I");
  ntp_t2.Branch("IsLepG7ecr1",&ntp_.IsLepG7ecr1,"IsLepG7ecr1[Hits]/I");
  ntp_t2.Branch("IsLepG7ecr2",&ntp_.IsLepG7ecr2,"IsLepG7ecr2[Hits]/I");
  ntp_t2.Branch("IsLepG7cc",&ntp_.IsLepG7cc,"IsLepG7cc[Hits]/I");
  ntp_t2.Branch("IsLepG7mm2",&ntp_.IsLepG7mm2,"IsLepG7mm2[Hits]/I");
  ntp_t2.Branch("MaybeLepG7",&ntp_.MaybeLepG7,"MaybeLepG7[Hits]/I");
  ntp_t2.Branch("TOFstat",&ntp_.TOFstat,"TOFstat[Hits]/I");
  ntp_t2.Branch("SC_time",&ntp_.SC_time,"SC_time[Hits]/F");
  ntp_t2.Branch("scvT",&ntp_.scvT,"scvT[Hits]/F");
  ntp_t2.Branch("scvT_Pid",&ntp_.scvT_Pid,"scvT_Pid[Hits]/F");
  ntp_t2.Branch("scvT_Lep",&ntp_.scvT_Lep,"scvT_Lep[Hits]/F");
  ntp_t2.Branch("scLen",&ntp_.scLen,"scLen[Hits]/F");
  ntp_t2.Branch("scId",&ntp_.scId,"scId[Hits]/I");
  ntp_t2.Branch("TOF_MassSq",&ntp_.TOF_MassSq,"TOF_MassSq[Hits]/F");
  ntp_t2.Branch("ECstat",&ntp_.ECstat,"ECstat[Hits]/I");
  ntp_t2.Branch("EC_time",&ntp_.EC_time,"EC_time[Hits]/F");
  ntp_t2.Branch("EC",&ntp_.EC,"EC[Hits]/F");
  ntp_t2.Branch("ECin",&ntp_.ECin,"ECin[Hits]/F");
  ntp_t2.Branch("ECout",&ntp_.ECout,"ECout[Hits]/F");
  ntp_t2.Branch("ECx",&ntp_.ECx,"ECx[Hits]/F");
  ntp_t2.Branch("ECy",&ntp_.ECy,"ECy[Hits]/F");
  ntp_t2.Branch("ECz",&ntp_.ECz,"ECz[Hits]/F");
  ntp_t2.Branch("ECfid",&ntp_.ECfid,"ECfid[Hits]/I");
  ntp_t2.Branch("ECisHit",&ntp_.ECisHit,"ECisHit[Hits]/I");
  ntp_t2.Branch("ECpre",&ntp_.ECpre,"ECpre[Hits]/F");
  ntp_t2.Branch("ECiopre",&ntp_.ECiopre,"ECiopre[Hits]/F");
  ntp_t2.Branch("sigEC_cut",&ntp_.sigEC_cut,"sigEC_cut[Hits]/F");
  ntp_t2.Branch("sigECio_cut",&ntp_.sigECio_cut,"sigECio_cut[Hits]/F");
  ntp_t2.Branch("LACstat",&ntp_.LACstat,"LACstat[Hits]/I");
  ntp_t2.Branch("LAC",&ntp_.LAC,"LAC[Hits]/F");
  ntp_t2.Branch("LACin",&ntp_.LACin,"LACin[Hits]/F");
  ntp_t2.Branch("CCstat",&ntp_.CCstat,"CCstat[Hits]/I");
  ntp_t2.Branch("CC_time",&ntp_.CC_time,"CC_time[Hits]/F");
  ntp_t2.Branch("CChit_stat",&ntp_.CChit_stat,"CChit_stat[Hits]/I");
  ntp_t2.Branch("CCnpe",&ntp_.CCnpe,"CCnpe[Hits]/F");
  ntp_t2.Branch("CCtheta",&ntp_.CCtheta,"CCtheta[Hits]/F");
  ntp_t2.Branch("CC_QF",&ntp_.CC_QF,"CC_QF[Hits]/F");
  ntp_t2.Branch("CC_CS",&ntp_.CC_CS,"CC_CS[Hits]/F");
  ntp_t2.Branch("CCseg",&ntp_.CCseg,"CCseg[Hits]/I");
  ntp_t2.Branch("CCpmt",&ntp_.CCpmt,"CCpmt[Hits]/I");
  ntp_t2.Branch("CCphimatch",&ntp_.CCphimatch,"CCphimatch[Hits]/I");

  ntp_t2.Branch("vx",&ntv_.vx,"vx/F");
  ntp_t2.Branch("vy",&ntv_.vy,"vy/F");
  ntp_t2.Branch("vz",&ntv_.vz,"vz/F");

  /* setup histograms */
  h110 = new TH1F("h110","Number of Tracks",7,-0.5,6.5);

  h201 = new TH1F("h201","vertex x",100,-5.,5.);
  h202 = new TH1F("h202","vertex y",100,-5.,5.);
  h203 = new TH1F("h203","vertex z",200,-30.,20.);

  h316 = new TH1F("h316","photon energy",300,1.,6.);
  h317 = new TH1F("h317","tpho",600,-100.0,200.0);

  h350 = new TH1F("h350","#phi (deg.)",360,-180.0,180.0);

  h370 = new TH1F("h370","IsLeptonG7",5,-2.5,2.5);

  h400 = new TH1F("h400","e+ Momentum",150,0.0,3.0);
  h401 = new TH1F("h401","e+ Momentum, vTime cut",150,0.0,3.0);
  h402 = new TH1F("h402","e+ Pz",150,0.0,3.0);
  h403 = new TH1F("h403","e+ Pz, vTime cut",150,0.0,3.0);
  h410 = new TH1F("h410","e- Momentum",150,0.0,3.0);
  h411 = new TH1F("h411","e- Momentum, vTime cut",150,0.0,3.0);
  h412 = new TH1F("h412","e- Pz",150,0.0,3.0);
  h413 = new TH1F("h413","e- Pz, vTime cut",150,0.0,3.0);

}





