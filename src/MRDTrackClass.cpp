/* vim:set noexpandtab tabstop=4 wrap */

#include "MRDTrackClass.hh"
#include "MRDspecs.hh"

#include "TObject.h"
#include "TVector3.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TF1.h"

#include <vector>
#include <cassert>
#include <algorithm>

// Track Getters
// =============
Int_t cMRDTrack::GetTrackID(){return MRDtrackID;}
Int_t cMRDTrack::GetTankTrackID(){return tanktrackID;}
// Locate the track in file>run>event>trigger hierarchy
std::string cMRDTrack::GetFile(){return wcsimfile;}
Int_t cMRDTrack::GetRunID(){return run_id;}
Int_t cMRDTrack::GetEventID(){return event_id;}
Int_t cMRDTrack::GetMrdSubEventID(){return mrdsubevent_id;}
Int_t cMRDTrack::GetTrigger(){return trigger;}

// Top level information about the track
Int_t cMRDTrack::GetNumDigits(){return digi_ids.size();}
Int_t cMRDTrack::GetNumLayersHit(){return layers_hit.size();}
Int_t cMRDTrack::GetNumPMTsHit(){return pmts_hit.size();}
std::vector<Int_t> cMRDTrack::GetDigitIds(){return digi_ids;}
std::vector<Double_t> cMRDTrack::GetDigitQs(){return digi_qs;}
std::vector<Double_t> cMRDTrack::GetDigitTs(){return digi_ts;}
std::vector<Int_t> cMRDTrack::GetDigiNumPhots(){return digi_numphots;}
std::vector<Double_t> cMRDTrack::GetDigiPhotTs(){return digi_phot_ts;}
std::vector<Int_t> cMRDTrack::GetDigiPhotParents(){return digi_phot_parents;}
std::vector<Int_t> cMRDTrack::GetLayersHit(){return layers_hit;}
std::vector<Int_t> cMRDTrack::GetPMTsHit(){return pmts_hit;}

// Reconstructed Variables
std::vector<Double_t> cMRDTrack::GetEdeps(){return eDepsInLayers;}
Double_t cMRDTrack::GetKEStart(){return KEStart;}
Double_t cMRDTrack::GetKEEnd(){return KEEnd;}
Double_t cMRDTrack::GetTotalEdep(){return KEStart-KEEnd;}
Int_t cMRDTrack::GetParticlePID(){return particlePID;}

// Information required for matching to tank tracks
double cMRDTrack::GetTrackAngle(){return trackangle;}
double cMRDTrack::GetTrackAngleError(){return trackangleerror;}
TVector3 cMRDTrack::GetMrdEntryPoint(){return mrdentrypoint;}
std::pair<double, double> cMRDTrack::GetMrdEntryBoundsX(){return mrdentryxbounds;}
std::pair<double, double> cMRDTrack::GetMrdEntryBoundsY(){return mrdentryybounds;}
bool cMRDTrack::GetInterceptsTank(){return interceptstank;}
TVector3 cMRDTrack::GetTankExitPoint(){return projectedtankexitpoint;}
double cMRDTrack::GetStartTime(){
	auto minelementit = std::min_element(digi_ts.begin(), digi_ts.end());
	if(minelementit!=digi_ts.end()) return *minelementit;
	return -1.;
}

TVector3 cMRDTrack::GetStartVertex(){return trackfitstart;}
TVector3 cMRDTrack::GetStopVertex(){return trackfitstop;}
double cMRDTrack::GetTrackLength(){return mutracklengthinMRD;}
double cMRDTrack::GetEnergyLoss(){return EnergyLoss;}
double cMRDTrack::GetEnergyLossError(){return EnergyLossError;}
bool cMRDTrack::GetIsPenetrating(){return ispenetrating;}
bool cMRDTrack::GetIsStopped(){return isstopped;}
bool cMRDTrack::GetIsSideExit(){return sideexit;}

