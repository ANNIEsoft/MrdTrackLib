/* vim:set noexpandtab tabstop=4 wrap */
#ifndef _MRDTrack_Class_
#define _MRDTrack_Class_ 

#ifndef MRDTrack_VERBOSE
//#define MRDTrack_VERBOSE
#endif

#include "MrdCell.hh"
#include "MrdCluster.hh"

#include "TObject.h"
#include "TVector3.h"
#include "TMatrixDSym.h"

#include <vector>
//#include <array>

class TBox;
class TCanvas;
class TF1;
class TArrow;

class cMRDTrack : public TObject {			// TObject inheritance is required to put in TClonesArray
	
	// Private members
	// ===============
	public:	//TODO public for copy constructor
	Int_t MRDtrackID;						// ID of this track within the trigger
	Int_t tanktrackID;						// correlated tank track within the trigger
	
	// Raw Info:
	std::string wcsimfile;					// which wcsim file this was in
	Int_t run_id;							// which run this file was in TODO all 0
	Int_t event_id;							// which event this track was in
	Int_t trigger;							// which (sub)trigger this track was in TODO? all 1
	Int_t mrdsubevent_id;					// which subevent this track was in
	std::vector<Int_t> digi_ids;			// vector of digi ids: GetCrnkvDigiHits()->At(digi_ids.at(i)) TODO v
	std::vector<Int_t> pmts_hit;			// vector of PMT IDs TODO empty
	std::vector<Double_t> digi_qs;			// vector of digit charges TODO empty
	std::vector<Double_t> digi_ts;			// vector of digit times TODO empty
	std::vector<Int_t> digi_numphots;		// number of true photons for each digit TODO empty
	std::vector<Double_t> digi_phot_ts;		// true hit times of photons in a digit TODO empty
	std::vector<Int_t> digi_phot_parents;	// wcsim track IDs of parents that provided photons for a digit TODO
//	std::vector<WCSimRootCherenkovDigiHit> digits;
	
	// Calculated/Reconstructed Info
	std::vector<Int_t> layers_hit;			// vector of MRD layers hit TODO empty
	std::vector<Double_t> eDepsInLayers;	// fixed length vector of energy deposited in each layer TODO empty
	Double_t KEStart;						// from depth or estimate if fully penetrating - TODO calculate
	Double_t KEEnd;							// 0 for stopping, or estimate if fully penetrating TODO cannot know?
	Double_t mutracklengthinMRD;			// absolute track length from front to end, from track fit
	Double_t penetrationdepth;				// z component of penetration: start of MRD to last hit paddle's z.
	Double_t EnergyLoss;					// based on penetration
	Double_t EnergyLossError;				// based on error in angle and impact on dE/dx
	Int_t particlePID;						// estimated PID
	std::vector<mrdcluster> htrackclusters;	// generated by CA algorithm, used for printing and drawing
	std::vector<mrdcluster> vtrackclusters;	// 
	std::vector<mrdcell> htrackcells;		// cells between horizontally aligned paddles
	std::vector<mrdcell> vtrackcells;		//               vertically 
	
	// Truth information:
	Int_t trueTrackID;						// index in the WCSimRootTrack clones array TODO fill
//	WCSimRootTrack* trueTrack;				// from WCSim GetTracks. Should we keep this?

	// Additional points to include in the fit from bonsia / vertex:
	std::vector<double> extravpoints;
	std::vector<double> extravpointerrors;
	std::vector<double> extrahpoints;
	std::vector<double> extrahpointerrors;
	std::vector<double> extrazpoints;
	std::vector<double> extrazpointerrors;
	
	// Track fit result; fit is a pol1:
	Double_t htrackorigin;
	Double_t htrackoriginerror;
	Double_t htrackgradient;
	Double_t htrackgradienterror;
	Double_t htrackfitchi2;
	TMatrixDSym htrackfitcov; // it's necessary to specify the dimensionality when constructing!!!!
	Double_t vtrackorigin;
	Double_t vtrackoriginerror;
	Double_t vtrackgradient;
	Double_t vtrackgradienterror;
	Double_t vtrackfitchi2;
	TMatrixDSym vtrackfitcov;
	
