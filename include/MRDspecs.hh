/* vim:set noexpandtab tabstop=4 wrap */
#ifndef _MRD_SPECS_
#define _MRD_SPECS_

#include <utility>
#include <vector>
//#include <array>

// first panel is a HORIZONTAL LAYER.
// TODO store these in the WCSimRootGeom?

namespace MRDSpecs{
	extern const int numpaddlesperpanelv;
	extern const int numpaddlesperpanelh;
	extern const int numhpanels;
	extern const int numvpanels;
	extern const int numpanels;
	extern const int numplates;

	extern const double scintfullxlen;
	extern const double scintfullzlen;
	extern const double scinthfullylen;
	extern const double scintvfullylen;

	extern const int numalustructs;
	extern const int numvetopaddles;
	extern const int vetopaddlesperpanel;

	extern const double steelfullxlen;
	extern const double steelfullylen;
	extern const double steelfullzlen;

	extern const double scinttapfullwidth;		// width of the scintilling taper at the narrow end
	extern const double scinttapfullheight;		// z length of tapering part of scint paddles.

	extern const double scintlgfullwidth;		// tapered light guides at narrow end
	extern const double scintlgfullheight; 

	extern const double alufullxlen;
	extern const double alufullylen;
	extern const double alufullzlen;		// TODO: check this with measurement
	extern const double alufullxthickness;		// TODO: check this with measurement
	extern const double alufullythickness;		// TODO:  " "
	extern const double windowwidth;
	extern const double windowheight;

	extern const double scintbordergap;		// gap between each scintillator (cm) to account for cladding etc
	extern const double steelscintgap;		// gap between steel and scintillator
	extern const double scintalugap;		// gap between scintillator and alu struct
	extern const double alusteelgap;		// gap between alu struct and subsequent steel of next layer
	extern const double layergap;			// total gaps between layers
	extern const double nothickness;

	extern const double mrdZlen;			// TODO reconcile this with MRD_depth

	extern const double tankouterRadius;

	// calculate full extent of MRD for it's mother volume / the canvas scaling factor.
	extern const double MRDPMTExposeHeight;
	extern const double MRDPMTRadius;
	extern const double mrdpmtfullheight;
	extern const double  widths[];
	extern const double  heights[];
	extern const double maxwidth;
	extern const double maxheight;

	extern const int nummrdpmts;
	extern const std::vector<int> layeroffsets;

	extern const float MRD_width;
	extern const float MRD_height;
	extern const float MRD_layer2;			// position in wcsim coords of second scint layer in cm
	extern const float MRD_start;			// position in wcsim coord of MRD front face in cm
	extern const float MRD_depth;			// total depth of the MRD in cm
	extern const float MRD_end;			// end of the MRD in cm
	extern const float MRD_steel_width;		// half width of steel in cm
	extern const float MRD_steel_height;		// half height of steel in cm

	extern const std::vector<double> mrdscintlayers;

	//TODO: should retrieve this info from the geo in wcsimanalysis class
	extern const float tank_start;			// front face of the tank in cm
	extern const float tank_radius;			// tank radius in cm
	extern const float tank_halfheight;		// tank half height in cm
	extern const float tank_yoffset;		// tank y offset in cm

	const float fidcutradius=12.56;			// radius of fid vol in cm
	const float fidcuty=50.;				// y extent of fid vol in cm - centered on tank origin
	const float fidcutz=0;					// z cut of fid vol - require intx in upstream half

	// information about the paddles so they can be looked up from the tube ID
	extern const std::vector<int> &paddle_orientations;		// H is 0, V is 1
	extern const std::vector<int> &paddle_layers;
	extern const std::vector<double> &paddle_originx;
	extern const std::vector<double> &paddle_originy;
	extern const std::vector<double> &paddle_originz;
	extern const std::vector<std::pair<double,double> > &paddle_extentsx;
	extern const std::vector<std::pair<double,double> > &paddle_extentsy;
	extern const std::vector<std::pair<double,double> > &paddle_extentsz;

}

#endif 