Double_t cMRDTrack::GetPenetrationDepth(){return penetrationdepth;}
//WCSimRootTrack* cMRDTrack::GetTrueTrack(){return trueTrack;}
Double_t cMRDTrack::GetHtrackOrigin(){return htrackorigin;}
Double_t cMRDTrack::GetHtrackOriginError(){return htrackoriginerror;}
Double_t cMRDTrack::GetHtrackGradient(){return htrackgradient;}
Double_t cMRDTrack::GetHtrackGradientError(){return htrackgradienterror;}
Double_t cMRDTrack::GetHtrackFitChi2(){return htrackfitchi2;}
std::array<double,2> cMRDTrack::GetHtrackFitCov(){return std::array<double,2>{htrackfitcov(0,0),htrackfitcov(0,1)};}
Double_t cMRDTrack::GetVtrackOrigin(){return vtrackorigin;}
Double_t cMRDTrack::GetVtrackOriginError(){return vtrackoriginerror;}
Double_t cMRDTrack::GetVtrackGradient(){return vtrackgradient;}
Double_t cMRDTrack::GetVtrackGradientError(){return vtrackgradienterror;}
Double_t cMRDTrack::GetVtrackFitChi2(){return vtrackfitchi2;}
std::array<double,2> cMRDTrack::GetVtrackFitCov(){return std::array<double,2>{vtrackfitcov(0,0),vtrackfitcov(0,1)};}


// Truth Level Info
Int_t cMRDTrack::GetTrueTrackID(){return trueTrackID;}
//	WCSimRootTrack* cMRDTrack::GetTrueTrack(){return trueTrack;}

//	// Digit Level Getters - all the obtainable information about a digit. Or just return the digit?
//	// ===============================
//	Int_t cMRDTrack::GetPMTNumber(Int_t digitnum){return PMTnum.at(digitnum);}
//	Double_t cMRDTrack::GetTime(Int_t digitnum){return DigitTs.at(digitnum);}
//	Double_t cMRDTrack::GetCharge(Int_t digitnum){return DigitQs.at(digitnum);}
//	Double_t cMRDTrack::GetEdeposited(Int_t digitnum){ return DigitQs.at(digitnum)*3;}
//	Int_t cMRDTrack::GetNumTrueHits(Int_t digitnum){return NumTrueHits.at(digitnum);}
//	std::vector<Int_t> cMRDTrack::GetPhotonIds(Int_t digitnum){return PhotonIds.at(digitnum);}
//	std::vector<Int_t> cMRDTrack::GetPhotonParents(Int_t digitnum){return PhotonParents.at(digitnum);}
//	std::vector<Double_t> cMRDTrack::GetPhotonTrueTimes(Int_t digitnum){return PhotonTrueTimes.at(digitnum);}
//	WCSimRootCherenkovDigiHit* cMRDTrack::GetDigit(Int_t i){
//		try{return &(digits.at(i));}
//		catch(const std::out_of_range& oor){return 0;}
//	}

//	// ---------------------------
//	// digit information not in WCSimRootCherenkovDigiHit
//	Int_t cMRDTrack::GetLayerNum(Int_t digitnum){return MRDlayers.at(digitnum);}
//	Int_t cMRDTrack::GetPaddleNum(Int_t digitnum){return MRDpaddles.at(digitnum);}
//	std::pair<Double_t, Double_t> cMRDTrack::GetXrange(Int_t digitnum){return xranges.at(digitnum);}
//	std::pair<Double_t, Double_t> cMRDTrack::GetYrange(Int_t digitnum){return yranges.at(digitnum);}
//	std::pair<Double_t, Double_t> cMRDTrack::GetZrange(Int_t digitnum){return zranges.at(digitnum);}
//	std::pair<Double_t, Double_t> cMRDTrack::GetTrange(Int_t digitnum){return tranges.at(digitnum);}
//	// ---------------------------

// Track Setters
// ==============
//	void cMRDTrack::AppendDigit(WCSimRootCherenkovDigiHit digitin){digits.push_back(digitin);}
void cMRDTrack::SetTankTrack(Int_t trackidin){tanktrackID = trackidin;}

// Default Constructor
// ====================
// Default constructor that initialises all private members required for ROOT classes
cMRDTrack::cMRDTrack() : MRDtrackID(-1), wcsimfile(""), run_id(-1), event_id(-1), trigger(-1), mrdsubevent_id(-1), digi_ids(), pmts_hit(), digi_qs(), digi_ts(), digi_numphots(), digi_phot_ts(), digi_phot_parents(), tanktrackID(-1), layers_hit(), eDepsInLayers(), KEStart(-1.), KEEnd(-1.), particlePID(-1), trueTrackID(-1), htrackcells(), vtrackcells(), htrackorigin(-1), htrackoriginerror(-1), htrackgradient(-1), htrackgradienterror(-1), htrackfitchi2(-1), vtrackorigin(-1), vtrackoriginerror(-1), vtrackgradient(-1), vtrackgradienterror(-1), vtrackfitchi2(-1), trackfitstart(TVector3(0,0,0)), trackfitstop(TVector3(0,0,0)), ispenetrating(false), isstopped(false), sideexit(false), penetrationdepth(-1), EnergyLoss(-1), projectedtankexitpoint(TVector3(0,0,0)), interceptstank(false), trackangle(-1.), EnergyLossError(-1), trackangleerror(-1), mrdentryxbounds(), mrdentryybounds(), extravpoints(), extravpointerrors(), extrahpoints(), extrahpointerrors(), extrazpoints(), extrazpointerrors() {};