	TVector3 trackfitstart, trackfitstop;
	double trackangle, trackangleerror;
	bool ispenetrating, isstopped, sideexit;
	TVector3 mrdentrypoint; // this is the start point of the fit, not projected to MRD start
	std::pair<double, double> mrdentryxbounds, mrdentryybounds;
	
	bool interceptstank;
	TVector3 projectedtankexitpoint;
	
	public:
	// Track Level Getters
	// ===================
	Int_t GetTrackID();
	Int_t GetTankTrackID();
	// Locate the track in file>run>event>trigger hierarchy
	std::string GetFile();
	Int_t GetRunID();
	Int_t GetEventID();
	Int_t GetMrdSubEventID();
	Int_t GetTrigger();
	
	// Top level information about the track
	Int_t GetNumDigits();
	Int_t GetNumLayersHit();
	Int_t GetNumPMTsHit();
	std::vector<Int_t> GetDigitIds();
	std::vector<Double_t> GetDigitQs();
	std::vector<Double_t> GetDigitTs();
	std::vector<Int_t> GetDigiNumPhots();
	std::vector<Double_t> GetDigiPhotTs();
	std::vector<Int_t> GetDigiPhotParents();
	std::vector<Int_t> GetLayersHit();
	std::vector<Int_t> GetPMTsHit();
	
	// Reconstructed Variables
	std::vector<Double_t> GetEdeps();
	Double_t GetKEStart();
	Double_t GetKEEnd();
	Double_t GetTotalEdep();
	Int_t GetParticlePID();
	//TODO: getters for fitparameters
	
	// Information required for matching to tank tracks
	double GetTrackAngle();
	double GetTrackAngleError();
	void GetAngleAndOffset(int tracktype, double &xgradient, double &ygradient, double &xoffset, double &yoffset);
	std::pair<double,double> CalculateCovariantError(double errorx, int indexgiven, TMatrixDSym covariancematrix, bool flag);
	double CalculateCovariantError(double errorx, int indexgiven, TMatrixDSym covariancematrix);
	TVector3 GetMrdEntryPoint();
	std::pair<double, double> GetMrdEntryBoundsX();
	std::pair<double, double> GetMrdEntryBoundsY();
	bool GetInterceptsTank();
	TVector3 GetTankExitPoint();
	double GetStartTime();
	TVector3 GetStartVertex();
	TVector3 GetStopVertex();
	double GetTrackLength();
	double GetEnergyLoss();
	double GetEnergyLossError();
	bool GetIsPenetrating();
	bool GetIsStopped();
	bool GetIsSideExit();
	void GetProjectionLimits(double zplane, double &xmax, double &xmin, double &ymax, double &ymin);
	TVector3 GetProjectedPoint(double zplane);
	
	Double_t GetPenetrationDepth();
	//WCSimRootTrack* GetTrueTrack();
	Double_t GetHtrackOrigin();
	Double_t GetHtrackOriginError();
	Double_t GetHtrackGradient();
	Double_t GetHtrackGradientError();
	Double_t GetHtrackFitChi2();
	std::vector<double> GetHtrackFitCov();
	Double_t GetVtrackOrigin();
	Double_t GetVtrackOriginError();
	Double_t GetVtrackGradient();
	Double_t GetVtrackGradientError();
	Double_t GetVtrackFitChi2();
	std::vector<double> GetVtrackFitCov();
	
	
	// debugging functions
	void TestCovarianceErrorCalc();
	
	// provided in MRDTrack_Draw_Print.cxx: draw cell arrows from reconstructio
	void DrawReco(TCanvas* imgcanvas, std::vector<TArrow*> &trackarrows, EColor thistrackscolour, std::vector<TBox*> paddlepointers);
	// draw fit line - copy of code to draw truth lines
	void DrawFit(TCanvas* imgcanvas, std::vector<TArrow*> &trackfitarrows, EColor thistrackscolour);
	// print CA info and track info
	void Print();
	void Print2();
	
