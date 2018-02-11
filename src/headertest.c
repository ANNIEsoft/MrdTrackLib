/* vim:set noexpandtab tabstop=4 wrap */
//#include "MRDspecs.hh"               // OK
//#include "MrdCluster.hh"             // OK
//#include "MrdCell.hh"                // OK
//#include "MRDTrackClass.hh"          // OK
#include "MRDSubEventClass.hh"       // OK
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

int main(){
	//mrdcell* acell = new mrdcell();  OK
	TFile* f = new TFile("testout.root","RECREATE");
	TTree* t = new TTree("treeout","test_tree");
	TClonesArray* subevarray = new TClonesArray("cMRDSubEvent");   // string is class name
	t->Branch("subeventsinthisevent",&subevarray);
	cMRDSubEvent* asubev = new((*subevarray)[0]) cMRDSubEvent();
	cMRDSubEvent* asubev2 = new((*subevarray)[1]) cMRDSubEvent();
	t->Fill();
	f->Write();
	f->Close();  // also calls 'delete f'
	
	return 0;
}