// destructor
cMRDTrack::~cMRDTrack(){/*std::cout<<"cMRDTrack destructor (no actions here)"<<std::endl;*/}

// Actual Constructor
// ==================
cMRDTrack::cMRDTrack(Int_t mrdtrackidin, std::string wcsimefilein, Int_t runidin, Int_t eventidin, Int_t subeventidin,
Int_t triggerin, std::vector<Int_t> digitidsin, std::vector<Int_t> digittubesin, std::vector<Double_t>
digitqsin, std::vector<Double_t> digittimesin, std::vector<Int_t> digitnumphotsin, std::vector<Double_t> 
digitstruetimesin, std::vector<Int_t> digitsparentsin, std::vector<mrdcell> htrackcellsin, 
std::vector<mrdcell> vtrackcellsin, std::vector<mrdcluster> htrackclustersin, std::vector<mrdcluster> vtrackclustersin) :
/* information retrieved when creating the track: initialize with input */
MRDtrackID(mrdtrackidin), wcsimfile(wcsimefilein), run_id(runidin), event_id(eventidin),
mrdsubevent_id(subeventidin), trigger(triggerin), digi_ids(digitidsin), pmts_hit(digittubesin),
digi_qs(digitqsin), digi_ts(digittimesin), digi_numphots(digitnumphotsin), digi_phot_ts(digitstruetimesin),
digi_phot_parents(digitsparentsin), htrackcells(htrackcellsin), vtrackcells(vtrackcellsin),
htrackclusters(htrackclustersin), vtrackclusters(vtrackclustersin),
/* information calculated: initialize to default */
tanktrackID(-1), layers_hit(), KEStart(-1.), KEEnd(-1.), particlePID(-1),
trueTrackID(-1), htrackorigin(-1), htrackoriginerror(-1), htrackgradient(-1),
htrackgradienterror(-1), htrackfitchi2(-1), vtrackorigin(-1), vtrackoriginerror(-1), vtrackgradient(-1),
vtrackgradienterror(-1), vtrackfitchi2(-1), trackfitstart(TVector3(0,0,0)), 
trackfitstop(TVector3(0,0,0)), ispenetrating(false), isstopped(false), sideexit(false),
penetrationdepth(-1), EnergyLoss(-1), projectedtankexitpoint(TVector3(0,0,0)), interceptstank(false),
trackangle(-1.), EnergyLossError(-1), trackangleerror(-1), mrdentryxbounds(), mrdentryybounds(),
extravpoints(), extravpointerrors(), extrahpoints(),
extrahpointerrors(), extrazpoints(), extrazpointerrors() {
	
	if(fillstaticmembers){
		MRDenergyvspenetration.SetParameters(-3.62645, 3.75503, 2.68525, 3.59244, 1.66969); // TODO MOVE!
		fillstaticmembers=false;
	}
	
#ifdef MRDTrack_VERBOSE
	std::cout<<std::endl<<"constructing a track with "<<digi_ids.size()<<" digits"<<std::endl;
#endif
	
	eDepsInLayers.assign(MRDSpecs::numpanels, 0.);  // can't assign the size in the class def. 
	DoReconstruction();
	
	if((ispenetrating&&isstopped)||(ispenetrating&&sideexit)||(isstopped&&sideexit)){
		std::cerr<<"ERROR!"<<std::endl<<"ispenetrating="<<ispenetrating<<", isstopped="<<isstopped<<", sideexit="<<sideexit<<std::endl;
		assert(false);
	}
	if(trackfitstart.X()<1.&&trackfitstart.Y()<1&&trackfitstart.Z()<1){
		std::cerr<<"POSSIBLE ERROR!"<<std::endl<<"Track fit start = ("
			<<trackfitstart.X()<<", "<<trackfitstart.Y()<<", "<<trackfitstart.Z()<<")"<<std::endl;
		assert(false);
	}
	if(trackfitstop.X()<1.&&trackfitstop.Y()<1&&trackfitstop.Z()<1){
		std::cerr<<"POSSIBLE ERROR!"<<std::endl<<"Track fit stop = ("
			<<trackfitstop.X()<<", "<<trackfitstop.Y()<<", "<<trackfitstop.Z()<<")"<<std::endl;
		assert(false);
	}
}