	// Truth Level Info
	Int_t GetTrueTrackID();
//	WCSimRootTrack* GetTrueTrack();
	
//	// Digit Level Getters - all the obtainable information about a digit. Or just return the digit?
//	// ===============================
//	Int_t GetPMTNumber(Int_t digitnum);
//	Double_t GetTime(Int_t digitnum);
//	Double_t GetCharge(Int_t digitnum);
//	Double_t GetEdeposited(Int_t digitnum);	//TODO derive from above. units?
//	Int_t GetNumTrueHits(Int_t digitnum);	//size of PhotonIds()
//	std::vector<Int_t> GetPhotonIds(Int_t digitnum);
//	std::vector<Int_t> GetPhotonParents(Int_t digitnum);
//	std::vector<Double_t> GetPhotonTrueTimes(Int_t digitnum);
//	WCSimRootCherenkovDigiHit* GetDigit(Int_t i);
	
//	// ---------------------------
//	// digit information not in WCSimRootCherenkovDigiHit
//	// Geometric information about a given digit
//	std::vector<Int_t> MRDlayers;
//	std::vector<Int_t> MRDpaddles;
//	std::vector<std::pair<Double_t, Double_t> > xranges;	// from width of panel(s) hit
//	std::vector<std::pair<Double_t, Double_t> > yranges;	// 
//	std::vector<std::pair<Double_t, Double_t> > zranges;	// from depth of panel
//	std::vector<std::pair<Double_t, Double_t> > tranges; 	// from uncertainty in PMT timing resoluton
//	// ---------------------------
//	Int_t GetLayerNum(Int_t digitnum);
//	Int_t GetPaddleNum(Int_t digitnum);	// number of paddle within this panel
//	std::pair<Double_t, Double_t> GetXrange(Int_t digitnum);
//	std::pair<Double_t, Double_t> GetYrange(Int_t digitnum);
//	std::pair<Double_t, Double_t> GetZrange(Int_t digitnum);
//	std::pair<Double_t, Double_t> GetTrange(Int_t digitnum);
//	// ---------------------------
	
	// "Setters"
	// =========
//	void AppendDigit(WCSimRootCherenkovDigiHit digitin);
//	void AppendDigit(thisdigitstime, thisdigitsq, thisdigitstubeid, photontimesinatrack, particleidsinatrack);
	void SetTankTrack(Int_t trackidin);
	void AddTrackPoint(TVector3 pointposition, TVector3 pointerror); // add a point from bonsai, or other external position constraint
	
	// Functions to do reconstruction
	// ==============================
	// Main track reconstruction code
	void DoReconstruction();
	bool CheckTankIntercept(double htrackgradientin, double vtrackgradientin, double htrackoriginin, 
						double vtrackoriginin, TVector3* solution1, TVector3* solution2);
	bool CheckTankIntercept(TVector3* entrypoint, TVector3* exitpoint, int tracktype);
	double GetClosestApproach(TVector3 pointin, int tracktype);
	TVector3 GetClosestPoint(TVector3 origin, int tracktype);
	void DoTGraphErrorsFit();
	void CheckIfStopping();
	void CalculateEnergyLoss();
	
	private:
	// void CalculateParticlePID();  // based on rate of loss? num tracks..? penetration? tank? 
	
	// Static Members
	// ==============
	static Bool_t fillstaticmembers;
	static TF1 MRDenergyvspenetration;
	
	// Default Constructor
	// ====================
	public:
	// Default constructor that initialises all private members required for ROOT classes
	cMRDTrack();
	
	// destructor
	~cMRDTrack();
	
	// Actual Constructor
	// ==================
	cMRDTrack(Int_t mrdtrackidin, std::string wcsimefilein, Int_t runidin, Int_t eventidin, Int_t subeventidin,
	Int_t triggerin, std::vector<Int_t> digitidsin, std::vector<Int_t> digittubesin, std::vector<Double_t>
	digitqsin, std::vector<Double_t> digittimesin, std::vector<Int_t> digitnumphotsin, std::vector<Double_t> 
	digitstruetimesin, std::vector<Int_t> digitsparentsin, std::vector<mrdcell> htrackcellsin, 
	std::vector<mrdcell> vtrackcellsin, std::vector<mrdcluster> htrackclustersin, 
	std::vector<mrdcluster> vtrackclustersin);
	
	// Copy Constructor
	// ================
	cMRDTrack(cMRDTrack const &trackin);
	
	// End class definition
	// ====================
	ClassDef(cMRDTrack,1);					// INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE
};

#endif

