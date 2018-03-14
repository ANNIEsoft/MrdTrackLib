/* vim:set noexpandtab tabstop=4 wrap */
//#include "MRDspecs.cpp" // for standalone test only
#include "MRDspecs.hh"
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cassert>
//#include <iomanip>
//#include <cstdlib>  // for getenv for standalone call

//.L thisfile+g; StripMrdPositions()    << standalone call

//// FIXME: RESULTS RETURNED FROM STRIPMRDPOSITIONS ARE IN MM!
//// EVERYTHING ELSE HERE IS IN CM. THIS IS CONFUSING.

// disable for standalone?
// declare the output vectors to put things in temporarily
static std::vector<int> temp_paddle_orientations(MRDSpecs::nummrdpmts);	// H is 0, V is 1
static std::vector<int> temp_paddle_layers(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originx(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originy(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originz(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsx(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsy(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsz(MRDSpecs::nummrdpmts);

int StripMrdPositions(){

	// input file reading infrastructure
	//std::ifstream input_file(fname);
	/* version that reads from file. Needs MRD_positions_raw to be in the same directory as executed.
	std::ifstream input_file(rawfilename);
		if ( input_file.fail() ) {
		std::cerr << "Failed to open file " << rawfilename << "!" << std::endl;
		exit(-1);
	}
	char linebuffer[256];
	std::string fileline;
	std::vector<std::string> filelines;
	
	// read the file and put the lines into a vector of std::strings
	while ( !input_file.eof() ) {
		input_file.getline(linebuffer,256);
		//istringstream streamconverter(linebuffer);
		//streamconverter >> fileline;	// pulls out each element by space delimitation
		fileline = std::string(linebuffer);
		filelines.push_back(fileline);
	}
	*/
	
	/* instead use string literal file to load into char[], then split this by newline tokens*/
	std::vector<std::string> filelines;
	const char* rawpositions = 
	#include "MRD_positions_raw.dat"
	;
	
	// use strtok to split char array into lines by searching for \n's. has problems?
//	char *nextline = strtok(rawpositions, "\n");
//	while(nextline) {                         // while token was found
//		std::string nextlinestring(nextline);
//		filelines.push_back(nextlinestring);
//		nextline = strtok(NULL, "\n");
//	}
	// maybe convert the whole char array into a string and use std::getline method
	std::string fileasstring(rawpositions); // ,length); // length optional, but needed if there may be zero's in your data
	std::istringstream fileasistream(fileasstring);
	std::string nextline;
	while(std::getline(fileasistream,nextline)){
		filelines.push_back(nextline);
	}
	
	// lines are of the format:
	// PMT 277 : Orientation V : Layer 9 : Origin (1216.5,-652.5,503.25) : Extent (1116.5→1316.5, -1388.5→83.5, 4450.7→4456.7)
	std::string theexpressionstring = "PMT %d : Orientation %c : Layer %d : Origin (%lf,%lf,%lf) : Extent (%lf→%lf, %lf→%lf, %lf→%lf)";
//	std::cout<<"pattern to match is "<<theexpressionstring<<std::endl;
	
//	// enable for standalone tests
//	std::vector<int> temp_paddle_orientations(MRDSpecs::nummrdpmts);	// H is 0, V is 1
//	std::vector<int> temp_paddle_layers(MRDSpecs::nummrdpmts);
//	std::vector<double> temp_paddle_originx(MRDSpecs::nummrdpmts);
//	std::vector<double> temp_paddle_originy(MRDSpecs::nummrdpmts);
//	std::vector<double> temp_paddle_originz(MRDSpecs::nummrdpmts);
//	std::vector<std::pair<double,double> > temp_paddle_extentsx(MRDSpecs::nummrdpmts);
//	std::vector<std::pair<double,double> > temp_paddle_extentsy(MRDSpecs::nummrdpmts);
//	std::vector<std::pair<double,double> > temp_paddle_extentsz(MRDSpecs::nummrdpmts);
	
	// use sscanf to extract the information
	int numlayers=0;
	for(int linenum=0; linenum<filelines.size(); linenum++){
		std::string stringtomatch = filelines.at(linenum);
		//std::cout<<"next string is "<<stringtomatch<<std::endl;
		if(stringtomatch=="") break;
		
		int pmt_id=-1;
		char next_paddle_orientation='?';
		int next_paddle_layer=-1;
		double next_paddle_originx=-1;
		double next_paddle_originy=-1;
		double next_paddle_originz=-1;
		double next_paddle_extentsx1=-1, next_paddle_extentsx2=-1;
		double next_paddle_extentsy1=-1, next_paddle_extentsy2=-1;
		double next_paddle_extentsz1=-1, next_paddle_extentsz2=-1;
		
		//std::cout<<"matching "<<stringtomatch.c_str()<<" with expression "<<theexpressionstring.c_str()<<" to look for 12 parameters"<<std::endl;
		
		int nmatched = sscanf(stringtomatch.c_str(), theexpressionstring.c_str(), &pmt_id, &next_paddle_orientation, &next_paddle_layer, &next_paddle_originx, &next_paddle_originy, &next_paddle_originz, &next_paddle_extentsx1, &next_paddle_extentsx2, &next_paddle_extentsy1, &next_paddle_extentsy2, &next_paddle_extentsz1, &next_paddle_extentsz2);
		
		//std::cout<<"match found "<<nmatched<<" parameters"<<std::endl;
		if(nmatched!=12){ // not all values were matched
			std::cerr<<"Match failure!"<<std::endl;
			assert("MRDspecs_StripMrdPositions.cpp match failure! Aborting!"&&false);
		} else {
			//std::cout<<"setting stats for pmt "<<pmt_id<<std::endl;
			std::string orientationstring(&next_paddle_orientation);
			(strcmp(&next_paddle_orientation,"H")==0) ? temp_paddle_orientations.at(pmt_id)=0 : temp_paddle_orientations.at(pmt_id)=1;
			temp_paddle_layers.at(pmt_id)=next_paddle_layer;
			temp_paddle_originx.at(pmt_id)=next_paddle_originx;
			temp_paddle_originy.at(pmt_id)=next_paddle_originy;
			temp_paddle_originz.at(pmt_id)=next_paddle_originz;
			temp_paddle_extentsx.at(pmt_id)=std::pair<double, double>(next_paddle_extentsx1,next_paddle_extentsx2);
			temp_paddle_extentsy.at(pmt_id)=std::pair<double, double>(next_paddle_extentsy1,next_paddle_extentsy2);
			temp_paddle_extentsz.at(pmt_id)=std::pair<double, double>(next_paddle_extentsz1,next_paddle_extentsz2);
			
//			std::cout<<"matched parameters were: "<<std::endl <<
//					pmt_id << std::endl <<
//					next_paddle_orientation << std::endl <<
//					next_paddle_layer << std::endl <<
//					next_paddle_originx << std::endl <<
//					next_paddle_originy << std::endl <<
//					next_paddle_originz << std::endl <<
//					next_paddle_extentsx1 << std::endl <<
//					next_paddle_extentsx2 << std::endl <<
//					next_paddle_extentsy1 << std::endl <<
//					next_paddle_extentsy2 << std::endl <<
//					next_paddle_extentsz1 << std::endl <<
//					next_paddle_extentsz2 << std::endl <<
//					std::endl;
		}
	}
	// success should return for each match:
	//submatch 1 is 0
	//submatch 2 is H
	//submatch 3 is 0
	//submatch 4 is 737.5
	//submatch 5 is -1219.5
	//submatch 6 is -586.65
	//submatch 7 is 1.5
	//submatch 8 is 1473.5
	//submatch 9 is -1319.5
	//submatch 10 is -1119.5
	//submatch 11 is 2665.35
	//submatch 12 is 2671.35
	
//	std::ofstream mrdpositions;
//	mrdpositions.open("mrdpositions.txt", std::ios::out);
//	mrdpositions<<"pmtids is "<<pmt_ids<<std::endl;
//	mrdpositions<<"orientations is "<<orientations<<std::endl;
//	mrdpositions<<"layers is "<<layers<<std::endl;
//	mrdpositions<<"originxs is "<<originx<<std::endl;
//	mrdpositions<<"originys is "<<originy<<std::endl;
//	mrdpositions<<"originzs is "<<originz<<std::endl;
//	mrdpositions<<"extentsx is "<<extentsx<<std::endl;
//	mrdpositions<<"extentsy is "<<extentsy<<std::endl;
//	mrdpositions<<"extentsz is "<<extentsz<<std::endl;
//	mrdpositions.close();
	
//	for(int i=0; i<temp_paddle_orientations.size(); i++){
//		std::cout<<"orientations is "<<temp_paddle_orientations.at(i)<<std::endl;
//		std::cout<<"layers is "<<temp_paddle_layers.at(i)<<std::endl;
//		std::cout<<"originxs is "<<temp_paddle_originx.at(i)<<std::endl;
//		std::cout<<"originys is "<<temp_paddle_originy.at(i)<<std::endl;
//		std::cout<<"originzs is "<<temp_paddle_originz.at(i)<<std::endl;
//		std::cout<<"extentsx is "<<temp_paddle_extentsx.at(i).first<<", "<<temp_paddle_extentsx.at(i).second<<std::endl;
//		std::cout<<"extentsy is "<<temp_paddle_extentsy.at(i).first<<", "<<temp_paddle_extentsy.at(i).second<<std::endl;
//		std::cout<<"extentsz is "<<temp_paddle_extentsz.at(i).first<<", "<<temp_paddle_extentsz.at(i).second<<std::endl;
//	}
	
	//assert("done with MRDspectest, exiting"&&false);
	
	return 1;
}

// disable remainder for standalone tests
// ----------------------
// call the function to set the temporary variables.
int nothing = StripMrdPositions();

// initialise the const namespace variables with the temporary ones.
const std::vector<int> &MRDSpecs::paddle_orientations = temp_paddle_orientations;
const std::vector<int> &MRDSpecs::paddle_layers = temp_paddle_layers;
const std::vector<double> &MRDSpecs::paddle_originx = temp_paddle_originx;
const std::vector<double> &MRDSpecs::paddle_originy = temp_paddle_originy;
const std::vector<double> &MRDSpecs::paddle_originz = temp_paddle_originz;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsx = temp_paddle_extentsx;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsy = temp_paddle_extentsy;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsz = temp_paddle_extentsz;


