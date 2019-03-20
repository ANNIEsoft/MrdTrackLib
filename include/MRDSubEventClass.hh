/* vim:set noexpandtab tabstop=4 wrap */
#ifndef _MRDSubEvent_Class_
#define _MRDSubEvent_Class_

#include "TObject.h"

#include <vector>

class mrdcell;
class cMRDTrack;

class TCanvas;
class TBox;
class TVector3;
class TLorentzVector;
class TText;
class TLine;
class TArrow;
class TColor;

class cMRDSubEvent : public TObject {		// TObject inheritance is required to put in TClonesArray
	
	// Private members
	// ===============
	private:
	Int_t mrdsubevent_id;					// ID of this track within the trigger
	
	// Raw Info:
	std::string wcsimfile;					// which wcsim file this was in
	Int_t run_id;							// which run this file was in  FIXME is same as mrdsubevent_id
	Int_t event_id;							// which event this track was in
	Int_t trigger;							// which (sub)trigger this track was in FIXME loads -1s/1's, no 0s
	std::vector<Int_t> digi_ids;			// vector of digi ids: GetCherenkovDigiHits()->At(digi_ids.at(i))
	std::vector<Int_t> pmts_hit;			// vector of PMT IDs hit
	std::vector<Double_t> digi_qs;			// vector of digit charges
	std::vector<Double_t> digi_ts;			// vector of digit times
	std::vector<Int_t> digi_numphots;		// number of true photons for each digit
	std::vector<Double_t> digi_phot_ts;		// true hit times of photons in a digit FIXME some up to -2000???
	std::vector<Int_t> digi_phot_parents;	// wcsim track IDs of parents that provided photons for a digit
//	std::vector<WCSimRootCherenkovDigiHit> digits;
//	std::vector<WCSimRootTrack> truetracks;	// nice but depends on WCSim classes
	std::vector<std::pair<TVector3,TVector3>> truetrackvertices; // start and endpoints.
	std::vector<Int_t> truetrackpdgs;		// 
	// XXX FIXME XXX we could also use digi_phot_parents to sort hits in a track by their parent for truth comp!
	
	// Calculated/Reconstructed Info
	std::vector<cMRDTrack> tracksthissubevent;	// tracks created this SubEvent
	std::vector<Int_t> layers_hit;				// vector of layers hit TODO currently empty
	std::vector<Double_t> eDepsInLayers;		// fixed len vector of energy deposition in each layer TODO
	
	// Involved in drawing
	std::pair<double, double> xupcorner1, xupcorner2, xdowncorner1, xdowncorner2, yupcorner1, yupcorner2, ydowncorner1, ydowncorner2;  // TODO remove me, probably can just be defined in makemrdimage.cxx
	std::vector<TArrow*> trackfitarrows;  //!  stores TLines which are associated with track boundaries
	std::vector<TArrow*> trackarrows;     //!  stores TArrows associated with CA reconstructed tracks
	std::vector<TArrow*> truetrackarrows; //!  stores TArrows associated with true tracks
	
	public:
	// SubEvent Level Getters
	// ======================
	// Locate the subevent in file>run>event>trigger hierarchy
	Int_t GetSubEventID();
	std::string GetFile();
	Int_t GetRunID();
	Int_t GetEventID();
	Int_t GetTrigger();
	
	// Top level information about the subevent
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
	std::vector<std::pair<TVector3,TVector3>> GetTrueTrackVertices();
	std::vector<Int_t> GetTrueTrackPdgs();
	
	// Reconstructed Variables
	std::vector<cMRDTrack>* GetTracks();
	std::vector<Double_t> GetEdeps();
	std::vector<TArrow*> GetTrackArrows();
	std::vector<TArrow*> GetTrueTrackArrows();
	std::vector<TArrow*> GetTrackFitArrows();
	
	void Print(); // print the subevent info.
	
	// Functions to do reconstruction
	// ==============================
	private:
	// Main track reconstruction code. Groups paddles in a line into a MRDTrack
	void DoReconstruction(bool printtracks, bool drawcells, bool drawfit);
	
	// Used within DoReconstruction
	void LeastSquaresMinimizer(Int_t numdatapoints, Double_t datapointxs[], Double_t datapointys[], Double_t datapointweights[], Double_t errorys[], Double_t &fit_gradient, Double_t &fit_offset, Double_t &chi2);
	bool BridgeSearch(const std::vector<mrdcell*> &tracktotest, const std::vector<std::pair<int,int> > &matchedtracks, const std::vector<std::vector<mrdcell*> > &allpaddletracks, const std::string horv);
	bool SearchForClusterInTracks(const std::vector<std::pair<int,int> > &matchedtracks, const std::vector<std::vector<mrdcell*> > &allpaddletracks, const std::vector<mrdcell*> tracktotest, const std::string horv);
	
	void FillStaticMembers();
	
	// Default Constructor
	// ====================
	public:
	// Default constructor that initialises all private members required for ROOT classes
	cMRDSubEvent();
	
	// destructor
	~cMRDSubEvent();
	
	// Actual Constructor
	// ==================
	cMRDSubEvent(Int_t mrdsubevent_idin, std::string wcsimefilein, Int_t runidin, Int_t eventidin,
	Int_t triggerin, std::vector<Int_t> digitidsin, std::vector<Int_t> digittubesin, std::vector<Double_t>
	digitqsin, std::vector<Double_t> digittimesin, std::vector<Int_t> digitnumphotsin, std::vector<Double_t> 
	digitstruetimesin, std::vector<Int_t> digitsparentsin, std::vector<std::pair<TVector3,TVector3>> 
	truetrackverticesin, std::vector<Int_t> truetrackpdgsin);
	
	// Drawing
	// =======
	void DrawMrdCanvases();
	void DrawTrueTracks();
	void DrawTracks();
	static Bool_t fillstaticmembers;
	static TCanvas* imgcanvas;
	static TText* titleleft;
	static TText* titleright;
	static std::vector<TBox*> paddlepointers;
	void ComputePaddleTransformation (const Int_t copyNo, TVector3 &origin, Bool_t &ishpaddle);
	int GetLayerNum(int copyNo, int* paddlenuminlayer=nullptr, bool* ishpaddle=nullptr);
	int GetNumPaddlesInLayer(int layernum);
	void HighlightPaddles(std::vector<std::vector<int>> paddlesToHighlight);
	
	static std::vector<Int_t> aspectrumv;
	static std::vector<std::string> colorhexes;
	static std::vector<EColor> trackcolours;
	
	void RemoveArrows();
	void CleanupCanvas();
	
	// Required by ROOT
	// ================
	void Clear();
	
	// End class definition
	// ====================
	ClassDef(cMRDSubEvent,1);					// INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE
};

#endif

