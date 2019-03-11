/* vim:set noexpandtab tabstop=4 wrap */

#include "MRDSubEventClass.hh"
#include "MRDspecs.hh"

#include "TCanvas.h"
#include "TBox.h"
#include "TText.h"
#include "TVector3.h"

#include <algorithm>
#include <cmath>
#include <cassert>

#ifndef DRAWVERBOSE
//#define DRAWVERBOSE
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
#endif

#ifndef DRAWSUPERVERBOSE
//#define DRAWSUPERVERBOSE
#endif

void cMRDSubEvent::DrawMrdCanvases(){
#ifdef DRAWVERBOSE
	cout<<"making canvas plots"<<endl;
#endif
	if(fillstaticmembers) FillStaticMembers();  // fills the canvas colours
	
//	// DEBUG STUFF
//	//////////////
//// XXX FROM HERE
//	//debug: highlight the corner paddles for checking
//	std::vector<int> holdme(pmts_hit); // XXX remember to disable this also at the end of this method
//	std::vector<int> pmtshit_debug{
//// test set 0:
//	// first H layer, bottom paddle of L and R sides
//	//0,1,
//	// first H layer, middle paddle of L and R sides
//	//(MRDSpecs::numpaddlesperpanelh/2)-1,(MRDSpecs::numpaddlesperpanelh/2),
//	// first H layer, top paddle of L and R sides
//	MRDSpecs::numpaddlesperpanelh-1,MRDSpecs::numpaddlesperpanelh-2,
//	//MRDSpecs::nummrdpmts-MRDSpecs::numpaddlesperpanelh,MRDSpecs::nummrdpmts-MRDSpecs::numpaddlesperpanelh+1,
//	//MRDSpecs::nummrdpmts-(MRDSpecs::numpaddlesperpanelh/2)-1,MRDSpecs::nummrdpmts-(MRDSpecs::numpaddlesperpanelh/2),
//	//MRDSpecs::nummrdpmts-2, MRDSpecs::nummrdpmts-1
//	};
//// test set 1:
////	0,2,4,6,8,10,12,14,16,18,20,22,24};  // all paddles in the RIGHT HAND (x>0) side of the first H layer.
//// test set 2:
////	for(auto&& apmt : pmtshit_debug) apmt++; // all left hand (x<0) paddles of first H layer. 
//// test set 3:
////	0,2,4,6,8,10,12,14,16,18,20,22,24,26,28};  // in conjunction with below:
////	for(auto&& apmt : pmtshit_debug) apmt+=MRDSpecs::numpaddlesperpanelh; // all TOP (y>0) paddles of first V layer.
//// test set 4:
////	for(auto&& apmt : pmtshit_debug) apmt++;  // all BOTTOM (y<0) paddles of first V layer.
//	
//	pmts_hit = pmtshit_debug; 
//	cout<<"pmts_hit={"; for(auto&& ahit : pmts_hit) cout<<ahit<<", "; cout<<"}"<<endl;
//	std::vector<double> holdmetoo(digi_ts);
//	std::vector<double> digi_tsdebug
//	{
//	0,220,
//	220,220,
//	220,220,
//	220,220,
//	220,220,
//	220,220};
//	(pmtshit_debug.size(),100.);
//	digi_ts=digi_tsdebug;
// XXX TO HERE
	
	// CONSTANTS USED FOR DEFINING CANVAS PADDLES
	/////////////////////////////////////////////
	Double_t scintboxwidth=1.4;  // a nominal "width" (depth) of the boxes representing paddles
	// paddle positions/sizes are scaled to the extent of the MRD to convert from cm to
	// a range of 0-1 to fit within the canvas, but some additional compression is required to
	// fit within the desired region
	double xmagicscaling  =1.2;
	double ymagicscaling  =1.2;
	double zmagicscaling  =1.4;
	// further reduction in the size of the relative width of the paddles otherwise they tend to 
	// merge together (since they are very closely spaced).
	Double_t topboxheight = (MRDSpecs::scintfullxlen/(MRDSpecs::maxwidth*xmagicscaling))*0.9;
	Double_t sideboxheight = (MRDSpecs::scintfullxlen/(MRDSpecs::maxwidth*xmagicscaling))*0.9;
	Double_t sideboxheight2 = (MRDSpecs::scintfullxlen2/(MRDSpecs::maxwidth*xmagicscaling))*0.9;
	Int_t paddlecolour;
	
	// FOR SCALING THE PADDLE COLOUR RANGE TO THE TIME RANGE OF HITS
	////////////////////////////////////////////////////////////////
	Double_t maxtime = (*std::max_element(digi_ts.begin(), digi_ts.end()));
	Double_t mintime = (*std::min_element(digi_ts.begin(), digi_ts.end()));
#ifdef DRAWVERBOSE
	cout<<"min time is "<<mintime<<endl;
	cout<<"max time is "<<maxtime+mintime<<endl;
#endif
	
	// BUILD A ROOT PAD THAT REPRESENTS THE MRD AND ITS PADDLES
	// highlight the paddles that were hit, and draw the reconstructed cells
	/////////////////////////////////////////////////////////////////////////
	static TText *texttop;
	static TText *textbottom;
	Double_t canvw=1200., canvh=700.;
	if(imgcanvas==0){
#ifdef DRAWVERBOSE
		cout<<"making canvases"<<endl;
#endif
		imgcanvas = new TCanvas("imgcanvas","MRD Digit Visualiser",canvw,canvh); 
		imgcanvas->Divide(2,1);
		imgcanvas->cd(1);
		titleleft = new TText(.32,.9,TString::Format("Side View, Event %d",event_id));
		titleleft->Draw();
		imgcanvas->cd(2);
		titleright = new TText(.32,.9,TString::Format("Top View, Event %d",event_id));
		titleright->Draw();
		imgcanvas->cd(2);
		for(int i=0; i<aspectrumv.size(); i++){
			TBox* colourbox = new TBox(0.02,0.1+(i*0.04),0.05,0.1+(i*0.04)+0.03);
			colourbox->SetFillColor(aspectrumv.at(i));
			colourbox->Draw();
		}
		char titlebuf[50];
		snprintf(titlebuf,50,"%d",(int)(maxtime*2));
		texttop = new TText(0.0,0.88,titlebuf);
		texttop->SetTextSize(0.04);
		texttop->Draw();
		snprintf(titlebuf,50,"%d",(int)mintime);
		textbottom = new TText(0.0,0.05,titlebuf);
		textbottom->SetTextSize(0.04);
		textbottom->Draw();
	} else {
#ifdef DRAWVERBOSE
		cout<<"re-drawing titles and time labels on canvases"<<endl;
#endif
		imgcanvas->cd(1);
//		gPad->Clear();
//		titleleft->Draw();
		titleleft->SetTitle(TString::Format("Side View, Event %d",event_id));
		titleleft->Draw();
		imgcanvas->cd(2);
//		gPad->Clear();
//		titleright->Draw();
		titleright->SetTitle(TString::Format("Top View, Event %d",event_id));
		titleright->Draw();
		char titlebuf[50];
		snprintf(titlebuf,50,"%d",(int)(maxtime*2));
		texttop->SetTitle(titlebuf);
		texttop->Draw();
		snprintf(titlebuf,50,"%d",(int)mintime);
		textbottom->SetTitle(titlebuf);
		textbottom->Draw();
	}
	
#ifdef DRAWVERBOSE
	cout<<"adding scints"<<endl;
#endif
	Bool_t firsthpaddledone=false, firstvpaddledone=false;
	//std::pair<double, double> xupcorner1, xupcorner2, xdowncorner1, xdowncorner2, yupcorner1, yupcorner2, ydowncorner1, ydowncorner2;	// part of cMRDSubEvent class now
	Bool_t rightsidehit=false, leftsidehit=false, tophit=false, bottomhit=false;
	
	// LOOP OVER ALL PADDLES AND HIGHLIGHT THAT THAT WERE HIT
	/////////////////////////////////////////////////////////
	for(int paddle=0; paddle<MRDSpecs::nummrdpmts; paddle++){
		
		// CHECK IF THE PADDLE WAS HIT BY FINDING IT'S PMT ID IN THE STRUCK PMTS
		////////////////////////////////////////////////////////////////////////
		Bool_t paddleishit;
		Double_t thetime;
		std::vector<Int_t>::iterator theit = std::find(pmts_hit.begin(), pmts_hit.end(), paddle);
		if(theit!=pmts_hit.end()){
			paddleishit=true;
			// grab the time the digit on this PMT. (what to do with more than one?)
			Int_t theindex = std::distance(pmts_hit.begin(),theit);
			thetime = digi_ts.at(theindex);
			Double_t relatime = (maxtime!=mintime) ? (thetime-mintime)/(maxtime-mintime) : 0.;
			Int_t colorindex = floor((aspectrumv.size()-1)*(relatime)); //relatime/2.
			paddlecolour = aspectrumv.at(colorindex);
			// kRed is an EColor, kRed+2 is an Int_t, representing generically a TColor, TBox requires a Color_t!
		} else {
			paddleishit=false;
		}
		
		// CALCULATE THE PADDLE POSITION FROM IT'S ID
		//////////////////////////////////////////////
		TVector3 origin;
		Double_t holderx, holdery, holderz, holderzoffset;
		Bool_t ishpaddle;
		ComputePaddleTransformation(paddle, origin, ishpaddle);
#ifdef DRAWSUPERVERBOSE
		cout<<"paddle "<<paddle<<" at ("<<origin.X()<<", "<<origin.Y()<<", "<<origin.Z()<<") was ";
		(paddleishit) ? cout<<" struck at "<<thetime : cout<<" not hit"; cout<<endl;
#endif
		
		// CHECK IF THIS HITS A NEW HALF OF THE HORIZONTAL OR VERTICAL PANEL
		// we'll use this to draw the vertical layers in side view & vice versa
		////////////////////////////////////////////////////////////////////////
		if(paddleishit&&origin.Y()>0){tophit=true;}
		else if(paddleishit&&origin.Y()<0){bottomhit=true;}
		if(paddleishit&&origin.X()>0){rightsidehit=true;}
		else if(paddleishit&&origin.X()<0){leftsidehit=true;}
#ifdef DRAWSUPERVERBOSE
		cout<<"updated stats are: "<<"tophit: "<<tophit<<", bottomhit: "<<bottomhit
			<<", rightsidehit: "<<rightsidehit<<", leftsidehit: "<<leftsidehit<<endl;
#endif
		
		
		// SCALE PHYSICAL 'CM' PADDLE POSITIONS TO CANVAS UNITS OF: 0-1, 0-1
		////////////////////////////////////////////////////////////////////////
		// flip x sign, a "top view" with z from Left->Right has 'RHS' for x>0, but that corresponds
		// to the LOWER (negative) half of the canvas -> i.e. x>0 should maps to canvas_y<0.
		// Scaling with some magic numbers... ???
		// scale by full width (and a magic fudge factor) & add 0.5 to go 0->1 not +-0.5
		holderx = 0.5+(-origin.X()/(MRDSpecs::maxwidth*xmagicscaling));
		holdery = (origin.Y()/(MRDSpecs::maxheight*ymagicscaling))+0.5;
		// two sides of each layer are shown side-by-side by shifting one with an offset in the 'z' direction
		Double_t anoffset=0;
		if((ishpaddle&&(origin.X()>0))||((!ishpaddle)&&(origin.Y()<0))){
			anoffset=(MRDSpecs::scintfullzlen+MRDSpecs::scintalugap)*5;
		}
		holderz = ((origin.Z()+anoffset)/(MRDSpecs::mrdZlen*zmagicscaling))+0.5;
		
//		cout<<"origin is: ("<<origin.X()<<","<<origin.Y()<<","<<origin.Z()<<")"<<endl;
//		cout<<"scaled origin is: ("<<holderx<<","<<holdery<<","<<holderz<<")"<<endl;
//		cout<<"ishpaddle="<<ishpaddle<<endl;
//		if(paddle==0){
//			cout<<"drawing box with height "<<(MRDSpecs::scintfullxlen/(MRDSpecs::maxwidth*xmagicscaling))
//				<<" and width "<<(scintboxwidth/(MRDSpecs::mrdZlen*1.2))<<endl;
//		}
		
		// Draw the box
		TBox *thepaddle;
		int layernum=MRDSpecs::paddle_layers.at(paddle);  //GetLayerNum(paddle);
		if(paddlepointers.at(paddle)!=0){
#ifdef DRAWSUPERVERBOSE
			cout<<"getting box pointer for paddle "<<paddle<<endl;
#endif
			thepaddle = paddlepointers.at(paddle);
		} else {
#ifdef DRAWSUPERVERBOSE
			cout<<"making box pointer for paddle "<<paddle<<endl;
#endif
			if(!ishpaddle){
				//TBox (Double_t leftx, Double_t bottomy, Double_t rightx, Double_t topy)
				double thesideboxheight;
				if((layernum%2==1)&&(layernum<4)){
					thesideboxheight = sideboxheight2;
				} else {
					thesideboxheight = sideboxheight;
				}
				thepaddle = new TBox(holderz,holderx-(thesideboxheight/2.),holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling)),holderx+(thesideboxheight/2.));
			} else {
				thepaddle = new TBox(holderz,holdery-(topboxheight/2.),holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling)),holdery+(topboxheight/2.));
			}
			paddlepointers.at(paddle) = thepaddle;
		}
		thepaddle->SetFillStyle(1001);	// solid fill
		if(paddleishit){ thepaddle->SetFillColor(paddlecolour); } else { thepaddle->SetFillColor(11); }
		// if the paddle was hit light it up - color 5 (kYellow) otherwise leave it dark - colour 11 (grey)
		thepaddle->SetLineColor(1);	//black
		thepaddle->SetLineWidth(2);
		(ishpaddle) ? imgcanvas->cd(1) : imgcanvas->cd(2);
		thepaddle->Draw();
		
		// check if we're changing panel
		if( paddle!=0 && (std::count(MRDSpecs::layeroffsets.begin(), MRDSpecs::layeroffsets.end(), (paddle+1))!=0) ){
#ifdef DRAWSUPERVERBOSE
			cout<<"drawing paddles in alternate view"<<endl;
#endif
			Double_t holderx1, holderx2, holdery1, holdery2;
			Double_t otherviewoffset;
			// this is the last paddle of this layer - draw the layer in the opposite view, combining all paddles
			if(!ishpaddle){
#ifdef DRAWSUPERVERBOSE
				cout<<"drawing paddles in side view"<<endl;
#endif
				otherviewoffset=-0.014;   // -0.04 places at "correct z"?
				imgcanvas->cd(1);
				holderx1=((MRDSpecs::scintalugap*10)/(MRDSpecs::maxwidth*xmagicscaling));
				holderx2=(MRDSpecs::scinthfullylen/(MRDSpecs::maxwidth*xmagicscaling));
				// some paddles are shown in both views, so we have more TBoxes than real PMTs
				Int_t overflowindex = MRDSpecs::nummrdpmts + (2*MRDSpecs::paddle_layers.at(paddle));	// RH paddle
				if(paddlepointers.at(overflowindex)==0){
					thepaddle = new TBox(holderz+otherviewoffset,0.5+holderx1,holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling))+otherviewoffset, 0.5+holderx1+holderx2);
					paddlepointers.at(overflowindex) = thepaddle;
				} else {
					thepaddle = paddlepointers.at(overflowindex);
				}
				thepaddle->SetFillStyle(1001);	// solid fill
				if(tophit){ thepaddle->SetFillColor(paddlecolour); } else { thepaddle->SetFillColor(11); }