// Copy Constructor
// ================
cMRDTrack::cMRDTrack(cMRDTrack const &trackin) :
MRDtrackID(trackin.MRDtrackID), wcsimfile(trackin.wcsimfile), run_id(trackin.run_id),
event_id(trackin.event_id), mrdsubevent_id(trackin.mrdsubevent_id), trigger(trackin.trigger),
digi_ids(trackin.digi_ids), pmts_hit(trackin.pmts_hit), digi_qs(trackin.digi_qs), digi_ts(trackin.digi_ts),
digi_numphots(trackin.digi_numphots), digi_phot_ts(trackin.digi_phot_ts),
digi_phot_parents(trackin.digi_phot_parents), tanktrackID(trackin.tanktrackID), 
layers_hit(trackin.layers_hit), eDepsInLayers(trackin.eDepsInLayers), KEStart(trackin.KEStart),
KEEnd(trackin.KEEnd), particlePID(trackin.particlePID),
trueTrackID(trackin.trueTrackID), htrackcells(trackin.htrackcells), vtrackcells(trackin.vtrackcells), 
htrackclusters(trackin.htrackclusters), vtrackclusters(trackin.vtrackclusters),
htrackorigin(trackin.htrackorigin), htrackoriginerror(trackin. htrackoriginerror),
htrackgradient(trackin. htrackgradient), htrackgradienterror(trackin. htrackgradienterror),
htrackfitchi2(trackin. htrackfitchi2), vtrackorigin(trackin. vtrackorigin),
vtrackoriginerror(trackin. vtrackoriginerror), vtrackgradient(trackin. vtrackgradient),
vtrackgradienterror(trackin. vtrackgradienterror), vtrackfitchi2(trackin. vtrackfitchi2),
trackfitstart(trackin.trackfitstart), trackfitstop(trackin.trackfitstop), 
ispenetrating(trackin.ispenetrating), isstopped(trackin.isstopped), sideexit(trackin.sideexit),
penetrationdepth(trackin.penetrationdepth), EnergyLoss(trackin.EnergyLoss), 
projectedtankexitpoint(trackin.projectedtankexitpoint), interceptstank(trackin.interceptstank),
trackangle(trackin.trackangle), EnergyLossError(trackin.EnergyLossError), 
trackangleerror(trackin.trackangleerror), mrdentryxbounds(trackin.mrdentryxbounds), 
mrdentryybounds(trackin.mrdentryybounds), extravpoints(trackin.extravpoints), 
extravpointerrors(trackin.extravpointerrors), extrahpoints(trackin.extrahpoints), 
extrahpointerrors(trackin.extrahpointerrors), extrazpoints(trackin.extrazpoints), 
extrazpointerrors(trackin.extrazpointerrors)
/*, digits(trackin.digits), trueTrack(trackin.trueTrack)*/ 
{
#ifdef MRDTrack_VERBOSE
	std::cout<<std::endl<<"copy constructing a track with "<<digi_ids.size()<<" digits"<<std::endl;
#endif
}

// Set Static Members
// ==================
TF1 cMRDTrack::MRDenergyvspenetration=TF1("af","expo(0)+pol0(2)+([3]/([4]-x))",0,1.6);
// TODO FIXME these are coded in the constructor! Move them elsewhere - a new static?
//  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
//   1  p0          -3.62645e+00   2.40923e+01   2.22710e-04   8.81482e-04
//   2  p1           3.75503e+00   1.56841e+01   1.89877e-04   8.82181e-04
//   3  p2           2.68525e+00   7.79532e+00   1.11828e-04   6.30502e-04
//   4  p3           3.59244e+00   1.46368e+01   1.30963e-04   1.08201e-03
//   5  p4           1.66969e+00   6.49103e-01   2.51472e-05  -6.46072e-03
Bool_t cMRDTrack::fillstaticmembers=true;

