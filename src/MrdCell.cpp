/* vim:set noexpandtab tabstop=4 wrap */
#include "MrdCell.hh"
#include "MrdCluster.hh"

void mrdcell::IncrementStatus(){ status +=1; }

void mrdcell::SetCellID(int cellidin){ cellid=cellidin; } // needs to be index in track

void mrdcell::SetClusterAddresses(std::vector<mrdcluster> &trackclusters){
	if(clusters.first!=nullptr) return;
	//clusters=std::make_pair(&trackclusters.at(cellid), &trackclusters.at(cellid+1));  << do we have a clash
	clusters=std::make_pair(&trackclusters.at(cellid+1), &trackclusters.at(cellid));
}

void mrdcell::ClearClusterAddresses(){
	// presumably for a reason we don't over-write existing cluster addresses in SetClusterAddresses
	// but when we construct a track, we pass it an externally owned vector of clusters and cells,
	// and if the addresses in our cells are non-null, they point to the external clusters, not the
	// copies we now have ownership of. We need to update them immediately!
	clusters=std::pair<mrdcluster*, mrdcluster*>(nullptr,nullptr);
}

mrdcell::mrdcell(mrdcluster* startcluster, mrdcluster* endcluster) : status(0), utneighbourcellindex(-1), 
	dtneighbourcellindex(-1), parentcellindex(-1), hasdaughters(false), neighbourchi2(-1.) {
	clusters = std::make_pair(startcluster, endcluster);
	isdownstreamgoing = (clusters.first->GetTime() < clusters.second->GetTime());
	cellid=cellcounter;
	cellcounter++;
}

// 'copy' constructor (also takes a new cell id)
mrdcell::mrdcell(const mrdcell& cellin, int cellidin){
	cellid=cellidin;  // id within this track
	clusters = std::pair<mrdcluster*, mrdcluster*>(nullptr,nullptr);
	isdownstreamgoing=cellin.isdownstreamgoing;
	status=cellin.status;
	utneighbourcellindex=cellin.utneighbourcellindex;
	dtneighbourcellindex=cellin.dtneighbourcellindex;
	parentcellindex=cellin.parentcellindex;
	hasdaughters=cellin.hasdaughters;
	neighbourchi2=cellin.neighbourchi2;
}

// default constructor
mrdcell::mrdcell(){}

// destructor
mrdcell::~mrdcell(){}

// initialise static counter
int mrdcell::cellcounter=0;