#ifdef DRAWSUPERVERBOSE
				(tophit) ? cout<<"top paddle is hit"<<endl : cout<<"top paddle not hit"<<endl;
#endif
				// kYellow if hit or grey otherwise
				thepaddle->SetLineColor(1);	//black
				thepaddle->SetLineWidth(2);
				thepaddle->Draw();
				overflowindex++;	// left side paddle
				if(paddlepointers.at(overflowindex)==0){
					thepaddle = new TBox(holderz+otherviewoffset,0.5-holderx1,holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling))+otherviewoffset, 0.5-(holderx1+holderx2));
					paddlepointers.at(overflowindex) = thepaddle;
				} else {
					thepaddle = paddlepointers.at(overflowindex);
				}
				thepaddle->SetFillStyle(1001);	// solid fill
				if(bottomhit){ thepaddle->SetFillColor(paddlecolour); } else { thepaddle->SetFillColor(11); }
#ifdef DRAWSUPERVERBOSE
				(bottomhit) ? cout<<"bottom paddle is hit"<<endl : cout<<"bottom paddle not hit"<<endl;
#endif
				// kYellow if hit or grey otherwise
				thepaddle->SetLineColor(1);	//black
				thepaddle->SetLineWidth(2);
				thepaddle->Draw();
				
			} else {
#ifdef DRAWSUPERVERBOSE
				cout<<"drawing paddles in top view"<<endl;
#endif
				otherviewoffset=+0.01;
				imgcanvas->cd(2);
				holdery1=((MRDSpecs::scintalugap*10)/(MRDSpecs::maxheight*ymagicscaling));
				holdery2=(MRDSpecs::scintvfullylen/(MRDSpecs::maxheight*ymagicscaling));
				Int_t overflowindex = MRDSpecs::nummrdpmts + (2*MRDSpecs::paddle_layers.at(paddle));	// top paddle
				if(paddlepointers.at(overflowindex)==0){
					thepaddle = new TBox(holderz+otherviewoffset,holdery1+0.5,holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling))+otherviewoffset,0.5+holdery1+holdery2);
					paddlepointers.at(overflowindex) = thepaddle;
				} else {
					thepaddle = paddlepointers.at(overflowindex);
				}
				thepaddle->SetFillStyle(1001);	// solid fill
				if(leftsidehit){ thepaddle->SetFillColor(paddlecolour); } else { thepaddle->SetFillColor(11); }
