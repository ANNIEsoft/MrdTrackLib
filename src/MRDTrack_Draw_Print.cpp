/* vim:set noexpandtab tabstop=4 wrap */

#include "MRDTrackClass.hh"
#include "MRDspecs.hh"

#include "TCanvas.h"
#include "TArrow.h"
#include "TBox.h"

#include <iostream>

#ifndef MRDTrack_RECO_VERBOSE
//#define MRDTrack_RECO_VERBOSE
#endif

using std::cout;
using std::endl;

void cMRDTrack::Print2(bool eventinfo){					// a more generic print

std::cout<<"NEXT MRD TRACK"<<std::endl;
if(eventinfo){
	std::cout<<"wcsimfile: "<<wcsimfile<<std::endl
	<<"run: "<<run_id<<", event: "<<event_id<<", trigger: "<<trigger<<", mrdsubevent: "<<mrdsubevent_id
	<<", MRD track: "<<MRDtrackID<<std::endl;
} else {
	std::cout<<"MRD track: "<<MRDtrackID<<std::endl;
}
std::cout<<"num digits: "<<digi_ids.size()<<std::endl
	<<"num pmts hit: "<<pmts_hit.size()<<std::endl
	<<"digit times: ";
	for(auto atime : digi_ts) std::cout<<atime<<", ";
std::cout<<std::endl<<"digit charges: ";
	for(auto acharge : digi_qs) std::cout<<acharge<<", ";
std::cout<<std::endl<<"layers hit: ";
	for(auto alayer : layers_hit) std::cout<<alayer<<", ";
std::cout<<std::endl<<"energy deposited: ";
	for(auto aqdeposit : eDepsInLayers) std::cout<<aqdeposit<<", ";
std::cout<<std::endl<<"track ";
	if(ispenetrating) std::cout<<"fully penetrates mrd"<<std::endl;
	if(isstopped) std::cout<<"stops within mrd"<<std::endl;
	if(sideexit) std::cout<<"exits side of mrd"<<std::endl;
std::cout<<"side view clusters: "<<htrackclusters.size()<<std::endl
	<<"top view clusters: "<<vtrackclusters.size()<<std::endl;
std::cout<<"total track length: "<<mutracklengthinMRD<<std::endl
	<<"penetration depth: "<<penetrationdepth<<std::endl
	<<"energy loss: "<<EnergyLoss<<", error: "<<EnergyLossError<<std::endl
	<<"track start: ("<<trackfitstart.X()<<", "<<trackfitstart.Y()<<", "<<trackfitstart.Z()
	<<"), track end: ("<<trackfitstop.X()<<", "<<trackfitstop.Y()<<", "<<trackfitstop.Z()<<")"<<std::endl
	<<"h origin: "<<htrackorigin<<", error: "<<htrackoriginerror
	<<", gradient: "<<htrackgradient<<", error: "<<htrackgradienterror<<std::endl
	<<"v origin: "<<vtrackorigin<<", error: "<<vtrackoriginerror
	<<", gradient: "<<vtrackgradient<<", error: "<<vtrackgradienterror<<std::endl
	<<"fit chi2: "<<htrackfitchi2<<", "<<vtrackfitchi2<<std::endl
	<<"angle from z: "<<trackangle<<", error: "<<trackangleerror<<std::endl
	<<"back projection ";
	if(interceptstank) std::cout<<"intercepts the tank"<<std::endl;
	else std::cout<<"does not intercept the tank"<<std::endl; 
	
}

