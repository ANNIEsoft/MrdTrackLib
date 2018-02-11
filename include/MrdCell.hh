/* vim:set noexpandtab tabstop=4 wrap */
#ifndef MrdCell_hh
#define MrdCell_hh

#include <utility>  // std::pair
#include <vector>

class mrdcluster;

// 2. define a cell as an object containing an ID, a line-segment (pair of clusters), an integer state (initialised to 0), and a vector of neighbour cell ids.
class mrdcell {
	public:
	static int cellcounter;
	int cellid;
	std::pair<mrdcluster*, mrdcluster*> clusters; //! used in building the cell
	bool isdownstreamgoing;	// is the track going upstream instead of downstream
	int status;
	int utneighbourcellindex;	// the up-track neighbour of this cell (*)
	int dtneighbourcellindex;	// the down-track neighbour of this cell (*)
								// a down-track cell index of -2 indicates more than one
	int parentcellindex;		// if the up-track cell has more than one downtrack neighbour, the track splits (*)
								// so it's upstream neighbour becomes it's parent
	bool hasdaughters;			// so we can keep short tracks that have daughters (*)
	double neighbourchi2;		// if we have more than one candidate, use the most aligned neighbour (*)
								// (*) denotes fields that are only useful during reconstruction
	
	void IncrementStatus();
	void SetClusterAddresses(std::vector<mrdcluster> &trackclusters);
	
	mrdcell(mrdcluster* startcluster, mrdcluster* endcluster);
	
	// 'copy' constructor (also takes a new cell id)
	mrdcell(const mrdcell& cellin, int cellidin);
	
	// default constructor
	mrdcell();
	
	// destructor
	~mrdcell();
};

#endif
