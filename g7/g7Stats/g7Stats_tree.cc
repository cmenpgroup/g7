extern "C" {
#include <stdlib.h>                                 /* standard header files */
#include <stdio.h>
#include <math.h>
}
#include <g7Stats.h>

void g7Stats_tree(){

  ntp_t2.Branch("ebeam",&nt_.ebeam,"ebeam/F");

  ntp_t2.Branch("nEvents",&ntp_.nEvents,"nEvents/I");
  ntp_t2.Branch("Hits",&ntp_.Hits,"Hits/I");
  ntp_t2.Branch("Q",&ntp_.Q,"Q[Hits]/I");
  ntp_t2.Branch("Pid",&ntp_.Pid,"Pid[Hits]/I");
  ntp_t2.Branch("IsLep",&ntp_.IsLep,"IsLep[Hits]/I");
  ntp_t2.Branch("MaybeLep",&ntp_.MaybeLep,"MaybeLep[Hits]/I");
  ntp_t2.Branch("nIsLep",&ntp_.nIsLep,"nIsLep/I");
  ntp_t2.Branch("nMaybeLep",&ntp_.nMaybeLep,"nMaybeLep/I");
  ntp_t2.Branch("NnegLep",&ntp_.NnegLep,"NnegLep/I");
  ntp_t2.Branch("negSec",&ntp_.negSec,"negSec[NnegLep]/I");
  ntp_t2.Branch("negLepEC",&ntp_.negLepEC,"negLepEC[NnegLep]/I");
  ntp_t2.Branch("negLepCC",&ntp_.negLepCC,"negLepCC[NnegLep]/I");
  ntp_t2.Branch("negLepTOF",&ntp_.negLepTOF,"negLepTOF[NnegLep]/I");
  ntp_t2.Branch("negGold",&ntp_.negGold,"negGold[NnegLep]/I");
  ntp_t2.Branch("NposLep",&ntp_.NposLep,"NposLep/I");
  ntp_t2.Branch("posSec",&ntp_.posSec,"posSec[NposLep]/I");
  ntp_t2.Branch("posLepEC",&ntp_.posLepEC,"posLepEC[NposLep]/I");
  ntp_t2.Branch("posLepCC",&ntp_.posLepCC,"posLepCC[NposLep]/I");
  ntp_t2.Branch("posLepTOF",&ntp_.posLepTOF,"posLepTOF[NposLep]/I");
  ntp_t2.Branch("posGold",&ntp_.posGold,"posGold[NposLep]/I");
  ntp_t2.Branch("Tracks",&ntp_.Tracks,"Tracks/I");
  ntp_t2.Branch("trig",&ntp_.trig,"trig/I");
  ntp_t2.Branch("ntrig",&ntp_.ntrig,"ntrig/I");
  ntp_t2.Branch("tbit",&ntp_.tbit,"tbit[ntrig]/I");

  ntp_t2.Branch("vx",&ntv_.vx,"vx/F");
  ntp_t2.Branch("vy",&ntv_.vy,"vy/F");
  ntp_t2.Branch("vz",&ntv_.vz,"vz/F");

  /* setup histograms */
  h201 = new TH1F("h201","vertex x",100,-5.,5.);
  h202 = new TH1F("h202","vertex y",100,-5.,5.);
  h203 = new TH1F("h203","vertex z",200,-13.,13.);

  h316 = new TH1F("h316","photon energy",300,1.,6.);

}