void cMRDTrack::Print(){	// a print specific to printing CA algorithm results
	for(int j=(htrackcells.size()-1); j>-1; j--){
		std::cout<<std::endl;
		mrdcell* acell = &htrackcells.at(j);
		acell->SetClusterAddresses(htrackclusters);
		mrdcluster* upcluster = acell->clusters.first;
		mrdcluster* downcluster = acell->clusters.second;
		if(!(acell->isdownstreamgoing)){
			// switch the order of clusters, since the track is upstream going,
			// but cluster ordering is downstream going
			mrdcluster* temp = upcluster;
			upcluster=downcluster;
			downcluster=temp;
		}
		std::cout<<"   cell "<<j<<" goes from cluster "<<upcluster->clusterid<<" in layer "<<upcluster->layer
			<<" to cluster "<<downcluster->clusterid<<" in layer "<<downcluster->layer<<std::endl;

		std::cout<<"      uptrack cluster contains tubes ";
		for(int k=0; k<upcluster->in_layer_tubeids.size(); k++){
			(k!=0) ? std::cout<<", " : std::cout<<" ";
			std::cout<<upcluster->in_layer_tubeids.at(k);
		}
		std::cout<<std::endl;
		std::cout<<"      maximum in-layer index is "<<upcluster->xmaxid
			<<", minimum in-layer index is "<<upcluster->xminid
			<<", effective centre index is "<<upcluster->GetCentreIndex()<<std::endl;
		std::cout<<"      physical extent is "<<upcluster->GetXmin()<<" to "<<upcluster->GetXmax()<<std::endl;

		std::cout<<std::endl;

		std::cout<<"      downtrack cluster contains tubes ";
		for(int k=0; k<downcluster->in_layer_tubeids.size(); k++){
			(k!=0) ? std::cout<<", " : std::cout<<" ";
			std::cout<<downcluster->in_layer_tubeids.at(k);
		}
		std::cout<<std::endl;
		std::cout<<"      maximum in-layer index is "<<downcluster->xmaxid
			<<", minimum in-layer index is "<<downcluster->xminid
			<<", effective centre index is "<<downcluster->GetCentreIndex()<<std::endl;
		std::cout<<"      physical extent is "<<downcluster->GetXmin()<<" to "<<downcluster->GetXmax()<<std::endl;
	}
	// ------------
	std::cout<<"vertical track "<<MRDtrackID<<" at "<<&vtrackcells[0]<<" has "
		<<vtrackcells.size()<<" cells:"<<std::endl;
	for(int j=(vtrackcells.size()-1); j>-1; j--){
		std::cout<<std::endl;
		mrdcell* acell = &vtrackcells.at(j);
		std::cout<<"setting cluster addresses for cell "<<j<<std::endl;
		acell->SetClusterAddresses(vtrackclusters);
		mrdcluster* upcluster = acell->clusters.first;
		mrdcluster* downcluster = acell->clusters.second;
		if(!(acell->isdownstreamgoing)){
			// switch the order of clusters, since the track is upstream going,
			// but cluster ordering is downstream going
			mrdcluster* temp = upcluster;
			upcluster=downcluster;
			downcluster=temp;
		}
		std::cout<<"   cell "<<j<<" goes from cluster "<<upcluster->clusterid<<" in layer "<<upcluster->layer
			<<" to cluster "<<downcluster->clusterid<<" in layer "<<downcluster->layer<<std::endl;

		std::cout<<"      uptrack cluster contains tubes ";
		for(int k=0; k<upcluster->in_layer_tubeids.size(); k++){
			(k!=0) ? std::cout<<", " : std::cout<<" ";
			std::cout<<upcluster->in_layer_tubeids.at(k);
		}
		std::cout<<std::endl;
		std::cout<<"      maximum in-layer index is "<<upcluster->xmaxid
			<<", minimum in-layer index is "<<upcluster->xminid
			<<", effective centre index is "<<upcluster->GetCentreIndex()<<std::endl;
		std::cout<<"      physical extent is "<<upcluster->GetXmin()<<" to "<<upcluster->GetXmax()<<std::endl;

		std::cout<<std::endl;

		std::cout<<"      downtrack cluster contains tubes ";
		for(int k=0; k<downcluster->in_layer_tubeids.size(); k++){
			(k!=0) ? std::cout<<", " : std::cout<<" ";
			std::cout<<downcluster->in_layer_tubeids.at(k);
		}
		std::cout<<std::endl;
		std::cout<<"      maximum in-layer index is "<<downcluster->xmaxid
			<<", minimum in-layer index is "<<downcluster->xminid
			<<", effective centre index is "<<downcluster->GetCentreIndex()<<std::endl;
		std::cout<<"      physical extent is "<<downcluster->GetXmin()<<" to "<<downcluster->GetXmax()<<std::endl;
	}
}

