/* vim:set noexpandtab tabstop=4 wrap */

#include "MRDSubEventClass.hh"
#include "MRDTrackClass.hh"

#include <iostream>

void cMRDSubEvent::Print(){                     // a generic print
	// Raw Info not printed:
//	std::vector<Int_t> digi_numphots;           // number of true photons for each digit
//	std::vector<Double_t> digi_phot_ts;         // true hit times of photons in a digit
//	std::vector<Int_t> digi_phot_parents;       // wcsim track IDs of parents that provided photons for a digit
	std::cout<<"NEW SUBEVENT"<<std::endl
//	<<"wcsimfile: "<<wcsimfile<<std::endl
	<<"run: "<<run_id<<", event: "<<event_id<<", trigger: "<<trigger<<", subevent: "<<mrdsubevent_id<<std::endl
	<<"num reconstructed tracks: "<<tracksthissubevent.size()<<std::endl
	<<"num true tracks: "<<truetrackvertices.size()<<std::endl;
//	<<"num digits: "<<digi_ids.size()<<std::endl;
	int digitsintracks=0;
	for(auto&& thetrack : tracksthissubevent) digitsintracks+=thetrack.digi_ids.size();
	std::cout<<"number of digits in tracks: "<<digitsintracks
			 <<" ("<<digi_ids.size()-digitsintracks<<" remain)"<<std::endl;
//	<<"num pmts hit: "<<pmts_hit.size()<<std::endl
//	<<"digit times: ";
//	for(auto atime : digi_ts) std::cout<<atime<<", ";
//	std::cout<<std::endl<<"digit charges: ";
//	for(auto acharge : digi_qs) std::cout<<acharge<<", ";
//	std::cout<<std::endl<<"layers hit: ";
//	for(auto alayer : layers_hit) std::cout<<alayer<<", ";
//	std::cout<<std::endl<<"energy deposited: ";
//	for(auto aqdeposit : eDepsInLayers) std::cout<<aqdeposit<<", ";
//	std::cout<<std::endl;
}


void cMRDSubEvent::DrawTracks(){
	CleanupCanvas();
	for(auto&& thetrack : tracksthissubevent){
		// draw the track
		int trackcolourindex=thetrack.MRDtrackID+1; // element 0 is black
		while(trackcolourindex+1>=cMRDSubEvent::trackcolours.size()) 
			trackcolourindex-=cMRDSubEvent::trackcolours.size();
		EColor thistrackscolour = cMRDSubEvent::trackcolours.at(trackcolourindex);
		EColor fittrackscolour = cMRDSubEvent::trackcolours.at(trackcolourindex+1);
		thetrack.DrawReco(imgcanvas, trackarrows, thistrackscolour, paddlepointers);
		thetrack.DrawFit(imgcanvas, trackfitarrows, fittrackscolour);
	}
}
