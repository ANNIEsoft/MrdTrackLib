/* vim:set noexpandtab tabstop=4 wrap */

#include "MRDSubEventClass.hh"
#include "MRDTrackClass.hh"
#include "MrdCluster.hh"
#include "MrdCell.hh"
#include "MRDspecs.hh"

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompChol.h"
#include "TDecompSVD.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>

#ifndef TRACKFINDVERBOSE
//#define TRACKFINDVERBOSE
#endif

class mrdcluster;
class mrdcell;
const Int_t mintracklength=0;	// tracks must have (mintracklength+1) cells to qualify
								// be generous - allow 1-cell tracks. We'll require tank coincidence anyway.
const Double_t chi2limit=125.0; //80.0;	// max chi^2 from a linear least squares fit to all 3 clusters for those
								// to be classed as clusters in the same track
								// 40 - offset of 1 cell. but doesn't allow multi-digit clusters //140
								// which increase opening angle to ~70. This doesn't allow large kinks
								// where deviation from straight projection is 2 cells.

void cMRDSubEvent::DoReconstruction(bool printtracks, bool drawcells, bool drawfit){
#ifdef __CINT__
	gSystem->Load("libMatrix");
#endif
//	// get digit tubes, layers and energy depositions. (not part of track reconstruction)
//	for(Int_t i=0;i<digi_ids.size();i++){
//		Int_t tube_id = pmts_hit.at(i);
//		Int_t strucklayer = mrdcluster::paddle_layers.at(tube_id);
//		if(std::count(layers_hit.begin(), layers_hit.end(), strucklayer)==0){
//			layers_hit.push_back(strucklayer);
//		}
//		eDepsInLayers.at(strucklayer)+= (digi_qs.at(i));	// TODO: convert q to energy?
//	}
	
	// 1. define a cluster, by an id and it's position. - done in cMRDSubEvent_ReconstructionClasses
	std::vector<mrdcluster*> theclusters;
	mrdcluster::clustercounter=0;	// reset for the start of a new event
	
	//2. define a cell as an object containing an ID, a pair of clusters, an integer state (initialised to 0),
	//   and a vector of neighbour cell ids. - done in cMRDSubEvent_ReconstructionClasses
	std::vector<mrdcell*> thecells;
	mrdcell::cellcounter=0;	// reset for the start of a new event
	
	// 3. find large clusters. where there are multiple hits in adjacent paddles within the same layer, these combine into a single cluster
	// sort digits into layers
#ifdef TRACKFINDVERBOSE
	std::cout<<"sorting digits into layers"<<std::endl;
#endif
	std::vector< std::vector<int> > digits_by_layer( (MRDSpecs::numpanels) );	//nb. inner parenthesis rqd!
	for(Int_t adigiindex=0; adigiindex<digi_ids.size(); adigiindex++){
		Int_t tube_id = pmts_hit.at(adigiindex);
		Int_t strucklayer = MRDSpecs::paddle_layers.at(tube_id);
		digits_by_layer.at(strucklayer).push_back(adigiindex);
	}
#ifdef TRACKFINDVERBOSE
	for(int layer=0; layer<MRDSpecs::numpanels; layer++){
		std::cout<<"found "<<digits_by_layer.at(layer).size()<<" digits in layer "<<layer<<std::endl;
	}
#endif
	
	// first make note of clusters spanning two or more adjacent pmts in the same layer
	// loop over layers, looking for multiple digits in adjacent pmts
	std::vector<Int_t> donedigits;
	for(int thislayer=0; thislayer<MRDSpecs::numpanels; thislayer++){
		if(digits_by_layer.at(thislayer).size()==0) continue;
		std::vector<Int_t> digitsinthislayer = digits_by_layer.at(thislayer);
		// it will be easier to perform a linear scan across the layer looking for adjacent pmts
		// this requires that digits in the layer are sorted by tube_id in ascending order
		
		
		// make a vector of pairs of digits in this layer, pair is <tube_id, digi_index>
		// then sort this vector - it will be sorted by tube_id.
		// then scan through the vector of pairs, comparing tube_ids. if match, use digi_index
		// part to retrieve the other required information
#ifdef TRACKFINDVERBOSE
		std::cout<<"building vector of pairs in layer "<<thislayer<<" for sorting"<<std::endl;
		std::cout<<"digits in this layer include:"<<std::endl;
#endif
		std::vector< std::pair<Int_t, Int_t> > digitssortedbytube;
		for(int j=0; j<digitsinthislayer.size(); j++){
			Int_t adigiindex = digitsinthislayer.at(j);
			Int_t atubeid = pmts_hit.at(adigiindex);
#ifdef TRACKFINDVERBOSE
			std::cout<<"   tube "<<atubeid<<", digit "<<adigiindex<<std::endl;
#endif
			digitssortedbytube.push_back(std::pair<Int_t, Int_t>(atubeid, adigiindex));
		}
#ifdef TRACKFINDVERBOSE
		std::cout<<"sorting pairs in this layer by tube id"<<std::endl;
#endif
		std::sort(digitssortedbytube.begin(), digitssortedbytube.end());
#ifdef TRACKFINDVERBOSE
		std::cout<<"sorted set of pairs is:"<<std::endl;
		for(int pairveci=0; pairveci<digitssortedbytube.size(); pairveci++){
			std::cout<<"   tube "<<digitssortedbytube.at(pairveci).first
				<<", digit "<<digitssortedbytube.at(pairveci).second<<std::endl;
		}
#endif
		
		// scan through all digits except the last one, in a forward direction
		// 1. look at the next digit's tube. If it's adjacent to this digit's tube, they're a cluster.
		// 2. if this digit is already in a cluster, add the next digit.
		// 3. otherwise, create a cluster with this digit and add the next digit.
#ifdef TRACKFINDVERBOSE
		std::cout<<"searching for multi-paddle clusters"<<std::endl;
#endif
		mrdcluster* thisdigitscluster=nullptr;
		for(Int_t thisdigit=0; thisdigit<(digitsinthislayer.size()-1); thisdigit++){
			// get digit index
			Int_t tube_id = digitssortedbytube.at(thisdigit).first;
			Int_t tube_id2 = digitssortedbytube.at(thisdigit+1).first;
			// merge the two halves so we can see if the tubes are adjacent
			Int_t posid1 = (tube_id%2==1) ? (tube_id-1)/2 : (tube_id/2);
			Int_t posid2 = (tube_id2%2==1) ? (tube_id2-1)/2 : (tube_id2/2);
			// if the next digit's tube_id is adjacent to that of this digit
			if(abs(posid1-posid2)<2){
				// they're a cluster! grab the digi_ids
				Int_t adigiindex = digitssortedbytube.at(thisdigit).second;
				Int_t adigiindex2 = digitssortedbytube.at(thisdigit+1).second;
				//check if this digit is already in a cluster
				if(thisdigitscluster==nullptr){
					// it's not: make a cluster and add both digits
#ifdef TRACKFINDVERBOSE
					std::cout<<"creating multi-paddle cluster in layer "<<thislayer
						<<" from digits in tubes "<<tube_id<<" and "<<tube_id2<<std::endl;
#endif
					thisdigitscluster = new mrdcluster(adigiindex, tube_id, thislayer, digi_ts.at(adigiindex));
					theclusters.push_back(thisdigitscluster);
					thisdigitscluster->AddDigit(adigiindex2, tube_id2, digi_ts.at(adigiindex2));
					donedigits.push_back(adigiindex);
					donedigits.push_back(adigiindex2);
				} else {
					// it is: add the next digit to this digit's cluster
#ifdef TRACKFINDVERBOSE
					std::cout<<"adding digiti in tube "<<tube_id2<<" to this cluster"<<std::endl;
#endif
					thisdigitscluster->AddDigit(adigiindex2, tube_id2, digi_ts.at(adigiindex2));
					donedigits.push_back(adigiindex2);
				}
			} else {
				thisdigitscluster=nullptr;
			}
		}
#ifdef TRACKFINDVERBOSE
		std::cout<<"found "<<theclusters.size()<<" multi-digit clusters so far"<<std::endl;
#endif
	}
	
	// 4. make any remaining non-grouped hits their own cluster.
#ifdef TRACKFINDVERBOSE
	std::cout<<"constructed "<<theclusters.size()<<" multi-paddle clusters."<<std::endl
		<<"Making remaining digits into clusters"<<std::endl;
#endif
	for(Int_t adigiindex=0; adigiindex<digi_ids.size(); adigiindex++){
		if(std::count(donedigits.begin(), donedigits.end(), adigiindex)==0){
			Int_t tube_id = pmts_hit.at(adigiindex);
			Int_t strucklayer = MRDSpecs::paddle_layers.at(tube_id);
			mrdcluster* thisdigitscluster = new mrdcluster(adigiindex, tube_id, strucklayer, digi_ts.at(adigiindex));
			theclusters.push_back(thisdigitscluster);
		}
	}
#ifdef TRACKFINDVERBOSE
	std::cout<<"constructed "<<theclusters.size()<<" total clusters from "<<digi_ids.size()<<" digits"<<std::endl;
#endif
	// When generating cells we run over a nested loop: the clusters should be ordered by layer,
	// so that cells between adjacent layers are generated before looking for cells spanning two
	// layers. Multi-digit clusters therefore need to be sorted into place. 
	while(true) {
		bool madechange=false;
		for(int index=0; index<theclusters.size()-1; index++){
			if((theclusters.at(index)->layer)>(theclusters.at(index+1)->layer)){
				std::swap(theclusters.at(index),theclusters.at(index+1));
				madechange=true;
			}
		}
		if(!madechange) break;
	}
	
	// 5. generate a std::vector all the possible cells for each layer
	for(int thislayer=0; thislayer<MRDSpecs::numpanels; thislayer++){
	// loop over all clusters in the layer ... 
#ifdef TRACKFINDVERBOSE
		std::cout<<"constructing cells from clusters starting in layer "<<thislayer<<std::endl;
#endif
		for(int thiscluster=0; thiscluster<theclusters.size(); thiscluster++){
			mrdcluster* startcluster = theclusters.at(thiscluster);
			if(startcluster->layer==thislayer){
				std::vector<mrdcell*> thisclusterscells;
				// ... and generate a cell between it and all clusters in the next and next-next layer
				// - but separating the two views. So 'next' is 'next horizontal' or 'next vertical' as appopriate.
				for(int thiscluster2=0; thiscluster2<theclusters.size(); thiscluster2++){
					mrdcluster* endcluster = theclusters.at(thiscluster2);
					Int_t nextlayer = endcluster->layer;
					if((nextlayer>thislayer)&&((nextlayer-thislayer)<5)&&((nextlayer-thislayer)%2==0)){
						// if making a cell between two adjacent layers, make the cell - 
						// no need to check for equivalent cells with intermediate layers
						if(((nextlayer-thislayer)==2)||(thisclusterscells.size()==0)){
#ifdef TRACKFINDVERBOSE
							std::cout<<"generating a cell between cluster "<<thiscluster
								<<" with centre "<<startcluster->GetCentreIndex()<<" in layer "<<thislayer
								<<" and cluster "<<thiscluster2
								<<" with centre "<<endcluster->GetCentreIndex()
								<<" in layer "<<nextlayer<<std::endl;
#endif
							// generate the cell between these thiscluster and thiscluster2
							mrdcell* thiscell = new mrdcell(startcluster, endcluster);
							thecells.push_back(thiscell);
							thisclusterscells.push_back(thiscell);
						} else {
							// check if a cell already exists starting from startcluster and going to 
							// an intermediate cluster in perfect alignment with this endcluster
							// if so, we will skip making this cell, as it will overlap with the existing cell
#ifdef TRACKFINDVERBOSE
							std::cout<<"scanning for intermediate cells between cluster "<<thiscluster
								<<" with centre "<<startcluster->GetCentreIndex()<<" in layer "<<thislayer
								<<" and cluster "<<thiscluster2
								<<" with centre "<<endcluster->GetCentreIndex()
								<<" in layer "<<nextlayer<<std::endl;
#endif
							Bool_t foundintermediate=false;
							// scan through existing cells starting from this cluster
							for(int acelli=0; acelli<thisclusterscells.size(); acelli++){
								mrdcell* acell = thisclusterscells.at(acelli);
								// skip if the cell end cluster is in nextlayer - it's not intermediate
								if(acell->clusters.second->layer==nextlayer) {
#ifdef TRACKFINDVERBOSE
									std::cout<<"skipping cell to present layer (can't be intermediate)"<<std::endl;
#endif
									continue;
								}
								// existing cell is in the intermediate layer - 
								// does the projection to nextlayer hit this cluster?
								double startclustercentre = startcluster->GetCentreIndex();
								mrdcluster* secondcluster = acell->clusters.second;
								double midclustercentre = secondcluster->GetCentreIndex();
								int xdiff = midclustercentre-startclustercentre;
								double projectedendpoint = startclustercentre+(2*xdiff);
#ifdef TRACKFINDVERBOSE
								std::cout<<"projection of cell from tube "<<startclustercentre
									<<" in layer "<<thislayer<<" to tube "<<midclustercentre
									<<" in layer "<<acell->clusters.second->layer
									<<" forward to layer "<<nextlayer<<" gives tube "
									<<projectedendpoint<<", compared to present cluster's centre of "
									<<endcluster->GetCentreIndex()<<std::endl;
#endif
								// allow larger kinks
								if(abs(projectedendpoint-endcluster->GetCentreIndex())<2){
									foundintermediate=true;  // same as existing cell
#ifdef TRACKFINDVERBOSE
									std::cout<<"this is an intermediate, skipping this endpoint cluster."<<std::endl;
#endif
									break;
								}
							}
							if(!foundintermediate){
#ifdef TRACKFINDVERBOSE
								std::cout<<"no intermediate cells found, creating a new cell"<<std::endl;
#endif
								// no existing cell that overlaps with this one; make the cell
								mrdcell* thiscell = new mrdcell(startcluster, endcluster);
								thecells.push_back(thiscell);
								thisclusterscells.push_back(thiscell);
							}
						}
					}
				}
			}
		}
	}
#ifdef TRACKFINDVERBOSE
	std::cout<<std::endl<<"constructed a total of "<<thecells.size()<<" cells"<<std::endl;
#endif
	
	//6. fill the neighbourhood for each cell. the neighbours are defined considering a pair of cells:
	//  a. the two cells must have a shared cluster (there will be two end clusters and a shared middle cluster)
	//  b. the clusters must be sufficiently aligned - the x^2 of a linear fit through all 3 cluster centres,
	// with errors defined by paddle boundaries, must be less than chi2limit.
	//  c. then there's some tricky stuff based on track splitting and converging. 
	// if an uptrack cell already has a downtrack cell, then another downtrack candidate is found, then the
	// track splits. In this case, we unset the previous downtrack candidate as a neighbour and instead make
	// both downtrack cells "daughters".
	// if a downtrack cell is found to align with multiple uptrack cells, then the uptrack cells converge.
	// in this case, we take the uptrack candidate with the best chi2 as the uptrack cell, and ignore others.
	//  d. finally, we do the scan from upstream to downstream, but the definition of "uptrack" and "downtrack"
	// must account for upstream directed tracks. We use the cell direction to account for this. 
	
	// loop over cells
	for(int acelli=0; acelli<thecells.size(); acelli++){
		mrdcell* upcell = thecells.at(acelli);
		mrdcluster* sharedcluster1 = upcell->clusters.second;
#ifdef TRACKFINDVERBOSE
		std::cout<<"searching for downstream neighbours of cell "<<acelli
			<<" with downstream cluster at "<<sharedcluster1<<std::endl;
#endif
		mrdcell* thiscellsneighbour = nullptr;
		// scan over other downstream cells
		for(int bcelli=(acelli+1); bcelli<thecells.size(); bcelli++){
			mrdcell* downcell = thecells.at(bcelli);
			mrdcluster* sharedcluster2 = downcell->clusters.first;
#ifdef TRACKFINDVERBOSE
			//std::cout<<"checking cell with upstream cluster at "<<sharedcluster2<<std::endl;
#endif
			// check if they share a cluster
			if(sharedcluster1==sharedcluster2){
#ifdef TRACKFINDVERBOSE
				std::cout<<"shared cluster! Checking if they're aligned enough to be a track"<<std::endl;
#endif
				// they share a central cluster. Next check they are sufficiently 'aligned' to be considered
				// part of the same track. Do this by checking a x^2 fit to a straight line through all clusters
				mrdcluster* upcluster = upcell->clusters.first;
				mrdcluster* downcluster = downcell->clusters.second;
				// make a linear-least-square fit to the three cluster centres, with errors on the three
				// centres based on the cluster paddle limits
#ifdef TRACKFINDVERBOSE
				//std::cout<<"checking clusters at "<<upcluster<<", "<<sharedcluster1<<", "<<downcluster<<std::endl;
#endif
				Double_t clusterzcentres[] = 
					{(Double_t)upcluster->layer, (Double_t)sharedcluster1->layer, (Double_t)downcluster->layer};
				Double_t clusterxcentres[] = 
					{upcluster->GetCentre(), sharedcluster1->GetCentre(), downcluster->GetCentre()};
				Double_t upclustererror = upcluster->GetXmax() - upcluster->GetXmin();
				Double_t sharedclustererror = sharedcluster1->GetXmax() - sharedcluster1->GetXmin();
				Double_t downclustererror = downcluster->GetXmax() - downcluster->GetXmin();
				Double_t clusterxerrors[] = {upclustererror, sharedclustererror, downclustererror};
				Double_t fit_gradient=0, fit_offset=0, chi2 = 0.;
				// is errors the same as weighting in chi^2 implementation?
#ifdef TRACKFINDVERBOSE
				std::cout<<"Performing least squares fit with centres at y values ("
					<<clusterxcentres[0]<<", "<<clusterxcentres[1]<<", "<<clusterxcentres[2]<<"), y errors ("
					<<clusterxerrors[0]<<", "<<clusterxerrors[1]<<", "<<clusterxerrors[2]<<"), z values ("
					<<clusterzcentres[0]<<", "<<clusterzcentres[1]<<", "<<clusterzcentres[2]<<")"<<std::endl;
#endif
				LeastSquaresMinimizer(3, clusterzcentres, clusterxcentres, 0, clusterxerrors, 
					fit_gradient, fit_offset, chi2);
#ifdef TRACKFINDVERBOSE
				std::cout<<"fit produces gradient "<<fit_gradient<<", offset "<<fit_offset<<", chi^2 "<<chi2<<std::endl;
#endif
				if(chi2>=0&&chi2<chi2limit){  // fit can fail with negative chi2. 
#ifdef TRACKFINDVERBOSE
					std::cout<<"chi2 is good, checking for existing neighbours and daughters"<<std::endl;
#endif
					// these cells are neighbours! 
					// determine the track direction to see how to proceed << FUDGED. 
					if(true||(upcell->isdownstreamgoing&&downcell->isdownstreamgoing)){
						// both are cells of a downstream going track.
						// now we need to check if this cell is already the upstream neighbour of
						// another cell - i.e. does the track split here
						if((upcell->dtneighbourcellindex==-2)||(upcell->dtneighbourcellindex>-1)){
// Previously the idea here was to look for tracks that split - i.e. if 2 cells are potential downstream candidates,
// the track 'splits' and the downstream tracks are considered independent from each other, and from the daughter.
// Unfortunately in practice this fragments tracks, and parent segments get pruned as they are not long enough.
// Instead, we'll just have to choose one or the other... and hope we choose right, because the other daughter may die!
// TODO: Perhaps add proper support for keeping all daughters associated with the parent ...
//
//#ifdef TRACKFINDVERBOSE
//							std::cout<<"this is a second downtrack candidate! Track splits!"<<std::endl;
//#endif
//							// we need to un-set the existing neighbour. Instead set both downstream
//							// candidates as their own new tracks, and call this track the 'parent'
//							if(upcell->dtneighbourcellindex!=-2){
//								mrdcell* dtneighbourcell = thecells.at(upcell->dtneighbourcellindex);
//								dtneighbourcell->parentcellindex = upcell->cellid;
//								dtneighbourcell->utneighbourcellindex = -1;
//								dtneighbourcell->neighbourchi2 = 0.;
//								upcell->dtneighbourcellindex=-2;
//							}
//							downcell->parentcellindex = upcell->cellid;
//							upcell->hasdaughters = true;
#ifdef TRACKFINDVERBOSE
							std::cout<<"this is a second downtrack candidate! Choosing best match"<<std::endl;
#endif
							// we need to un-set the existing neighbour. Instead set both downstream
							// candidates as their own new tracks, and call this track the 'parent'
							if(!((downcluster->layer)>(thecells.at(upcell->dtneighbourcellindex)->clusters.second->layer)) &&
								chi2<(thecells.at(upcell->dtneighbourcellindex)->neighbourchi2)){
#ifdef TRACKFINDVERBOSE
								std::cout<<"this match has a better chi2, overriding existing pairing"<<std::endl;
								std::cout<<"considering cell "<<acelli<<" the upstream neighbour of cell "<<bcelli<<std::endl;
#endif
								downcell->utneighbourcellindex = upcell->cellid;
								downcell->neighbourchi2 = chi2;
								mrdcell* olddtneighbourcell = thecells.at(upcell->dtneighbourcellindex);
								upcell->dtneighbourcellindex=downcell->cellid;
								// wipe old downstream neighbour details
								olddtneighbourcell->utneighbourcellindex = -1;
								 // should we still leave some of this info? 
								olddtneighbourcell->neighbourchi2 = 0.;
								olddtneighbourcell->parentcellindex = upcell->cellid;
								downcell->parentcellindex = upcell->cellid;
								upcell->hasdaughters = true;
							} else {
#ifdef TRACKFINDVERBOSE
								std::cout<<"this match has a worse chi2, ignoring this pairing"<<std::endl;
#endif
							}
						} else {
#ifdef TRACKFINDVERBOSE
							std::cout<<"no existing downtrack candidate..."<<std::endl;
#endif
							// this cell does not have a downstream cell yet. If the downstream cell has no
							// existing upstream cell, use this one. If the downstream cell already has an
							// upstream neighbour defined (converging upstream tracks), use the better fit
							if(downcell->utneighbourcellindex!=-1){
							std::cout<<"existing candidate chi2="<<downcell->neighbourchi2<<std::endl;
							std::cout<<"existing candidate upstream layer="
								<<thecells.at(downcell->utneighbourcellindex)->clusters.first->layer<<std::endl;
							std::cout<<"new candidate chi2="<<chi2<<std::endl
								<<"new candidate upstream layer="<<upcell->clusters.first->layer<<std::endl;
							}
							if( (downcell->utneighbourcellindex==-1) || 
								(downcell->utneighbourcellindex!=-1 && chi2<downcell->neighbourchi2) ||
								(downcell->utneighbourcellindex	!=-1 && chi2<(downcell->neighbourchi2*3.) &&
								upcell->clusters.first->layer > 
								(thecells.at(downcell->utneighbourcellindex)->clusters.first->layer)) ){
#ifdef TRACKFINDVERBOSE
								std::cout<<"considering cell "<<acelli<<" the upstream neighbour of cell "<<bcelli<<std::endl;
#endif
								downcell->utneighbourcellindex = upcell->cellid;
								downcell->neighbourchi2 = chi2;
								upcell->dtneighbourcellindex = downcell->cellid;
							} else {
								// else this cell already has a neighbour with a better fit. Nothing to do.
#ifdef TRACKFINDVERBOSE
								std::cout<<"... but upstream or downstream cell already has a better fit!"<<std::endl;
#endif
							}
						}
						// we need to check if the track splits, so continue scanning downstream cells
					}
					/* else if((!(upcell->isdownstreamgoing))&&(!(downcell->isdownstreamgoing))){
						// both are components of upstream going tracks. Check for any existing daughters
						// of this parent
						if(downcell->dtneighbourcellindex!=-1){
#ifdef TRACKFINDVERBOSE
							std::cout<<"this is a second downtrack candidate! Track splits!"<<std::endl;
#endif
							// this cell has already been given a down-track cell
							if(downcell->dtneighbourcellindex!=-2){
								mrdcell* dtneighbourcell = thecells.at(downcell->dtneighbourcellindex);
								dtneighbourcell->parentcellindex = downcell->cellid;
								dtneighbourcell->utneighbourcellindex = -1;
								dtneighbourcell->neighbourchi2 = 0.;
								downcell->dtneighbourcellindex=-2;
							}
							upcell->parentcellindex = downcell->cellid;
							downcell->hasdaughters = true;
						} else {
							// this cell has no existing downtrack cells, so proceed to make neighbours
							// (first checking if we have an upstream neighbour - choose best)
							if( (upcell->utneighbourcellindex==-1) || 
							(upcell->utneighbourcellindex!=-1 && chi2<upcell->neighbourchi2) ){
								// this cell has no upstream candidate, or this one has a better alignment
#ifdef TRACKFINDVERBOSE
								std::cout<<"considering cell "<<bcelli<<" the upstream neighbour of cell "<<acelli<<std::endl;
#endif
								upcell->utneighbourcellindex = downcell->cellid;
								upcell->neighbourchi2 = chi2;
								downcell->dtneighbourcellindex = upcell->cellid;
							} // else this cell already has a better fitting uptrack neighbour. Nothing to do.
						}
					}	// end if based on upstream/downstream
					*/
				} else {
					// end if based on chi2 limit.
#ifdef TRACKFINDVERBOSE
					std::cout<<"chi2 insufficient, cells not aligned"<<std::endl;
#endif
				}
			}	// end if based on shared cluster
		}	// end loop over downstream cells
#ifdef TRACKFINDVERBOSE
			if(upcell->dtneighbourcellindex==-1) std::cout<<"no downstream neighbour cell found"<<std::endl;
			else std::cout<<"downstream neighbour cell found"<<std::endl;
#endif
	}	// end loop over upstream cells
	
	int cellswithoutupstreamneighbours=0;
	int cellswithupstreamneighbours=0;
	for(int acelli=0; acelli<thecells.size(); acelli++){
		mrdcell* thiscell = thecells.at(acelli);
		if(thiscell->utneighbourcellindex==-1) { cellswithoutupstreamneighbours++; }
		else { cellswithupstreamneighbours++; }
	}
#ifdef TRACKFINDVERBOSE
	std::cout<<"found "<<cellswithoutupstreamneighbours<<" cells without any upstream neighbours"
		<<" and "<<cellswithupstreamneighbours<<" cells with upstream neighbours"<<std::endl;
#endif
	
	// 7. now the automation [ do {...} while (...) ]
	//  a. for each iteration, loop over all cells
	//   > if any upstream neighbour cells have the same state as this cell, increment this cell's state number
	// repeat while there at least 1 cell has a neighbour with the same state number.
	if(cellswithupstreamneighbours){
#ifdef TRACKFINDVERBOSE
		std::cout<<"doing automation!"<<std::endl;
#endif
		int loopiterations=0;
		Bool_t madeachange;
		std::vector <Bool_t> newcellstatuses(thecells.size());
		do {
	#ifdef TRACKFINDVERBOSE
			std::cout<<"automation loop "<<loopiterations<<std::endl;
	#endif
			madeachange=false;
			for(int acelli=0; acelli<thecells.size(); acelli++){
				mrdcell* thiscell = thecells.at(acelli);
				if(thiscell->utneighbourcellindex!=-1){
					// the cell has a neighbour: compare statuses
					mrdcell* neighbourcell = thecells.at(thiscell->utneighbourcellindex);
					if(thiscell->status==neighbourcell->status){
						// the cell has the same status as it's upstream neighbour - increment it's status
						newcellstatuses.at(acelli) = true;
						madeachange=true;
#ifdef TRACKFINDVERBOSE
						std::cout<<"incrementing status of cell "<<acelli<<std::endl;
#endif
					} else {
						// the cell doesn't have the same status as upstream neighbour - keep same status
						newcellstatuses.at(acelli) = false;
					}
				} else {
					// no neighbour - don't increment any statuses
					newcellstatuses.at(acelli) = false;
				}
			}
			// now perform the actual status incrementing - we have to do this independantly of status checking
			for(int acelli=0; acelli<thecells.size(); acelli++){
				if(newcellstatuses.at(acelli)) thecells.at(acelli)->IncrementStatus();
			}
			loopiterations++;
		} while (madeachange&&loopiterations<30);
		assert(loopiterations!=29 && "automation loop failed to exit normally!");
		// should never have loopiterations > (max(numhlayers, numvlayers)-1) 
		//  - this would be a track longer than the MRD!
	}
	
	// 8. cells with a state of >= mintracklength and no (upstream) neighbours are defined as 
	// the downstream track ends.
	std::vector<Int_t> trackstartindices; 
	for(int acelli=0; acelli<thecells.size(); acelli++){
		mrdcell* thiscell = thecells.at(acelli);
		// check if the cell is a down-track endpoint - i.e. if it has downtrack neighbours
		if(thiscell->dtneighbourcellindex>-1) continue;
		// filter short tracks here by also not making tracks if the downtrack endpoint has a low status
		// value - i.e. few upstream neighbours. We allow short tracks if they instead have or are a parent. 
		if((thiscell->status<mintracklength)&&(thiscell->parentcellindex==-1)&&(thiscell->hasdaughters==false)){
#ifdef TRACKFINDVERBOSE
			std::cout<<"skipping short track with endpoint cell "<<acelli<<std::endl;
#endif
			continue;
		}
		trackstartindices.push_back(acelli);	// this is a valid downstream end cell!
	}
#ifdef TRACKFINDVERBOSE
	std::cout<<"found "<<trackstartindices.size()<<" track startpoints"<<std::endl;
#endif
	
	// 9. These cells are combined with upstream neighbours until the cell with state 0 (upstream edge). 
	//    This combination is the track. 
	std::vector<std::vector<mrdcell*> > hpaddletracks, vpaddletracks;
	for(int atracki=0; atracki<trackstartindices.size(); atracki++){
		std::vector<mrdcell*> thistrack;
		mrdcell* thiscell = thecells.at(trackstartindices.at(atracki));
		thistrack.push_back(thiscell);
#ifdef TRACKFINDVERBOSE
		std::cout<<"track "<<atracki<<" has start cell index "<<trackstartindices.at(atracki)<<std::endl;
#endif
		do {
			int nextupcellindex = thiscell->utneighbourcellindex;
#ifdef TRACKFINDVERBOSE
			std::cout<<"cell "<<thiscell<<" has neighbour index "<<nextupcellindex<<std::endl;
#endif
			if(nextupcellindex==-1) {
				if(thiscell->status!=0) std::cerr<<"cell with nonzero status but no upstream neighbour!"<<std::endl;
				break;
			}
#ifdef TRACKFINDVERBOSE
			std::cout<<"adding cell "<<nextupcellindex<<" to track "<<atracki<<std::endl;
#endif
			thiscell = thecells.at(nextupcellindex);
			thistrack.push_back(thiscell);
		} while (1);
#ifdef TRACKFINDVERBOSE
		std::cout<<"track "<<atracki<<" has a total of "<<thistrack.size()<<" cells"<<std::endl;
#endif
		if(thistrack.size()){
			if((thiscell->clusters.first->layer)%2==0) hpaddletracks.push_back(thistrack);
			else vpaddletracks.push_back(thistrack);
		}
	}
	
	// Tracks need to be matched in the two views
	// we'll define a 'figure of merit' for a given track to match it to it's pair in the opposite view.
	// this will be based on:
	// same side
	// similar start and end z
	// similar crossing point, if applicable
	// similar track length
#ifdef TRACKFINDVERBOSE
	std::cout<<"building match matrix for matching tracks in two views"<<std::endl;
	std::cout<<"we have "<<hpaddletracks.size()<<" tracks in horizontal paddles and "
		<<vpaddletracks.size()<<" tracks in vertical paddles"<<std::endl;
#endif
	std::vector<std::vector<double> > matchmerits (hpaddletracks.size(), std::vector<double>(vpaddletracks.size(),0));
	for(int tracki=0; tracki<hpaddletracks.size(); tracki++){
		std::vector<mrdcell*> htrack = hpaddletracks.at(tracki);
#ifdef TRACKFINDVERBOSE
		std::cout<<"scanning matches against htrack "<<tracki<<" which has "<<htrack.size()<<" cells"<<std::endl;
#endif
		for(int trackj=0; trackj<vpaddletracks.size(); trackj++){
			std::vector<mrdcell*> vtrack = vpaddletracks.at(trackj);
			
			/*
			First requirement is that the track in the opposing view is on the correct side.
			Scan through hit layers, using a pair of layers in the more accurate view.
			The side in the intermediate layer (alt-view, if hit) should be the same as the two main
			view paddles. If both main-view paddles are central, either intermediate side is allowed.
			*/
#ifdef TRACKFINDVERBOSE
			std::cout<<"making figure-of-merit with v track "<<trackj
				<<", which has "<<vtrack.size()<<" cells"<<std::endl;
#endif
			
			// first test how compatible v track (alt) is with h track (main)
			double thisfom=0.;
			int hcomparisons=0;
			int hfom=0;
			// scan over side view hits, for all intermediate hits in alt view, update fom based on side
			for(int testi=htrack.size()-1; testi>-1; testi--){
				// pull out the next pair of clusters - what layer are they in?
				int startlayer = htrack.at(testi)->clusters.first->layer;
				int stoplayer = htrack.at(testi)->clusters.second->layer;
				
				// and what side are they on?
				int startpmt = htrack.at(testi)->clusters.first->GetCentreIndex();
				startpmt-= ((MRDSpecs::numpaddlesperpanelh/2.)-1.)/2.;  // -6 -> centre paddle will be 0.
				double hsidestart=0;  // 0 indicates a centre paddle - neither side.
					 if(startpmt>0) hsidestart= 1;            // right hand side
				else if(startpmt<0) hsidestart=-1;            // left hand side
				double stoppmt = htrack.at(testi)->clusters.second->GetCentreIndex();
				stoppmt-= ((MRDSpecs::numpaddlesperpanelh/2.)-1.)/2.;   // -6 -> centre paddle will be 0.
				int hsidestop=0;  // 0 indicates a centre paddle - neither side.
					 if(stoppmt>0) hsidestop= 1;              // right hand side
				else if(stoppmt<0) hsidestop=-1;              // left hand side
				double avgside = (hsidestart+hsidestop)/2.;   // this averages to 0 in
															  // case they're on opposite sides
//				if(avgside==0){
//#ifdef TRACKFINDVERBOSE
//					std::cout<<"average side of hcell "<<testi<<" is 0, no compatibility check"<<std::endl;
//#endif
//					continue;  // paddles are central - either side is consistent. 								   // no change to figure of merit
//				}
#ifdef TRACKFINDVERBOSE
				std::cout<<"htrack cell "<<testi<<" starts in layer "<<startlayer<<" on side "<<hsidestart
					<<" and goes to layer "<<stoplayer<<" on side "<<hsidestop
					<<" giving average side "<<avgside<<std::endl;
#endif
				
				// now scan over the other view hits
				for(int testj=vtrack.size()-1; testj>-1; testj--){
					// look for hits between the current pair of layers
					int interlayer=vtrack.at(testj)->clusters.first->layer;
					int outerlayer=vtrack.at(testj)->clusters.second->layer;
#ifdef TRACKFINDVERBOSE
					std::cout<<"vtrack cell "<<testj<<" starts in layer "<<interlayer<<std::endl;
#endif
					if( (interlayer>startlayer&&interlayer<stoplayer) || 
						((interlayer==startlayer-1)&&(outerlayer==stoplayer+1)) ||
						(outerlayer>startlayer&&outerlayer<stoplayer) ){
						double altside;
						if(interlayer>startlayer&&interlayer<stoplayer){
							// alternate cluster layer is between this cell's layers
							altside = vtrack.at(testj)->clusters.first->altviewhalf;
						} else {
							// alternate view cell straddles this cell, use average side of clusters
							altside = vtrack.at(testj)->clusters.first->altviewhalf +
									  vtrack.at(testj)->clusters.second->altviewhalf;
						}
#ifdef TRACKFINDVERBOSE
						std::cout<<"vtrack cell "<<testj<<" in alt view is on side "<<altside<<std::endl;
#endif
						if(!(outerlayer>startlayer&&outerlayer<stoplayer)){
							if((avgside*altside)>0){
								// both are on the same side:
								hfom+=1;
							} else if ((avgside*altside)<0){
								// they're on opposite sides
								hfom-=1;
							} else if(avgside==0){
								if((interlayer==startlayer-1)&&(outerlayer==stoplayer+1)){
									// this cell crosses sides, and alternate view cell straddles this cell
									// Alt view should also cross sides.
									if(altside==0) hfom+=1; else hfom-=1;
								} else if(interlayer>startlayer&&interlayer<stoplayer){
									// this cell crosses sides, and alternate view cell starts between this 
									// cell's layers and crosses sides.
									if(altside==0) hfom+=1; // No demerit
								} else {
									hcomparisons--; // counter the increment below.
								}
							}
							hcomparisons++;
						} else {
							// this cell crosses sides. We should also look for side crossings on cells
							// ending between this cell's layers, not just starting between them.
							if(avgside==0&&altside==0){
								hfom+=1;
								hcomparisons++;
							}
						}
					}
					
					if(testj==0){  // one last test on the last cluster
						int interlayer=vtrack.at(testj)->clusters.second->layer;
#ifdef TRACKFINDVERBOSE
						std::cout<<"vtrack cell "<<testj<<" ends in layer "<<interlayer<<std::endl;
#endif
						if(interlayer>startlayer&&interlayer<stoplayer){
							// check if the side is the same as the straddling hits
							double altside = vtrack.at(testj)->clusters.second->altviewhalf;
#ifdef TRACKFINDVERBOSE
							std::cout<<"vtrack cell "<<testj<<" in alt view is on side "<<altside<<std::endl;
#endif
							if((avgside*altside)>0){
								// both are on the same side:
								hfom+=1;
							} else if ((avgside*altside)<0){
								// they're on opposite sides
								hfom-=1;
							} // else no preference
							hcomparisons++;
						}
					}
				}
			}
#ifdef TRACKFINDVERBOSE
			std::cout<<"net FOM from side comparison in h view is "<<hfom<<"/"<<hcomparisons<<std::endl;
#endif
			if(hcomparisons) thisfom+= 2.*((double)hfom/(pow((double)hcomparisons,0.3)));
#ifdef TRACKFINDVERBOSE
			std::cout<<"thisfom="<<thisfom<<std::endl;
#endif
			
			// now check how compatible h track (alt) is with v track (main)
			int vcomparisons=0;
			int vfom=0;
			// scan over top view hits, for all intermediate hits in alt view, update fom based on side
			for(int testi=vtrack.size()-1; testi>-1; testi--){
				// pull out the next pair of clusters - what layer are they in?
				int startlayer = vtrack.at(testi)->clusters.first->layer;
				int stoplayer = vtrack.at(testi)->clusters.second->layer;
				
				// and what side are they on?
				int startpmt = vtrack.at(testi)->clusters.first->GetCentreIndex();
				startpmt-= ((MRDSpecs::numpaddlesperpanelv/2.)-1.)/2.;  // -7 -> centre paddle will be 0.
				double vsidestart=0;  // 0 indicates a centre paddle - neither side.
					 if(startpmt>0) vsidestart= 1;            // right hand side
				else if(startpmt<0) vsidestart=-1;            // left hand side
				double stoppmt = vtrack.at(testi)->clusters.second->GetCentreIndex();
				stoppmt-= ((MRDSpecs::numpaddlesperpanelv/2.)-1.)/2.;   // -7 -> centre paddle will be 0.
				int vsidestop=0;  // 0 indicates a centre paddle - neither side.
					 if(stoppmt>0) vsidestop= 1;              // right hand side
				else if(stoppmt<0) vsidestop=-1;              // left hand side
				double avgside = (vsidestart+vsidestop)/2.;   // this averages to 0 in
															  // case they're on opposite sides
#ifdef TRACKFINDVERBOSE
				std::cout<<"vtrack cell "<<testi<<" starts in layer "<<startlayer<<" on side "<<vsidestart
					<<" and goes to layer "<<stoplayer<<" on side "<<vsidestop
					<<" giving average side "<<avgside<<std::endl;
#endif
//				if(avgside==0){
//#ifdef TRACKFINDVERBOSE
//					std::cout<<"average side of vcell "<<testi<<" is 0, no compatibility check"<<std::endl;
//#endif
//					continue;
//				}
				
				
				// now scan over the other view hits
				for(int testj=htrack.size()-1; testj>-1; testj--){
					// look for hits between the current pair of layers
					int interlayer=htrack.at(testj)->clusters.first->layer;
					int outerlayer=htrack.at(testj)->clusters.second->layer;
#ifdef TRACKFINDVERBOSE
					std::cout<<"htrack cell "<<testj<<" starts in layer "<<interlayer<<std::endl;
#endif
					if( (interlayer>startlayer&&interlayer<stoplayer) || 
						((interlayer==startlayer-1)&&(outerlayer==stoplayer+1)) ||
						(outerlayer>startlayer&&outerlayer<stoplayer) ){
						double altside;
						if(interlayer>startlayer&&interlayer<stoplayer){
							// alternate cluster layer is between this cell's layers
							altside = htrack.at(testj)->clusters.first->altviewhalf;
						} else {
							// alternate view cell straddles this cell, use average side of clusters
							altside = htrack.at(testj)->clusters.first->altviewhalf +
									  htrack.at(testj)->clusters.second->altviewhalf;
						}
#ifdef TRACKFINDVERBOSE
						std::cout<<"htrack cell "<<testj<<" in alt view is on side "<<altside<<std::endl;
#endif
						if(!(outerlayer>startlayer&&outerlayer<stoplayer)){
							if((avgside*altside)>0){
								// both are on the same side:
								vfom+=1.;
							} else if ((avgside*altside)<0){
								// they're on opposite sides
								vfom-=1.;
							} else if(avgside==0){
								if((interlayer==startlayer-1)&&(outerlayer==stoplayer+1)){
									// this cell crosses sides, and alternate view cell straddles this cell
									// Alt view should also cross sides.
									if(altside==0) vfom+=1; else vfom-=1;
								} else if(interlayer>startlayer&&interlayer<stoplayer){
									// this cell crosses sides, and alternate view cell starts between this 
									// cell's layers and crosses sides.
									if(altside==0) vfom+=1; // No demerit
								} else {
									vcomparisons--; // counter the increment below.
								}
							}
							vcomparisons++;
						} else {
							// this cell crosses sides. We should also look for side crossings on cells
							// ending between this cell's layers, not just starting between them.
							if(avgside==0&&altside==0){
								vfom+=1;
								vcomparisons++;
							}
						}
					}
					
					if(testj==0){  // one last test on the last cluster
						int interlayer=htrack.at(testj)->clusters.second->layer;
#ifdef TRACKFINDVERBOSE
						std::cout<<"htrack cell "<<testj<<" ends in layer "<<interlayer<<std::endl;
#endif
						if(interlayer>startlayer&&interlayer<stoplayer){
							double altside = htrack.at(testj)->clusters.second->altviewhalf;
#ifdef TRACKFINDVERBOSE
							std::cout<<"htrack cell "<<testj<<" in alt view is on side "<<altside<<std::endl;
#endif
							if((avgside*altside)>0){
								// both are on the same side:
								vfom+=1.;
							} else if ((avgside*altside)<0) {
								// they're on opposite sides
								vfom-=1.;
							} // else one of them is a multi-paddle cluster spanning both sides. null merit.
							vcomparisons++;
						}
					}
				}
			}
#ifdef TRACKFINDVERBOSE
			std::cout<<"net FOM from side comparison in v view is "<<vfom<<"/"<<vcomparisons<<std::endl;
#endif
			if(vcomparisons) thisfom+= 2.*((double)vfom/(pow((double)vcomparisons,0.3)));
#ifdef TRACKFINDVERBOSE
			std::cout<<"thisfom="<<thisfom<<std::endl;
#endif
			
#ifdef TRACKFINDVERBOSE
			std::cout<<"checking compatibility of start and endpoints"<<std::endl;
#endif
			// give some merit based on proximity of the start and endpoints
			int hstartlayer = htrack.back()->clusters.first->layer;
			int vstartlayer = vtrack.back()->clusters.first->layer;
			int hstoplayer = htrack.front()->clusters.second->layer;
			int vstoplayer = vtrack.front()->clusters.second->layer;
			
#ifdef TRACKFINDVERBOSE
			std::cout<<"updating figure-of-merit for endpoint matching by "
				<<-(((abs(hstartlayer-vstartlayer)+1.)/2.)-2.)
				<<" for start proximity (layers "<<hstartlayer<<", "<<vstartlayer<<") and "
				<<-(((abs(hstoplayer-vstoplayer)+1.)/2.)-2.)
				<<" for end proximity (layers "<<hstoplayer<<", "<<vstoplayer<<")"<<std::endl;
#endif
			
			// adjacent layers in start and end points give FOM+1...
			thisfom -= ((abs(hstartlayer-vstartlayer)+1.)/2.)-2.;
#ifdef TRACKFINDVERBOSE
			std::cout<<"thisfom="<<thisfom<<std::endl;
#endif
			thisfom -= ((abs(hstoplayer-vstoplayer)+1.)/2.)-2.;
#ifdef TRACKFINDVERBOSE
			std::cout<<"thisfom="<<thisfom<<std::endl;
#endif 
			double deltah = htrack.front()->clusters.second->GetCentre()
							-htrack.back()->clusters.first->GetCentre();
			double deltaxh = MRDSpecs::mrdscintlayers.at(hstoplayer)-MRDSpecs::mrdscintlayers.at(hstartlayer);
			double coshang = cos(atan(deltah/deltaxh));
			
			double deltav = vtrack.front()->clusters.second->GetCentre()
							-vtrack.back()->clusters.first->GetCentre();
			double deltaxv = MRDSpecs::mrdscintlayers.at(vstoplayer)-MRDSpecs::mrdscintlayers.at(vstartlayer);
			double cosvang = cos(atan(deltav/deltaxv));
#ifdef TRACKFINDVERBOSE
			std::cout<<"updating figure-of-merit based on steepness of track angle by "
				<<coshang<<" for horizontal angle and "<<cosvang<<" for vertical angle"<<std::endl;
#endif
			thisfom+=(coshang+cosvang);
			thisfom+=(vtrack.size() + htrack.size())/2.;
#ifdef TRACKFINDVERBOSE
			std::cout<<"updating figure-of-merit based on number of cells in tracks: "
				<<htrack.size()/2<<" for horizontal track and "<<vtrack.size()/2
				<<" for vertical track"<<std::endl
				<<"FINAL FOM FOR MATCH: "<<thisfom<<std::endl;
#endif
			matchmerits.at(tracki).at(trackj)=thisfom;
		}
	}
	
#ifdef TRACKFINDVERBOSE
	std::cout<<"matching tracks in two views"<<std::endl;
#endif
	double fomthreshold=7.0; // a pair of tracks must have at least this figure-of-merit to be matched
						   // XXX needs tuning
						   // XXX ADD MORE FOM FOR MORE CELLS IN A TRACK.
	// now find the maximum merit pairs, noting that there may not be the same number of tracks in each view!
	std::vector<std::pair<int,int> > matchedtracks;  // <hpaddletracks index, vpaddletracks index>
	while(true){
		double currentmax=0.;
		int maxrow=-1, maxcolumn=-1;
		for(int rowi=0; rowi<matchmerits.size(); rowi++){
			std::vector<double> therow = matchmerits.at(rowi);
			std::vector<double>::iterator thisrowsmaxit = std::max_element(therow.begin(), therow.end());
			if((thisrowsmaxit!=therow.end())&&((*thisrowsmaxit)>currentmax)){
				currentmax=*thisrowsmaxit; 
				maxrow = rowi;
				maxcolumn = std::distance(therow.begin(),thisrowsmaxit);
			}
		}
#ifdef TRACKFINDVERBOSE
		std::cout<<"max FOM for pairing "<<matchedtracks.size()<<" is "<<currentmax<<std::endl;
#endif
		// OK, in theory we should just find the maximum of the matchmerits matrix, match that 
		// pair, mask off that row/column, and repeat.
		// in practice we have a bunch of fringe cases such as tracks that merge or split,
		// for which we want to avoid creating tracks, even if they have the best FOM.
		// first check if this is a single-cell track joining two already assigned clusters:
		if(currentmax>fomthreshold){
			bool makethepair=true;
			std::vector<mrdcell*> htrack = hpaddletracks.at(maxrow);
			std::vector<mrdcell*> vtrack = vpaddletracks.at(maxcolumn);
#ifdef TRACKFINDVERBOSE
			std::cout<<"candidate track "<<matchedtracks.size()<<" htrack has "<<htrack.size()
				<<" cells, goes from ("<<htrack.front()->clusters.second->GetCentreIndex()
				<<", "<<htrack.front()->clusters.second->layer<<") to ("
				<<htrack.back()->clusters.first->GetCentreIndex()<<", "
				<<htrack.back()->clusters.first->layer<<"), vtrack has "<<vtrack.size()
				<<" cells, goes from ("<<vtrack.front()->clusters.second->GetCentreIndex()
				<<", "<<vtrack.front()->clusters.second->layer<<") to ("
				<<vtrack.back()->clusters.first->GetCentreIndex()<<", "
				<<vtrack.back()->clusters.first->layer<<"),"<<std::endl;
#endif
			if(htrack.size()<3||vtrack.size()<3){ // aggressively prune bridges
				bool hisbadmatch = BridgeSearch(htrack, matchedtracks, hpaddletracks, "h");
#ifdef TRACKFINDVERBOSE
				if(hisbadmatch) std::cout<<"h track in this pair bridges two existing tracks, removing it."<<std::endl;
#endif
				// we're going to discard this track from all future considerations, so we should
				// update the merit table to remove it from future pair considerations
				if(hisbadmatch) matchmerits.at(maxrow).assign(matchmerits.at(maxrow).size(),-1);
				// same now for the vtrack part of the pair
				bool visbadmatch = BridgeSearch(vtrack, matchedtracks, vpaddletracks, "V");
#ifdef TRACKFINDVERBOSE
				if(visbadmatch) std::cout<<"v track in this pair bridges two existing tracks, removing it."<<std::endl;
#endif
				if(visbadmatch) for(auto&& avec : matchmerits) avec.at(maxcolumn) = -1;
				if(hisbadmatch||visbadmatch) continue;
				
				/////////////
				// second fringe case: better matches from a split track than an independent track.
				// we want to prefer the case of the independent track, even if the fom is a little lower.
				// so:
				// IF either of the tracks in this candidate pair (htrack, vtrack) 
				// have clusters that are already part of an existing matched track, then search for
				// an alternative that does not have any cluster sharing.
				
#ifdef TRACKFINDVERBOSE
				std::cout<<"searching for shared clusters"<<std::endl;
#endif
				// ok, first check the htrack
				bool hhassharedcluster = 
					SearchForClusterInTracks(matchedtracks, hpaddletracks, htrack, "h");
#ifdef TRACKFINDVERBOSE
				if(hhassharedcluster) std::cout<<"h track in this pair shares clusters with existing track."<<std::endl;
#endif
				bool vhassharedcluster = 
					SearchForClusterInTracks(matchedtracks, vpaddletracks, vtrack, "v");
#ifdef TRACKFINDVERBOSE
				if(vhassharedcluster) std::cout<<"v track in this pair shares clusters with existing track."<<std::endl;
#endif
				if(hhassharedcluster&&!vhassharedcluster){
					// OK, so we're looking at a pair where the h part of the track splits/merges with an
					// existing track. Let's see if there's another match for the v part that doesn't
					// need this counterpart.
					// other possible matches for the v part are in the same column.
#ifdef TRACKFINDVERBOSE
					std::cout<<"searching for alternative h track matches to this v track"
						<<" - this pairing has fom "<<currentmax<<std::endl;
#endif
					std::vector<double> otherhcands; // fom column for this v track
					for(int rowi=0; rowi<matchmerits.size(); rowi++){
						std::vector<double> therow = matchmerits.at(rowi);
						if(rowi==maxrow) otherhcands.push_back(-1); // mask off current match
						else otherhcands.push_back(therow.at(maxcolumn));
					}
					while(true) {
						// find the next best matching element for the v part
						auto nextmaxfomit = std::max_element(otherhcands.begin(), otherhcands.end());
#ifdef TRACKFINDVERBOSE
						std::cout<<"next best match to v track has fom "<<(*nextmaxfomit);
#endif
						if((*nextmaxfomit/currentmax)>0.8){
							// the next best match is also pretty good...
							// but we need to see whether THAT has shared clusters (don't want) 
							// or whether it has a better match to a different v track without shared clusters!
							auto althrow = std::distance(otherhcands.begin(), nextmaxfomit);
							std::vector<mrdcell*> alternatehtrack = hpaddletracks.at(althrow);
							bool althhassharedcluster= 
								SearchForClusterInTracks(matchedtracks, hpaddletracks, alternatehtrack, "h");
							if(althhassharedcluster){
								// this other candidate also has shared clusters, so strike it from
								// the alternate candidates vector and find the next best match
#ifdef TRACKFINDVERBOSE
								std::cout<<" however it also has shared clusters"<<std::endl;
#endif
								otherhcands.at(althrow)=-1;
								continue;
							} else {
#ifdef TRACKFINDVERBOSE
								std::cout<<" and it does not have shared clusters"<<std::endl;
#endif
							}
							// if it doesn't have any shared clusters, does it have a better match
							// with another v vector?
							std::vector<double> othermatchesforalt = matchmerits.at(althrow);
							while(true){
								auto aahfom = 
									std::max_element(othermatchesforalt.begin(), othermatchesforalt.end());
#ifdef TRACKFINDVERBOSE
								std::cout<<"this alternative's best match has fom "<<*aahfom;
#endif
								if(*aahfom<=fomthreshold) break;
								auto aahfi = std::distance(othermatchesforalt.begin(), aahfom);
								if(aahfi==maxcolumn){
									// this is another h track, with no shared clusters, 
									// producing a fairly good pairing (>80% previous fom), and whose best
									// non-shared match is our v track. Prefer to pair this with the v track.
#ifdef TRACKFINDVERBOSE
									std::cout<<" with our current vtrack. Preventing original pairing"<<std::endl;
#endif
									makethepair=false;
									break;
								} else {
									// the proposed alternative h track has no shared clusters, but does
									// have a better match with another v track. Last thing is to see if
									// that v track has any shared clusters
									auto avv = vpaddletracks.at(aahfi);
									bool ahvhsc = 
										SearchForClusterInTracks(matchedtracks, vpaddletracks, avv, "v");
									if(!ahvhsc){
										// this alternative has a better match to another track without
										// any shared clusters. To not mess up that pairing, do not use.
#ifdef TRACKFINDVERBOSE
										std::cout<<" to a track with no shared clusters."
											<<"Looking for other alternatives."<<std::endl;
#endif
										break;
									} else {
#ifdef TRACKFINDVERBOSE
										std::cout<<" but it's match also has a shared track."
											<<"Looking at other matches to this alternative."<<std::endl;
#endif
										// the best previous pairing for that track has a shared cluster.
										// keep searching it's other pairings.
										othermatchesforalt.at(aahfi)=-1;
									}
								}
							}
							otherhcands.at(althrow)=-1; // we've checked this track now.
							// if we don't break in the following line, it wasn't suitable - perhaps
							// because it had a better non-shared match, or because it's fom with the v
							// track wasn't up to standards
							if(!makethepair){
#ifdef TRACKFINDVERBOSE
								std::cout<<"found a suitable htrack alternative. Not making the pair."<<std::endl;
#endif
								break; // we've found a suitable better match
							} else {
							// otherwise, continue to the next alternative match for the v track.
#ifdef TRACKFINDVERBOSE
								std::cout<<" done checking this htrack alternative. Moving to next one."<<std::endl;
#endif
							}
						} else {
							// next best match for the v part isn't so great (<80% fom)
							// let's give up looking for alternatives and make this pair.
#ifdef TRACKFINDVERBOSE
							std::cout<<"No more htrack alternatives. We'll use it."<<std::endl;
#endif
							break;
						}
					} // while loop over other h candidates for the v part
				}
				//-------------
				
				// and now a big duplicate for the case of a split v track with not-split h track:
				if(vhassharedcluster&&!hhassharedcluster){
#ifdef TRACKFINDVERBOSE
					std::cout<<"searching for alternative v track matches to this h track"
						<<" - this pairing has fom "<<currentmax<<std::endl;
#endif
					std::vector<double> othervcands=matchmerits.at(maxrow);
					othervcands.at(maxcolumn)=-1;
					while(true) {
						auto nextmaxfomit = std::max_element(othervcands.begin(), othervcands.end());
#ifdef TRACKFINDVERBOSE
						std::cout<<"next best match to v track has fom "<<(*nextmaxfomit);
#endif
						if((*nextmaxfomit/currentmax)>0.8){
							auto altvcol = std::distance(othervcands.begin(), nextmaxfomit);
							std::vector<mrdcell*> alternatevtrack = vpaddletracks.at(altvcol);
							bool altvhassharedcluster= 
								SearchForClusterInTracks(matchedtracks, vpaddletracks, alternatevtrack, "v");
							if(altvhassharedcluster){
#ifdef TRACKFINDVERBOSE
								std::cout<<" however it also has shared clusters"<<std::endl;
#endif
								othervcands.at(altvcol)=-1;
								continue;
							} else {
#ifdef TRACKFINDVERBOSE
								std::cout<<" and it does not have shared clusters"<<std::endl;
#endif
							}
							std::vector<double> othermatchesforalt;
							for(auto arow : matchmerits) othermatchesforalt.push_back(arow.at(maxcolumn));
							while(true){
								auto aavfom = 
									std::max_element(othermatchesforalt.begin(), othermatchesforalt.end());
#ifdef TRACKFINDVERBOSE
								std::cout<<"this alternative's best match has fom "<<*aavfom;
#endif
								if(*aavfom<=fomthreshold) break;
								auto aavfi = std::distance(othermatchesforalt.begin(), aavfom);
								if(aavfi==maxcolumn){
#ifdef TRACKFINDVERBOSE
									std::cout<<" with our current htrack. Preventing original pairing"<<std::endl;
#endif
									makethepair=false;
									break;
								} else {
									auto ahh = hpaddletracks.at(aavfi);
									bool ahvhsc = 
										SearchForClusterInTracks(matchedtracks, hpaddletracks, ahh, "h");
									if(!ahvhsc){
#ifdef TRACKFINDVERBOSE
										std::cout<<" to a track with no shared clusters."
											<<"Looking for other alternatives."<<std::endl;
#endif
										break;
									} else {
#ifdef TRACKFINDVERBOSE
										std::cout<<" but it's match also has a shared track."
											<<"Looking at other matches to this alternative."<<std::endl;
#endif
										othermatchesforalt.at(aavfi)=-1;
									}
								}
							}
							othervcands.at(altvcol)=-1;
							if(!makethepair){
#ifdef TRACKFINDVERBOSE
								std::cout<<"found a suitable vtrack alternative. Not making the pair."<<std::endl;
#endif
								break;
							} else {
#ifdef TRACKFINDVERBOSE
								std::cout<<" done checking this vtrack alternative. Moving to next one."<<std::endl;
#endif
							}
						} else {
#ifdef TRACKFINDVERBOSE
							std::cout<<"No more vtrack alternatives. We'll use it."<<std::endl;
#endif
							break;
						}
					} // while loop over other v candidates for the h part
				}
				// end search over remaining tracks to find a better one with lower fom
				
				// OK, we've tried our best to find alternates, but there might not be anything better.
				// still, we're not keen on having tracks built from clusters in existing tracks.
				// update the FOM, and re-evaluate if we still want to make this:
				if(hhassharedcluster) currentmax-=1.5;
				if(vhassharedcluster) currentmax-=1.5;
				if(currentmax<fomthreshold) makethepair=false;
				
			} // else - this new candidate has >= 3 cells - bypass additional pruning tests
			
			// add this matching to the list of pairs
			if(makethepair){
#ifdef TRACKFINDVERBOSE
				std::cout<<"matching horizontal track "<<maxrow<<" to vertical track "<<maxcolumn
					<<", match FOM "<<currentmax<<std::endl;
#endif
				matchedtracks.push_back(std::make_pair(maxrow, maxcolumn));
			}
			// remove the matched tracks from the matchmerits matrix
			matchmerits.at(maxrow).assign(matchmerits.at(maxrow).size(),-1);
			for(auto&& avec : matchmerits) avec.at(maxcolumn) = -1;
		} else {
			break;  // the maximum FOM between remaining tracks is insufficient. We're done here.
		}
	}
	
	//////////////////////
	// end of track pairing.
	//////////////////////
	
	// only record tracks which are seen in both views
	if(printtracks) std::cout<<"Found "<<matchedtracks.size()<<" tracks this subevent."<<std::endl;
	// Make the actual cMRDTrack track objects, now that we have a coherent set of cells:
	for(int tracki=0; tracki<matchedtracks.size(); tracki++){
		// get the track (vector of cells)
		std::vector<mrdcell*> hpaddletrack = hpaddletracks.at(matchedtracks.at(tracki).first);
		std::vector<mrdcell*> vpaddletrack = vpaddletracks.at(matchedtracks.at(tracki).second);
#ifdef TRACKFINDVERBOSE
		std::cout<<"track "<<tracki<<" has "<<hpaddletrack.size()<<" horizontal and "
			<<vpaddletrack.size()<<" vertical cells"<<std::endl;
#endif
			
		// declare all the things to make a track class
		std::vector<Int_t> digitindexes_inthistrack;
		std::vector<Int_t> tubeids_inthistrack;
		std::vector<Double_t> digitqs_inthistrack;
		std::vector<Double_t> digittimes_inthistrack;
		std::vector<Int_t> digitnumphots_inthistrack;
		std::vector<Double_t> digittruetimes_inthistrack;
		std::vector<Int_t> digittrueparents_inthistrack;
		
		// scan through all the cells, and grab the digit ids of all digits in all clusters in this track
		// once for the hpaddle track... 
		// we also duplicate the cells and clusters here to save them as objects within the MRDTrack
		std::vector<mrdcell> thehtrackcells, thevtrackcells;
		std::vector<mrdcluster> htrackclusters, vtrackclusters;
#ifdef TRACKFINDVERBOSE
		std::cout<<"getting digit information for htrack clusters"<<std::endl;
#endif
		for(int celli=0; celli<hpaddletrack.size(); celli++){
			mrdcell* acell = hpaddletrack.at(celli);
			thehtrackcells.emplace_back(mrdcell(*acell, thehtrackcells.size()));
			mrdcluster* downcluster = acell->clusters.second;
			htrackclusters.emplace_back(*downcluster);
			digitindexes_inthistrack.insert(digitindexes_inthistrack.end(), downcluster->digitindexes.begin(), downcluster->digitindexes.end());
			// also add the upstream cluster if this is the last (most upstream) cell
			if(celli==hpaddletrack.size()-1){
				mrdcluster* upcluster = acell->clusters.first;
				htrackclusters.emplace_back(*upcluster);
				digitindexes_inthistrack.insert(digitindexes_inthistrack.end(), upcluster->digitindexes.begin(), upcluster->digitindexes.end());
			}
		}
		// ...and once for the vpaddle track
#ifdef TRACKFINDVERBOSE
		std::cout<<"getting digit information for vtrack clusters"<<std::endl;
#endif
		for(int celli=0; celli<vpaddletrack.size(); celli++){
			mrdcell* acell = vpaddletrack.at(celli);
			thevtrackcells.emplace_back(mrdcell(*acell, thevtrackcells.size()));
			mrdcluster* downcluster = acell->clusters.second;
			vtrackclusters.emplace_back(*downcluster);
			digitindexes_inthistrack.insert(digitindexes_inthistrack.end(), downcluster->digitindexes.begin(), downcluster->digitindexes.end());
			if(celli==vpaddletrack.size()-1){
				mrdcluster* upcluster = acell->clusters.first;
				// since clusters are shared between cells, take the upcluster only from the first cell
				vtrackclusters.emplace_back(*upcluster);
				digitindexes_inthistrack.insert(digitindexes_inthistrack.end(), upcluster->digitindexes.begin(), upcluster->digitindexes.end());
			}
		}
		
#ifdef TRACKFINDVERBOSE
		std::cout<<"getting additional digit information"<<std::endl;
#endif
		// use the digit indices to fill in the rest of the information about digits in this track
		for(int digiti=0; digiti<digitindexes_inthistrack.size(); digiti++){
			Int_t thedigitindex = digitindexes_inthistrack.at(digiti);
			tubeids_inthistrack.push_back(pmts_hit.at(thedigitindex));
			digitqs_inthistrack.push_back(digi_qs.at(thedigitindex));
			digittimes_inthistrack.push_back(digi_ts.at(thedigitindex));
			digitnumphots_inthistrack.push_back(digi_numphots.at(thedigitindex));
			digittruetimes_inthistrack.push_back(digi_phot_ts.at(thedigitindex));
			digittrueparents_inthistrack.push_back(digi_phot_parents.at(thedigitindex));
		}
		
#ifdef TRACKFINDVERBOSE
		std::cout<<"emplacing the cMRDTrack"<<std::endl;
#endif
		//emplace_back creates the cMRDTrack in-place to avoid a copy.
		tracksthissubevent.emplace_back(tracki, wcsimfile, run_id, event_id, mrdsubevent_id, trigger, digitindexes_inthistrack, tubeids_inthistrack, digitqs_inthistrack, digittimes_inthistrack, digitnumphots_inthistrack, digittruetimes_inthistrack, digittrueparents_inthistrack, thehtrackcells, thevtrackcells, htrackclusters, vtrackclusters);
		
#ifdef TRACKFINDVERBOSE
		std::cout<<"printing and drawing"<<std::endl;
#endif
		// if requested, print and/or draw the track
		int trackcolourindex;
		EColor thistrackscolour, fittrackscolour;
		if(drawcells||drawfit){
			trackcolourindex=tracki+1; // element 0 is black
			while(trackcolourindex+1>=trackcolours.size()) trackcolourindex-=(trackcolours.size()-1);
			thistrackscolour = trackcolours.at(trackcolourindex);
			fittrackscolour = trackcolours.at(trackcolourindex+1); // for now, give it a diff color
		}
		cMRDTrack* thatrack = &(tracksthissubevent[tracksthissubevent.size()-1]);
		if(printtracks) thatrack->Print();
		if(drawcells) thatrack->DrawReco(imgcanvas, trackarrows, thistrackscolour, paddlepointers);
		if(drawfit) thatrack->DrawFit(imgcanvas, trackfitarrows, fittrackscolour);
	}
	
#ifdef TRACKFINDVERBOSE
	std::cout<<"cleaning up heap clusters and cells"<<std::endl;
#endif
	// CLEANUP: cMRDTrack stores everything we need: the heap allocations can be freed
	for(auto acluster : theclusters){
		delete acluster;
	}
	theclusters.clear();
	for(auto acell : thecells){
		delete acell;
	}
	thecells.clear();
#ifdef TRACKFINDVERBOSE
	std::cout<<"cleanup complete, returning"<<std::endl;
#endif
}