void cMRDTrack::DrawReco(TCanvas* imgcanvas, std::vector<TArrow*> &trackarrows, EColor thistrackscolour, std::vector<TBox*> paddlepointers){
	
	// TODO: when reconstructed track start/end is set by other view, (in top view at least),
	// theoffset should be set to 0 so it aligns correctly with the alt view paddles
	
	// draw horizontal paddle track
	imgcanvas->cd(1);
	for(int j=0; j<htrackcells.size(); j++){
		mrdcell* acell = &htrackcells.at(j);
		acell->SetClusterAddresses(htrackclusters);
		mrdcluster* upcluster = acell->clusters.first;
		mrdcluster* downcluster = acell->clusters.second;
		// cluster ordering is downstream going, regardless of track direction
		// a cluster may have multiple tubes hit; find the top and bottom tubes
		Int_t uptubetopid = (2*upcluster->xmaxid) + MRDSpecs::layeroffsets.at(upcluster->layer);
		Int_t uptubebottomid = (2*upcluster->xminid) + MRDSpecs::layeroffsets.at(upcluster->layer);
		if((upcluster->altviewhalf)<0){ uptubetopid++; uptubebottomid++; }
		// find the positions in the canvas of the those tubes' bottom corners
		TBox* uptopbox = paddlepointers.at(uptubetopid);
		TBox* upbottombox = paddlepointers.at(uptubebottomid);
		if(uptopbox==0||upbottombox==0){
			std::cerr<<"null box! uptubetopid="<<uptubetopid<<",uptubebottomid="<<uptubebottomid<<std::endl;
		}
		// find the average x and y positions: this is the centre position on the canvas of this cluster
		Double_t upclustery = (upbottombox->GetY1()+uptopbox->GetY2())/2.;
		Double_t upclusterx = upbottombox->GetX2();
		// repeat process with downcluster to find arrow endpoint in canvas
		Int_t downtubetopid = (2*downcluster->xmaxid) + MRDSpecs::layeroffsets.at(downcluster->layer);
		Int_t downtubebottomid = (2*downcluster->xminid) + MRDSpecs::layeroffsets.at(downcluster->layer);
		if((downcluster->altviewhalf)<0){ downtubetopid++; downtubebottomid++; }
		TBox* downtopbox = paddlepointers.at(downtubetopid);
		TBox* downbottombox = paddlepointers.at(downtubebottomid);
		Double_t downclustery = (downbottombox->GetY1()+downtopbox->GetY2())/2.;
		Double_t downclusterx = downbottombox->GetX2();
		//if((upcluster->altviewhalf)==0){ upclusterx+=0.0025; }
		//if((downcluster->altviewhalf)==0){ downclusterx+=0.0025; }
		
		// choose arrow head based on track direction
		TArrow* myarrow;
		if(acell->isdownstreamgoing){
			// downcluster is the arrow head
			myarrow = new TArrow(upclusterx, upclustery, downclusterx, downclustery, 0.01, ">");
		} else {
			myarrow = new TArrow(upclusterx, upclustery, downclusterx, downclustery, 0.01, "<");
		}
		myarrow->SetLineWidth(2);
		myarrow->SetLineColor(thistrackscolour);
		//(downcluster->layer%2==0) ? imgcanvas->cd(1) : imgcanvas->cd(2);
		myarrow->Draw();
		imgcanvas->Update();
#ifdef MRDTrack_VERBOSE
		std::cout<<"drawing reconstructed track arrow from "<<myarrow->GetX1()<<", "<<myarrow->GetY1()<<" to "
			<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
		//std::this_thread::sleep_for (std::chrono::seconds(3));
		trackarrows.push_back(myarrow);
	}
	// draw vertical paddle track
	imgcanvas->cd(2);
	for(int j=0; j<vtrackcells.size(); j++){
		mrdcell* acell = &vtrackcells.at(j);
		acell->SetClusterAddresses(vtrackclusters);
		mrdcluster* upcluster = acell->clusters.first;
		mrdcluster* downcluster = acell->clusters.second;
		// cluster ordering is downstream going, regardless of track direction
		// a cluster may have multiple tubes hit; find the top and bottom tubes
		Int_t uptubetopid = (2*upcluster->xmaxid) + MRDSpecs::layeroffsets.at(upcluster->layer);
		Int_t uptubebottomid = (2*upcluster->xminid) + MRDSpecs::layeroffsets.at(upcluster->layer);
		if((upcluster->altviewhalf)<0){ uptubetopid++; uptubebottomid++; }
		// find the positions in the canvas of the those tubes' bottom corners
		TBox* uptopbox = paddlepointers.at(uptubetopid);
		TBox* upbottombox = paddlepointers.at(uptubebottomid);
		if(uptopbox==0||upbottombox==0){
			std::cerr<<"null box! uptubetopid="<<uptubetopid<<",uptubebottomid="<<uptubebottomid<<std::endl;
		}
		// find the average x and y positions: this is the centre position on the canvas of this cluster
		Double_t upclustery = (upbottombox->GetY1()+uptopbox->GetY2())/2.;
		Double_t upclusterx = upbottombox->GetX2();
		// repeat process with downcluster to find arrow endpoint in canvas
		Int_t downtubetopid = (2*downcluster->xmaxid) + MRDSpecs::layeroffsets.at(downcluster->layer);
		Int_t downtubebottomid = (2*downcluster->xminid) + MRDSpecs::layeroffsets.at(downcluster->layer);
		if((downcluster->altviewhalf)<0){ downtubetopid++; downtubebottomid++; }
		TBox* downtopbox = paddlepointers.at(downtubetopid);
		TBox* downbottombox = paddlepointers.at(downtubebottomid);
		Double_t downclustery = (downbottombox->GetY1()+downtopbox->GetY2())/2.;
		Double_t downclusterx = downbottombox->GetX2();
		//if((upcluster->altviewhalf)==0){ upclusterx+=0.0025; }
		//if((downcluster->altviewhalf)==0){ downclusterx+=0.0025; }
	
		// choose arrow head based on track direction
		TArrow* myarrow;
		if(acell->isdownstreamgoing){
			// downcluster is the arrow head
			myarrow = new TArrow(upclusterx, upclustery, downclusterx, downclustery, 0.01, ">");
		} else {
			myarrow = new TArrow(upclusterx, upclustery, downclusterx, downclustery, 0.01, "<");
		}
		myarrow->SetLineWidth(2);
		myarrow->SetLineColor(thistrackscolour);
		//(downcluster->layer%2==0) ? imgcanvas->cd(1) : imgcanvas->cd(2);
		myarrow->Draw();
		imgcanvas->Update();
#ifdef MRDTrack_VERBOSE
		std::cout<<"drawing reconstructed track arrow from "<<myarrow->GetX1()<<", "<<myarrow->GetY1()<<" to "
			<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
		//std::this_thread::sleep_for (std::chrono::seconds(3));
		trackarrows.push_back(myarrow);
	}
}

