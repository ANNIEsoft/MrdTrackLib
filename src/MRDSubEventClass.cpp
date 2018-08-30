/* vim:set noexpandtab tabstop=4 wrap */

#include "MrdCell.hh"
#include "MRDspecs.hh"
#include "MRDSubEventClass.hh"
#include "MRDTrackClass.hh"

#include "TVector3.h"
#include "TCanvas.h"
#include "TText.h"
#include "TArrow.h"
#include "TColor.h"

#include <vector>
#include <iostream>

#ifndef _MRDSubEvent_VERBOSE_
//#define _MRDSubEvent_VERBOSE_
#endif

// Info about the parent event
Int_t cMRDSubEvent::GetSubEventID(){return mrdsubevent_id;}
std::string cMRDSubEvent::GetFile(){return wcsimfile;}
Int_t cMRDSubEvent::GetRunID(){return run_id;}
Int_t cMRDSubEvent::GetEventID(){return event_id;}
Int_t cMRDSubEvent::GetTrigger(){return trigger;}

// Top level information about the subevent
Int_t cMRDSubEvent::GetNumDigits(){return digi_ids.size();}
Int_t cMRDSubEvent::GetNumLayersHit(){return layers_hit.size();}
Int_t cMRDSubEvent::GetNumPMTsHit(){return pmts_hit.size();}
std::vector<Int_t> cMRDSubEvent::GetDigitIds(){return digi_ids;}
std::vector<Double_t> cMRDSubEvent::GetDigitQs(){return digi_qs;}
std::vector<Double_t> cMRDSubEvent::GetDigitTs(){return digi_ts;}
std::vector<Int_t> cMRDSubEvent::GetDigiNumPhots(){return digi_numphots;}
std::vector<Double_t> cMRDSubEvent::GetDigiPhotTs(){return digi_phot_ts;}
std::vector<Int_t> cMRDSubEvent::GetDigiPhotParents(){return digi_phot_parents;}
std::vector<Int_t> cMRDSubEvent::GetLayersHit(){return layers_hit;}
std::vector<Int_t> cMRDSubEvent::GetPMTsHit(){return pmts_hit;}
std::vector<std::pair<TVector3,TVector3>> cMRDSubEvent::GetTrueTrackVertices(){return truetrackvertices;}
std::vector<Int_t> cMRDSubEvent::GetTrueTrackPdgs(){return truetrackpdgs;}

// Reconstructed Variables
std::vector<cMRDTrack>* cMRDSubEvent::GetTracks(){ return &tracksthissubevent;}
std::vector<Double_t> cMRDSubEvent::GetEdeps(){return eDepsInLayers;}
std::vector<TArrow*> cMRDSubEvent::GetTrackArrows(){return trackarrows;}
std::vector<TArrow*> cMRDSubEvent::GetTrueTrackArrows(){return truetrackarrows;}
std::vector<TArrow*> cMRDSubEvent::GetTrackFitArrows(){return trackfitarrows;}

void cMRDSubEvent::FillStaticMembers(){
	for(int i=0; i<19; i++){
		Int_t colorindex = TColor::GetColor(colorhexes.at(i).c_str());
		aspectrumv.at(i)=colorindex;
	}
	fillstaticmembers=false;
}

// Default Constructor
// ====================
// Default constructor that initialises all private members required for ROOT classes
cMRDSubEvent::cMRDSubEvent() : mrdsubevent_id(-1), wcsimfile(""), run_id(-1), event_id(-1), trigger(-1), digi_ids(), pmts_hit(), digi_qs(), digi_ts(), digi_numphots(), digi_phot_ts(), digi_phot_parents(), layers_hit(), eDepsInLayers(), tracksthissubevent(), truetrackvertices(), truetrackpdgs() {};

// Destructor
// ==========
cMRDSubEvent::~cMRDSubEvent(){
#ifdef _MRDSubEvent_VERBOSE_
	std::cout<<"cMRDSubEvent destructor"<<std::endl;
#endif
	RemoveArrows();
}

