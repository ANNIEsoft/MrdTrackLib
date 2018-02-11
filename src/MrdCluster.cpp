/* vim:set noexpandtab tabstop=4 wrap */

#include "MrdCluster.hh"
#include "MRDspecs.hh"
#include <algorithm>

int mrdcluster::clustercounter=0;   // initialisation of static counter

void mrdcluster::AddDigit(int digitindexin, int pmtidin, double timein){
	digitindexes.push_back(digitindexin);
	digittimes.push_back(timein);
	
	// calculate in-layer tube index - first subtract id of first tube in the layer
	pmtidin -= MRDSpecs::layeroffsets.at(layer);
	// correct for the fact there are two halves to each layer. 
	// tubes 0 and 1 are in opposite halves!!
	// if pmtidin%2==1, it is in the second half. Merge halves by subtracting 1, then divide by 2. 
	if(pmtidin%2==1){
		if(altviewhalf>0) altviewhalf=0; // added digit is in the other half to constructor digit
		pmtidin -= 1;
	} else {
		if(altviewhalf<0) altviewhalf=0;
	}
	pmtidin /= 2;
	
	if(pmtidin>(*std::max_element(in_layer_tubeids.begin(), in_layer_tubeids.end()))){
		// this PMT is 'higher' than the others - increase the xmax accordingly
		xmaxid = pmtidin;
	} else {
		// this PMT is lower - decrease the xmin accordingly
		xminid = pmtidin;
	}
	
	in_layer_tubeids.push_back(pmtidin);
}

// return an *effective in-layer tube index* of the cluster centre.
double mrdcluster::GetCentreIndex(){
	if(xmaxid==xminid){
		//cout<<"xmaxid==xminid, centreindex is "<<xmaxid<<endl;
		return xmaxid;
	} else {
		//cout<<"xmaxid!=xminid, centreindex is "<<((xmaxid+xminid)/2.)<<endl;
		return ((xmaxid+xminid)/2.);
	}
}

// return an actual cm in-layer position
double mrdcluster::GetXmax(){
	//cout<<"GetXmax for cluster "<<clusterid<<endl;
	if (layer%2==0){	// h layer: short paddle extents are those in extentsy
		//cout<<"layer is a h layer, xmaxid is "<<xmaxid
		//	<<", getting upper paddle extent of pmt id "<<(2*xmaxid) + layeroffsets.at(layer)
		//	<<" = "<<MRDSpecs::paddle_extentsy.at((2*xmaxid) + layeroffsets.at(layer)).second<<endl;
		return MRDSpecs::paddle_extentsy.at((2*xmaxid) + MRDSpecs::layeroffsets.at(layer)).second;
	} else {
		//cout<<"layer is a v layer, xmaxid is "<<xmaxid
		//	<<", getting upper paddle extent of pmt id "<<(2*xmaxid) + layeroffsets.at(layer)
		//	<<" = "<<MRDSpecs::paddle_extentsx.at((2*xmaxid) + layeroffsets.at(layer)).second<<endl;
		return MRDSpecs::paddle_extentsx.at((2*xmaxid) + MRDSpecs::layeroffsets.at(layer)).second;
	}
}

double mrdcluster::GetXmin(){
	//cout<<"GetXmin for cluster "<<clusterid<<endl;
	if (layer%2==0){
		//cout<<"layer is a h layer, xminid is "<<xminid
		//	<<", getting lower paddle extent of pmt id "<<(2*xminid) + layeroffsets.at(layer)
		//	<<" = "<<MRDSpecs::paddle_extentsy.at((2*xminid) + layeroffsets.at(layer)).first<<endl;
		return MRDSpecs::paddle_extentsy.at((2*xminid) + MRDSpecs::layeroffsets.at(layer)).first;
	} else {
		//cout<<"layer is a v layer, xminid is "<<xminid
		//	<<", getting lower paddle extent of pmt id "<<(2*xminid) + layeroffsets.at(layer)
		//	<<" = "<<MRDSpecs::paddle_extentsx.at((2*xminid) + layeroffsets.at(layer)).first<<endl;
		return MRDSpecs::paddle_extentsx.at((2*xminid) + MRDSpecs::layeroffsets.at(layer)).first;
	}
}

double mrdcluster::GetCentre(){
	return ((this->GetXmax()+this->GetXmin())/2.);
}

double mrdcluster::GetTime(){
	return digittimes.at(0);
}

// constructor
mrdcluster::mrdcluster(int digitindexin, int pmtidin, int layerin, double timein){
	digitindexes.push_back(digitindexin);
	layer=layerin;
	clusterid=clustercounter;
	clustercounter++;
	digittimes.push_back(timein);
	//cout<<"generating cluster with tube "<<pmtidin<<endl;
	
	// calculate pmt number within this layer
	pmtidin -= MRDSpecs::layeroffsets.at(layer);
	// correct for the fact there are two halves to each layer. 
	// tubes 0 and 1 are in opposite halves!!
	// if pmtidin%2==1, it is in the second half. Merge halves by subtracting 1. 
	if(pmtidin%2==1){ altviewhalf=-1; pmtidin -= 1; }
	else { altviewhalf=1; }
	pmtidin /= 2;
	//cout<<"in-layer tube id is "<<pmtidin<<endl;
	
	in_layer_tubeids.push_back(pmtidin);
	
	// as the constructor takes the first digit, this is both the max and min hit tube
	xmaxid=pmtidin;
	xminid=pmtidin;
	
}

// default constructor 
mrdcluster::mrdcluster(){}

// destructor
mrdcluster::~mrdcluster(){}