#ifdef DRAWSUPERVERBOSE
				(leftsidehit) ? cout<<"left paddle is hit"<<endl : cout<<"left paddle not hit"<<endl;
#endif
				// kYellow if hit or grey otherwise
				thepaddle->SetLineColor(1);	//black
				thepaddle->SetLineWidth(2);
				thepaddle->Draw();
				overflowindex++;	// bottom paddle
				if(paddlepointers.at(overflowindex)==0){
					thepaddle = new TBox(holderz+otherviewoffset,0.5-holdery1,holderz+(scintboxwidth/(MRDSpecs::mrdZlen*zmagicscaling))+otherviewoffset,0.5-(holdery1+holdery2));
					paddlepointers.at(overflowindex) = thepaddle;
				} else {
					thepaddle = paddlepointers.at(overflowindex);
				}
				thepaddle->SetFillStyle(1001);	// solid fill
				if(rightsidehit){ thepaddle->SetFillColor(paddlecolour); } else { thepaddle->SetFillColor(11); }
#ifdef DRAWSUPERVERBOSE
				(rightsidehit) ? cout<<"right paddle is hit"<<endl : cout<<"right paddle not hit"<<endl;
#endif
				// kYellow if hit or grey otherwise
				thepaddle->SetLineColor(1);	//black
				thepaddle->SetLineWidth(2);
				thepaddle->Draw();
			}
			
			// reset for the next layer
			tophit=false; bottomhit=false; leftsidehit=false; rightsidehit=false;