// Actual Constructor
// ==================
cMRDSubEvent::cMRDSubEvent(Int_t mrdsubevent_idin, std::string wcsimefilein, Int_t runidin, Int_t eventidin,
Int_t triggerin, std::vector<Int_t> digitidsin, std::vector<Int_t> digittubesin, std::vector<Double_t>
digitqsin, std::vector<Double_t> digittimesin, std::vector<Int_t> digitnumphotsin, std::vector<Double_t> 
digitstruetimesin, std::vector<Int_t> digitsparentsin, std::vector<std::pair<TVector3,TVector3>> 
truetrackverticesin, std::vector<Int_t> truetrackpdgsin) :
/* information retrieved when creating the track: initialize with input */
mrdsubevent_id(mrdsubevent_idin), wcsimfile(wcsimefilein), run_id(runidin), event_id(eventidin),
trigger(triggerin), digi_ids(digitidsin), pmts_hit(digittubesin), digi_qs(digitqsin),
digi_ts(digittimesin), digi_numphots(digitnumphotsin), digi_phot_ts(digitstruetimesin),
digi_phot_parents(digitsparentsin),
/* information calculated: initialize to default */
layers_hit(), tracksthissubevent(), trackarrows(), truetrackarrows(), trackfitarrows() {
	eDepsInLayers.assign(MRDSpecs::numpanels, 0.);	// can't assign the size in the class def. 
	for(auto&& atrack : truetrackverticesin) truetrackvertices.push_back(atrack);
	for(auto&& atrack : truetrackpdgsin) truetrackpdgs.push_back(atrack);
	
#ifdef _MRDSubEvent_VERBOSE_
	std::cout<<std::endl<<"constructing a subevent with "<<digi_ids.size()<<" digits"<<std::endl;
#endif
	if(fillstaticmembers) FillStaticMembers();
	
	Bool_t printtracks=false;
	Bool_t drawcells=false;
	Bool_t drawfit=false;
	Bool_t drawtruetracks=false;
	Bool_t saveimage=false;
	
	// Clean up the Canvas before we draw this subevent
	CleanupCanvas();
	
	if(drawcells||drawfit||drawtruetracks) DrawMrdCanvases();  // creates the canvas with the digits
	if(drawtruetracks) DrawTrueTracks();   // draws true tracks over the event
	DoReconstruction(printtracks, drawcells, drawfit); // adds the tracks to the canvas
	if(saveimage) imgcanvas->SaveAs(TString::Format("mrdtracks_%d.png",event_id));
	//std::cout<<"sleeping for 5 seconds to analyse output"<<std::endl;
	//if(tracksthissubevent.size()) std::this_thread::sleep_for (std::chrono::seconds(15));
	//std::cout<<"moving to next event"<<std::endl;
	//gPad->WaitPrimitive();
	//assert(false);
}

void cMRDSubEvent::CleanupCanvas(){  // clean any existing arrows from the imgcanvas, but don't delete them
	if(imgcanvas){
#ifdef _MRDSubEvent_VERBOSE_
		cout<<"Cleaning TArrows from imgcanvas"<<endl;
#endif
		for(int subpad=1; subpad<3; subpad++){ // loop over top and side views
			TList* imgcanvasprimaries = imgcanvas->GetPad(subpad)->GetListOfPrimitives();
			TIter primaryiterator(imgcanvasprimaries);
			while(TObject* aprimary = primaryiterator()){
				TString primarytype = aprimary->ClassName();
#ifdef _MRDSubEvent_VERBOSE_
				cout<<"next prim is a "<<primarytype<<endl;
#endif
				if(primarytype=="TArrow"){
					imgcanvasprimaries->Remove(aprimary);
				}
			}
		}
	}
}

void cMRDSubEvent::RemoveArrows(){	// sometimes need to clear the arrows even before deleting the subevent.
	for(auto anarrow : trackarrows){
		delete anarrow;
	}
	trackarrows.clear();
	
	for(auto anarrow : truetrackarrows){
		delete anarrow;
	}
	truetrackarrows.clear();
	
	for(auto anarrow : trackfitarrows){
		delete anarrow;
	}
	trackfitarrows.clear();
}

// Required by ROOT
// ================
void cMRDSubEvent::Clear(){
#ifdef _MRDSubEvent_VERBOSE_
	std::cout<<"calling clear on cMRDSubEvent "<<mrdsubevent_id<<std::endl;
#endif
	mrdsubevent_id=-1;
	wcsimfile="";
	run_id=-1;
	event_id=-1;
	trigger=-1;
	digi_ids.clear();
	pmts_hit.clear();
	digi_qs.clear();
	digi_ts.clear();
	digi_numphots.clear();
	digi_phot_ts.clear();
	digi_phot_parents.clear();
//		digits.clear();
	layers_hit.clear();
	eDepsInLayers.assign(MRDSpecs::numpanels,0.);
	truetrackvertices.clear();
	truetrackpdgs.clear();
	// heap allocated objects for drawing the event on a canvas
	RemoveArrows();
}

//===========================
//===========================

// Static member initialization

Bool_t cMRDSubEvent::fillstaticmembers=true;
TCanvas* cMRDSubEvent::imgcanvas=0;
TText* cMRDSubEvent::titleleft=0;
TText* cMRDSubEvent::titleright=0;
// allocate paddle vector now: they'll be filled in first call to DrawMrdCanvases
std::vector<TBox*> cMRDSubEvent::paddlepointers(MRDSpecs::nummrdpmts+(2*MRDSpecs::numpanels));
std::vector<Int_t> cMRDSubEvent::aspectrumv{1435, 1436, 1437, 1438, 1439, 1440, 1441, 1442, 1443, 1443, 1444, 1445, 885, 1446, 1447, 1448, 879, 1449, 1450};
std::vector<std::string> cMRDSubEvent::colorhexes{"#21ffff", "#20deea", "#1fbcd5", "#21a8cd", "#269bcb", "#2b8fca", "#367fcb", "#416fcb", "#4965cd", "#505dcf", "#5855d0", "#6247cf", "#6d3ace", "#782acd", "#851acc", "#910dcc", "#9e07cd", "#aa00ce", "#bf00d7"};
std::vector<EColor> cMRDSubEvent::trackcolours{kBlack, kBlue, (EColor)TColor::GetColorDark(kGreen), kRed, kViolet, kOrange, kMagenta,(EColor)(kAzure+2),(EColor)(kOrange+4),(EColor)(kViolet-6),(EColor)(kTeal-6)};

