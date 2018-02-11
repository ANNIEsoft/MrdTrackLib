/* vim:set noexpandtab tabstop=4 wrap */
#ifndef MrdCluster_hh
#define MrdCluster_hh

#include <vector>

// supporting classes used in DoReconstruction cellular algorithm version
// 1. define a cluster, by an id and it's position
class mrdcluster {
	public:
	static int clustercounter;
	int clusterid;
	int xmaxid;
	int xminid; 
	int layer;
	int altviewhalf;
	std::vector<int> in_layer_tubeids;
	std::vector<double> digittimes;
	std::vector<int> digitindexes;  //index of the digit within the WCSimRootTrigger
	void AddDigit(int digitindexin, int pmtidin, double timein);
	// return an *effective in-layer tube index* of the cluster centre.
	double GetCentreIndex();
	// return an actual cm in-layer position
	double GetXmax();
	double GetXmin();
	double GetCentre();
	double GetTime();
	
	// constructor
	mrdcluster(int digitindexin, int pmtidin, int layerin, double timein);
	
	// default constructor 
	mrdcluster();
	
	// destructor
	~mrdcluster();
	
};


//#ifdef __MAKECINT__
//#pragma link C++ class std::pair<mrdcluster*,mrdcluster*>+;
//#endif

#endif