#ifdef DRAWVERBOSE
			cout<<"layer finished, moving to next one"<<endl;
#endif
		}
	}
	//gPad->WaitPrimitive();	// wait for user to click
	// imgcanvas->SaveAs(TString::Format("mrdtracks_%d.png",event_id)); // invoke after adding tracks
//	pmts_hit = holdme;
//	digi_ts = holdmetoo;
}

// code taken from MRDDetectorConstruction.cc, WCSimDetectorConstruction::ComputePaddleTransformation
// with comments stripped, G4int->Int_t, G4bool->Bool_t, G4ThreeVector->TVector3, remove G4RotationMatrix
//============================= 
void cMRDSubEvent::ComputePaddleTransformation (const Int_t copyNo, TVector3 &origin, Bool_t &ishpaddle){
	double Xposition=0, Yposition=0, Zposition=0;
	
	int paddlenum;
	int panelnum = GetLayerNum(copyNo, &paddlenum, &ishpaddle);
	int pairnum = floor(paddlenum/2);
	
	for(int precedinglayer=0; precedinglayer<panelnum; precedinglayer++){ // scan over the preceding layers and add up the z offsets
		double thescintzlen;
		if((precedinglayer%2==1)&&(precedinglayer<4)){
			thescintzlen=MRDSpecs::scintfullzlen2;
		} else {
			thescintzlen=MRDSpecs::scintfullzlen;
		}
		Zposition += MRDSpecs::steelfullzlen + MRDSpecs::alufullzlen + MRDSpecs::layergap + thescintzlen;
	}
	
	Zposition = Zposition + MRDSpecs::steelfullzlen + MRDSpecs::steelscintgap;
	double thescintzlen = ((panelnum%2==1)&&(panelnum<4)) ? MRDSpecs::scintfullzlen2 : MRDSpecs::scintfullzlen;
	Zposition = Zposition + (thescintzlen/2.);
	Zposition = Zposition + MRDSpecs::MRDPMTRadius - (MRDSpecs::mrdZlen/2);
	
	
	if(ishpaddle){
		if (paddlenum%2==0){
			Xposition=((MRDSpecs::scinthfullylen+MRDSpecs::scintbordergap)/2.);
		} else {
			Xposition=-((MRDSpecs::scinthfullylen+MRDSpecs::scintbordergap)/2.);
		}
		Yposition = pairnum*(MRDSpecs::scintfullxlen+MRDSpecs::scintbordergap);
		Yposition = Yposition - 0.5*(((MRDSpecs::scintfullxlen+MRDSpecs::scintbordergap)*(MRDSpecs::numpaddlesperpanelh/2.))-MRDSpecs::scintbordergap)+(MRDSpecs::scintfullxlen/2);
	} else {
		if (paddlenum%2==0){
			Yposition=((MRDSpecs::scintvfullylen+MRDSpecs::scintbordergap)/2); 
		} else {
			Yposition=-((MRDSpecs::scintvfullylen+MRDSpecs::scintbordergap)/2);
		}
		double scintxlen = (panelnum==1||panelnum==3) ? MRDSpecs::scintfullxlen2 : MRDSpecs::scintfullxlen;
		Xposition = pairnum*(scintxlen+MRDSpecs::scintbordergap);
		Xposition = Xposition - 0.5*(((scintxlen+MRDSpecs::scintbordergap)*(MRDSpecs::numpaddlesperpanelvv.at((panelnum-1)/2)/2.))-MRDSpecs::scintbordergap)+(scintxlen/2); 
	}
	TVector3 theposition(Xposition,Yposition,Zposition);
	origin=theposition;
	
//	// DOUBLE CHECK:
//	std::cout<<"ComputePaddleTransformation Origin: ("<<origin.X()<<", "<<origin.Y()<<", "<<origin.Z()<<")"
//			<<", MRDSpecs origin: ("<<MRDSpecs::paddle_originx.at(copyNo)/10.
//			<<", "<<MRDSpecs::paddle_originy.at(copyNo)/10.
//			<<", "<<MRDSpecs::paddle_originz.at(copyNo)/10.-MRDSpecs::MRD_start-MRDSpecs::MRD_depth/2.
//			<<")"<<endl;
//	assert(origin.X()==(MRDSpecs::paddle_originx.at(copyNo)/10.));
//	assert(origin.Y()==(MRDSpecs::paddle_originy.at(copyNo)/10.));
//	assert(origin.Z()==((MRDSpecs::paddle_originz.at(copyNo)/10.)-MRDSpecs::MRD_start-MRDSpecs::MRD_depth/2.));
//	assert(panelnum==MRDSpecs::paddle_layers.at(copyNo));
//	assert(ishpaddle==bool(MRDSpecs::paddle_orientations.at(copyNo)));
	
}

int cMRDSubEvent::GetLayerNum(int copyNo, int* paddlenuminlayer, bool* ishpaddle){
	int layernum = 0;
	int runningtot=0;
	int lastrunningtot;
	do{
		lastrunningtot = runningtot;
		runningtot += GetNumPaddlesInLayer(layernum);
		if(copyNo < runningtot){
			if(paddlenuminlayer){ *paddlenuminlayer = copyNo - lastrunningtot; }
			if(ishpaddle){ (*ishpaddle) = (layernum%2==0); }
			return layernum;
		}
		layernum++;
	} while (1);
	std::string errorstring = "Error calling cMRDSubEvent::GetLayerNum for paddle "+std::to_string(copyNo)
		+ ": Paddle number out of bounds!";
	assert(false&&errorstring.c_str());
	
	return -1;
}

int cMRDSubEvent::GetNumPaddlesInLayer(int layernum){
	if((layernum%2)==0) return MRDSpecs::numpaddlesperpanelh;
	else return MRDSpecs::numpaddlesperpanelvv.at((layernum-1)/2);
}