void cMRDSubEvent::DrawTracks(){
	for(auto thetrack : tracksthissubevent){
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

void cMRDSubEvent::LeastSquaresMinimizer(Int_t numdatapoints, Double_t datapointxs[], Double_t datapointys[], Double_t datapointweights[], Double_t errorys[], Double_t &fit_gradient, Double_t &fit_offset, Double_t &chi2){
	// linear least squares finder from $ROOTSYS/tutorials/matrix/solveLinear.C
	const Int_t nrVar  = 2;				// number of...? fit variables? 2 because a linear fit?
	Int_t method = 2;					// implementation method to use
	
//	// now passed as args
//	//const Int_t numdatapoints = 3;
//	Double_t datapointxs[]      = {0.0,1.0,2.0};
//	Double_t datapointys[]      = {1.4,1.5,3.7};
//	Double_t datapointweights[] = {0.5,0.2,1.0};
	
	// Make the vectors 'Use" the data : they are not copied, the vector data
	// pointer is just set appropriately
	TVectorD x; x.Use(numdatapoints,datapointxs);
	TVectorD y; y.Use(numdatapoints,datapointys);
	TVectorD e;
	if(datapointweights!=0) e.Use(numdatapoints,datapointweights);
	
	TMatrixD A(numdatapoints,nrVar);
	TMatrixDColumn(A,0) = 1.0;
	TMatrixDColumn(A,1) = x;
	
	// Two possible methods: first the 'Normal Equations' whose derivation can be found
	// here: http://mathworld.wolfram.com/LeastSquaresFitting.html
	// for root documentation of NormalEqn see https://root.cern/doc/master/TDecompChol_8cxx.html
	if(method==1){
		// Method 1. solve through 'Normal Equations'
		if(datapointweights==0){
#ifdef TRACKFINDVERBOSE
			std::cout<<"solving with method 1, no weightings"<<std::endl;
#endif
			const TVectorD c_norm = NormalEqn(A,y);
			fit_offset=c_norm(0);
			fit_gradient=c_norm(1);
	//		return;
		} else {
#ifdef TRACKFINDVERBOSE
			std::cout<<"solving with method 2, with weightings"<<std::endl;
#endif
			// weightings are passed by providing the 'e' vector to NormalEqn.
			const TVectorD c_norm = NormalEqn(A,y,e);
			fit_offset=c_norm(0);
			fit_gradient=c_norm(1);
	//		return;
		}
	}
	
	if(method==2){
		// Method 2. solve through single-value-decomposition (SVD) (numerically  preferred method)
		if(datapointweights==0){
#ifdef TRACKFINDVERBOSE
			std::cout<<"solving with method 2, no weightings"<<std::endl;
#endif
			// no weightings, proceed with unweighted datapoints
			TDecompSVD svd(A);
			Bool_t ok;
			const TVectorD c_svd = svd.Solve(y,ok);
#ifdef TRACKFINDVERBOSE
			std::cout<<"TDecompSVD returned ok="<<ok<<std::endl;
#endif
			fit_offset=c_svd(0);
			fit_gradient=c_svd(1);
	//		return;
		} else {
#ifdef TRACKFINDVERBOSE
			std::cout<<"solving with method 2, with weightings"<<std::endl;
#endif
			// weightings - fill weighted matrix/vectors
			TMatrixD Aw = A;
			TVectorD yw = y;
			for (Int_t irow = 0; irow < A.GetNrows(); irow++) {
			TMatrixDRow(Aw,irow) *= 1/e(irow);
			yw(irow) /= e(irow);
			}
			// solve with the weighted datapoints
			TDecompSVD svd(Aw);
			Bool_t ok;
			const TVectorD c_svd = svd.Solve(yw,ok);
#ifdef TRACKFINDVERBOSE
			std::cout<<"TDecompSVD returned ok="<<ok<<std::endl;
#endif
			fit_offset=c_svd(0);
			fit_gradient=c_svd(1);
	//		return;
		}
	}
	
	chi2=0.;
	for(int i=0; i<numdatapoints; i++){
		Double_t fitpred = (fit_gradient*datapointxs[i]) + fit_offset;
#ifdef TRACKFINDVERBOSE
		std::cout<<"predicted datapoint "<<i<<" = "<<fitpred
			<<", actual datapoint = "<<datapointys[i]<<std::endl;
#endif
		// calculate the error on the position from the paddle extents. Since the cluster may span
		// several paddles, we have to calculate this on a per-cluster basis
		chi2 += pow((datapointys[i]-fitpred),2)/errorys[i];
	}
	
	return;
}

bool cMRDSubEvent::SearchForClusterInTracks(const std::vector<std::pair<int,int> > &matchedtracks, const std::vector<std::vector<mrdcell*> > &allpaddletracks, const std::vector<mrdcell*> tracktotest, const std::string horv){
#ifdef TRACKFINDVERBOSE
	std::cout<<"searching for clusters in ";
	(horv=="h") ? std::cout<<"h" : std::cout<<"v";
	std::cout<<" track "<<std::endl;
#endif
	bool hassharedcluster=false;
	std::map<mrdcluster*,int> clustermap;
	for(auto&& acell : tracktotest) clustermap.emplace(acell->clusters.first,1);
	clustermap.emplace(tracktotest.back()->clusters.second,1);
#ifdef TRACKFINDVERBOSE
	std::cout<<"track to compare has "<<clustermap.size()<<" clusters, looking for these clusters in "
		<<matchedtracks.size()<<" existing tracks"<<std::endl;
#endif
	
	for(auto apair : matchedtracks){                                   // loop over existing pairs
		std::vector<mrdcell*> atrack;                                  // 
		if(horv=="h") atrack = allpaddletracks.at(apair.first );       // pull the h track part
		else          atrack = allpaddletracks.at(apair.second);       // pull the v track part
		for(auto&& acell : atrack){                                    // loop over cells in h track
			mrdcluster* fexcluster = acell->clusters.first;            // get the first cluster of the cell
			if(clustermap.count(fexcluster)!=0){                       // test for sharing
				hassharedcluster=true;
				break;
			}
		}
		// for just the last cell in the track, also test the second cluster
		if(!hassharedcluster)
			if(clustermap.count(atrack.back()->clusters.second)!=0) hassharedcluster=true;
		if(hassharedcluster) break;
	}
	return hassharedcluster;
}

bool cMRDSubEvent::BridgeSearch(const std::vector<mrdcell*> &tracktotest, const std::vector<std::pair<int,int> > &matchedtracks, const std::vector<std::vector<mrdcell*> > &allpaddletracks, const std::string horv){
	// test if a single-cell track pair candidate is joining two existing tracks in either of it's views
	bool badmatch=false;
	mrdcluster* trackstartcluster = tracktotest.front()->clusters.second;
	mrdcluster* trackstopcluster  = tracktotest.back()->clusters.first;
	
	bool startclusterassigned=false, stopclusterassigned=false;
	
	// scan over all matched tracks so far
	for(int tracki=0; tracki<matchedtracks.size(); tracki++){
		// get the track (vector of cells)
		std::vector<mrdcell*> apaddletrack;
		if(horv=="h") apaddletrack = allpaddletracks.at(matchedtracks.at(tracki).first);
		else          apaddletrack = allpaddletracks.at(matchedtracks.at(tracki).second);
		
		// scan over all the clusters in this track to see if it already 
		// contains the new candidate's clusters
		for(auto&& acell : apaddletrack){
			mrdcluster* uscluster = acell->clusters.first;
			mrdcluster* dscluster = acell->clusters.second;
			if(trackstartcluster==uscluster||trackstartcluster==dscluster)
				startclusterassigned=true;
			if(trackstopcluster==uscluster||trackstopcluster==dscluster)
				stopclusterassigned=true;
		}
	}
	
	if(startclusterassigned&&stopclusterassigned)
		badmatch=true; // don't create a new track from cells bridging already assigned clusters
	return badmatch;
}
