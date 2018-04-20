/* vim:set noexpandtab tabstop=4 wrap */
#include "MRDspecs.hh"
#include <algorithm>
#include <array>

// first panel is a HORIZONTAL LAYER.
// TODO store these in the WCSimRootGeom?

namespace MRDSpecs{

	const int numpaddlesperpanelv=30;
	const int numpaddlesperpanelh=26;
	const int numhpanels=6;
	const int numvpanels=5;
	const int numpanels=numhpanels+numvpanels;
	const int numplates=12;

	const double scintfullxlen = 20;
	const double scintfullzlen = 0.6;
	const double scinthfullylen = 147.2;
	const double scintvfullylen = 130.2;

	const int numalustructs=11;
	const int numvetopaddles=26;
	const int vetopaddlesperpanel=13;

	const double steelfullxlen = 305;
	const double steelfullylen = 274;
	const double steelfullzlen = 5;

	const double scinttapfullwidth = 17.1;
	const double scinttapfullheight = 7.8;

	const double scintlgfullwidth = 5.08;
	const double scintlgfullheight = 33.3;

	const double alufullxlen = steelfullxlen+15;
	const double alufullylen = steelfullylen+15;
	const double alufullzlen = 3.81;
	const double alufullxthickness = 2.54;
	const double alufullythickness = 2.54;
	const double windowwidth = (steelfullxlen-(4*alufullxthickness))/3;
	const double windowheight= (steelfullylen-(4*alufullythickness))/3;

	const double scintbordergap=0.3;
	const double steelscintgap=0.5;
	const double scintalugap=0.2;
	const double alusteelgap=2.0;
	const double layergap = steelscintgap + scintalugap + alusteelgap;
	const double nothickness = 0.01;

	const double mrdZlen = numplates*steelfullzlen + (numpanels+1)*scintfullzlen + numalustructs*alufullzlen + numpanels*layergap + scintalugap;

	const double tankouterRadius= 152.4;

	// calculate full extent of MRD for it's mother volume / the canvas scaling factor.
	const double MRDPMTExposeHeight=0;
	const double MRDPMTRadius=5.08;
	const double mrdpmtfullheight = MRDPMTExposeHeight;
	const double  widths[] = {2*(scinthfullylen+scinttapfullheight+scintlgfullheight+(scintbordergap/2)+mrdpmtfullheight+nothickness),((numpaddlesperpanelv/2)*(scintfullxlen+scintbordergap))};
	const double  heights[] = {2*(scintvfullylen+scinttapfullheight+scintlgfullheight+(scintbordergap/2)+mrdpmtfullheight+nothickness),((numpaddlesperpanelh/2)*(scintfullxlen+scintbordergap))};
	const double maxwidth = *std::max_element(widths,widths+(sizeof(widths)/sizeof(widths[0])))+0.1;
	const double maxheight = *std::max_element(heights,heights+(sizeof(heights)/sizeof(heights[0])))+0.1;

	const int nummrdpmts=306;
	const std::vector<int> layeroffsets = {0, 26, 56, 82, 112, 138, 168, 194, 224, 250, 280, 306};
	// ids of the first pmt in each layer.                                       KEEP an extra ^^^

	const float MRD_width = ((numpaddlesperpanelv/2)*(scintfullxlen+scintbordergap))/2.;
	const float MRD_height = ((numpaddlesperpanelh/2)*(scintfullxlen+scintbordergap))/2.;
	const float MRD_layer2 = 290.755;
	const float MRD_start = 325.5;
	const float MRD_depth = 139.09;
	const float MRD_end = 464.59;
	const float MRD_steel_width = (305./2.);
	const float MRD_steel_height = (274./2.);

	/* output from WCSim:
	########## MRD front face: 325.5                     ##########
	########## MRD total Z length: 139.09                ##########
	########## MRD scintillator layer 0  (H) at z=336.08 ##########     1
	########## MRD scintillator layer 1  (V) at z=348.19 ########## 1
	########## MRD scintillator layer 2  (H) at z=360.30 ##########     2
	########## MRD scintillator layer 3  (V) at z=372.41 ########## 2
	########## MRD scintillator layer 4  (H) at z=384.52 ##########     3
	########## MRD scintillator layer 5  (V) at z=396.63 ########## 3
	########## MRD scintillator layer 6  (H) at z=408.74 ##########     4
	########## MRD scintillator layer 7  (V) at z=420.85 ########## 4
	########## MRD scintillator layer 8  (H) at z=432.96 ##########     5
	########## MRD scintillator layer 9  (V) at z=445.07 ########## 5
	########## MRD scintillator layer 10 (H) at z=457.18 ##########
	*/

	const std::vector<double> mrdscintlayers{336.080, 348.190, 360.300, 372.410, 384.520, 396.630, 408.740, 420.850, 432.960, 445.070, 457.180 };

	//TODO: should retrieve this info from the geo in wcsimanalysis class
	const float tank_start = 15.70;
	const float tank_radius = 152.4;
	const float tank_halfheight = 198.;
	const float tank_yoffset = -14.46;

	const float fidcutradius=12.56;             // 80% of full tank radius
	const float fidcuty=50.;                    // a meter total fiducial volume in the centre y
	const float fidcutz=0;                      // fidcuial volume is before the tank centre.

	//totMRD_box = new G4Box("totMRD",(maxwidth/2),(maxheight/2),mrdZlen/2);

}