void cMRDTrack::DrawFit(TCanvas* imgcanvas, std::vector<TArrow*> &trackfitarrows, EColor thistrackscolour){
	Double_t mrdentryx = trackfitstart.X();
	Double_t mrdentryy = trackfitstart.Y();
	Double_t mrdentryz = trackfitstart.Z();
	Double_t mrdexitx = trackfitstop.X();
	Double_t mrdexity = trackfitstop.Y();
	Double_t mrdexitz = trackfitstop.Z();
	
//	for debugging
//	Double_t mrdentryz = MRDSpecs::paddle_originz.at(35)/10.;
//	Double_t mrdentryx = -0.01;//MRDSpecs::paddle_originx.at(0)/10.;
//	Double_t mrdentryy = 0.01;//MRDSpecs::paddle_originy.at(0)/10.;
//	Double_t mrdexitz = MRDSpecs::paddle_originz.at(MRDSpecs::nummrdpmts-30)/10.;
//	Double_t mrdexitx = MRDSpecs::paddle_originx.at(MRDSpecs::nummrdpmts-27)/10.;
//	Double_t mrdexity = MRDSpecs::paddle_originy.at(MRDSpecs::nummrdpmts-1)/10.;
	
#ifdef MRDTrack_RECO_VERBOSE
	std::cout<<"mrd entry point is ("<<mrdentryx<<", "<<mrdentryy<<", "<<mrdentryz<<")"<<std::endl;
	std::cout<<"mrd exit point is ("<<mrdexitx<<", "<<mrdexity<<", "<<mrdexitz<<")"<<std::endl;
#endif
	
	// up to now all measurements are in WCSim absolute coordinates. Shift z axis so that
	// the MRD is centered on (0,0);
	mrdentryz -= (MRDSpecs::MRD_start+(MRDSpecs::MRD_depth/2.));
	mrdexitz -= (MRDSpecs::MRD_start+(MRDSpecs::MRD_depth/2.));
	
	bool trackisbackwardgoing=false;
	
#ifdef MRDTrack_RECO_VERBOSE
	std::cout<<"shifting z axis; new entry and exit points are "<<mrdentryz<<" and "<<mrdexitz<<std::endl;
	std::cout<<"entry and exit points in terms of mrd width, height and depth are: ("
		<<(mrdentryx/MRDSpecs::maxwidth)<<", "<<(mrdentryy/MRDSpecs::maxheight)
		<<", "<<(mrdentryz/MRDSpecs::mrdZlen)<<") -> ("
		<<(mrdexitx/MRDSpecs::maxwidth)<<", "<<(mrdexity/MRDSpecs::maxheight)
		<<", "<<(mrdexitz/MRDSpecs::mrdZlen)<<")"<<std::endl;
#endif
	
	// the following is code copied from DrawTruthTracks, as it converts cm to canvas units,
	// accounts for offsets with side, and draws the necessary arrows
	
	// scale cm to canvas size and offset to start of MRD diagram
	/*  ✩ ✨ Magic Numbers! ✨ ✩ */
	double anoffset=(MRDSpecs::scintfullzlen+MRDSpecs::scintalugap)*5.;  // this accounts for the half shift
	double topscalefactor=1.2;           // compress canvas width to paddle diagram height (top view)
	double sidescalefactor=1.2;         //   "         "      "       "       "    width (side view)
	double topdepthscalefactor=1.18;     //   "         "      "       "       "    depth (top view)
	double sidedepthscalefactor=1.16;     // compress canvas depth to paddle diagram depth (side view)
	double xscalefactor=1.;//(0.5/0.403825);  // correct differences in definition of MRD width and height
	double yscalefactor=1.;//(0.5/0.384671);  // between this method and that for paddle placements
	double topzoffset=0.075;               // shifts the track arrows +z          (top  view)
	double sidezoffset=0.058;              // to account for centering of diagram (side view)
	
//	mrdentryx*=xscalefactor;
//	mrdexitx*=xscalefactor;
//	mrdentryy*=yscalefactor;
//	mrdexity*=yscalefactor;
//#ifdef MRDTrack_RECO_VERBOSE
//	std::cout<<"scaled entry and exit points in terms of mrd width, height and depth are: ("
//		<<(mrdentryx/MRDSpecs::maxwidth)<<", "<<(mrdentryy/MRDSpecs::maxheight)
//		<<", "<<(mrdentryz/MRDSpecs::mrdZlen)<<") -> ("
//		<<(mrdexitx/MRDSpecs::maxwidth)<<", "<<(mrdexity/MRDSpecs::maxheight)
//		<<", "<<(mrdexitz/MRDSpecs::mrdZlen)<<")"<<std::endl;
//#endif
	
	// one last thing: the beam comes from the left. In the top view, right-hand-side (x>0)
	// needs to map to the bottom of the canvas (canvas_y<0) - so, let's swap the signs of all
	// x points
	mrdentryx*=-1.;
	mrdexitx*=-1.;
	Double_t avgtrackanglex=-1.*vtrackgradient;
	Double_t avgtrackangley=htrackgradient;
	
	std::vector<double> xstarts, ystarts, zstartsx, zstartsy, xstops, ystops, zstopsx, zstopsy;
	xstarts.push_back((mrdentryx/(MRDSpecs::maxwidth*topscalefactor))+0.5);
	ystarts.push_back((mrdentryy/(MRDSpecs::maxheight*sidescalefactor))+0.5);
	// starting z may need a shift depending on the appropriate half
	// in top view
	if(mrdentryy>0){
		zstartsx.push_back((mrdentryz/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
	} else {
		zstartsx.push_back(((mrdentryz+anoffset)/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
	}
	// in side view
	if(mrdentryx>0){
		zstartsy.push_back((mrdentryz/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
	} else {
		zstartsy.push_back(((mrdentryz+anoffset)/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
	}
	// check if we cross sides, and if so, create a middle stop and start set
	// top view
	if((mrdentryy*mrdexity)<0){
		// we'll need two lines with a bit of a disconnect. find the crossing point.
		double crossingz = mrdentryz-((mrdentryy/yscalefactor) / /*TMath::Tan*/(avgtrackangley));
		double crossingx = (mrdentryx/xscalefactor) + ((crossingz-mrdentryz)*avgtrackanglex);
		xstops.push_back(((crossingx*xscalefactor)/(MRDSpecs::maxwidth*topscalefactor))+0.5);
		if(mrdentryy>0){
			zstopsx.push_back((crossingz/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
		} else {
			zstopsx.push_back(((crossingz+anoffset)/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
		}
		xstarts.push_back(((crossingx*xscalefactor)/(MRDSpecs::maxwidth*topscalefactor))+0.5);
		if(mrdexity>0){
			zstartsx.push_back((crossingz/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
		} else {
			zstartsx.push_back(((crossingz+anoffset)/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
		}
	}
	// side view
	if((mrdentryx*mrdexitx)<0){
		// we'll need two lines with a bit of a disconnect. find the crossing point.
		double crossingz = mrdentryz-((mrdentryx/xscalefactor) / /*TMath::Tan*/(avgtrackanglex));
		double crossingy = (mrdentryy/yscalefactor) + ((crossingz-mrdentryz)*avgtrackangley);
		ystops.push_back(((crossingy*yscalefactor)/(MRDSpecs::maxheight*sidescalefactor))+0.5);
		if(mrdentryx>0){
			zstopsy.push_back((crossingz/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
		} else {
			zstopsy.push_back(((crossingz+anoffset)/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
		}
		ystarts.push_back(((crossingy*yscalefactor)/(MRDSpecs::maxheight*sidescalefactor))+0.5);
		if(mrdexitx>0){
			zstartsy.push_back((crossingz/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
		} else {
			zstartsy.push_back(((crossingz+anoffset)/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
		}
	}
	// finally add the endpoint values, with offset for z according to ending MRD half.
	xstops.push_back((mrdexitx/(MRDSpecs::maxwidth*topscalefactor))+0.5);
	ystops.push_back((mrdexity/(MRDSpecs::maxheight*sidescalefactor))+0.5);
	// top view
	if(mrdexity>0){
		zstopsx.push_back((mrdexitz/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
	} else {
		zstopsx.push_back(((mrdexitz+anoffset)/(MRDSpecs::mrdZlen*topdepthscalefactor))+0.5+topzoffset);
	}
	// side view
	if(mrdexitx>0){
		zstopsy.push_back((mrdexitz/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
	} else {
		zstopsy.push_back(((mrdexitz+anoffset)/(MRDSpecs::mrdZlen*sidedepthscalefactor))+0.5+sidezoffset);
	}
	
	// OK done.
	// now loop over the pairs and make the arrows
	// top view
	imgcanvas->cd(2);  // top view for x positions
	for(int i=0; i < zstartsx.size(); i++){
		// Draw arrow representing "true" (assumed straight) trajectory in top view
		std::string arrowdir = (!trackisbackwardgoing) ? ">" : "<";
		TArrow* myarrow = 
			new TArrow(zstartsx.at(i), xstarts.at(i), zstopsx.at(i), xstops.at(i), 0.005, arrowdir.c_str());
		myarrow->SetLineWidth(2);
		myarrow->SetLineColor(thistrackscolour);
		myarrow->SetLineStyle(2);  //dashed
		myarrow->Draw();
#ifdef MRDTrack_RECO_VERBOSE
		std::cout<<"drawing top view fit track arrow from "<<myarrow->GetX1()<<", "<<myarrow->GetY1()
			<<" to "<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
		trackfitarrows.push_back(myarrow);
		
		if(zstartsx.size()==2&&i==0){
			// add an intermediate link dashed line to link the arrows
			myarrow = 
				new TArrow(zstopsx.at(i), xstops.at(i), zstartsx.at(i+1), xstarts.at(i+1),0.0,">");
				// an arrow size of 0.0 gives no arrow head (just a line)
			myarrow->SetLineWidth(2);
			myarrow->SetLineColor(thistrackscolour);
			myarrow->SetLineStyle(3);  //dotted
			myarrow->Draw();
#ifdef MRDTrack_RECO_VERBOSE
			std::cout<<"drawing line from "<<myarrow->GetX1()<<", "<<myarrow->GetY1()<<" to "
				<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
			trackfitarrows.push_back(myarrow);
		}
	}
	
	// need to do top and side views separately as they may have different sizes
	// side view
	imgcanvas->cd(1);  // side view for y positions
	for(int i=0; i < zstartsy.size(); i++){
		// Draw arrow representing "true" (assumed straight) trajectory in top view
		std::string arrowdir = (!trackisbackwardgoing) ? ">" : "<";
		TArrow* myarrow = 
			new TArrow(zstartsy.at(i), ystarts.at(i), zstopsy.at(i), ystops.at(i), 0.005, arrowdir.c_str());
		myarrow->SetLineWidth(2);
		myarrow->SetLineColor(thistrackscolour);
		myarrow->SetLineStyle(2);  //dashed
		myarrow->Draw();
#ifdef MRDTrack_RECO_VERBOSE
		std::cout<<"drawing side view fit track arrow from "<<myarrow->GetX1()<<", "
		<<myarrow->GetY1()<<" to "<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
		trackfitarrows.push_back(myarrow);
		
		if(zstartsy.size()==2&&i==0){
			// add an intermediate link dashed line to link the arrows
			myarrow = 
				new TArrow(zstopsy.at(i), ystops.at(i), zstartsy.at(i+1), ystarts.at(i+1),0.0,">");
				// an arrow size of 0.0 gives no arrow head (just a line)
			myarrow->SetLineWidth(2);
			myarrow->SetLineColor(thistrackscolour);
			myarrow->SetLineStyle(3);  //dotted
			myarrow->Draw();
#ifdef MRDTrack_RECO_VERBOSE
			std::cout<<"drawing line from "<<myarrow->GetX1()<<", "<<myarrow->GetY1()<<" to "
				<<myarrow->GetX2()<<", "<<myarrow->GetY2()<<std::endl;
#endif
			trackfitarrows.push_back(myarrow);
		}
	}
}
