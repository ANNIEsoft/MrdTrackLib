# $Id: GNUmakefile,v 1.17 2006/09/04 15:43:27 t2k Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

all: libMrdTrack.so

libMrdTrack.so : 
	@echo Compiling libMrdTrack.so with rootcling ...
	@root -l -b -q $(PWD)/makelib.CC

clean :
	@rm -f libMakeMrdTrackClasses_cxx.so
	@rm -f libMakeMrdTrackClasses_cxx_ACLiC_dict_rdict.pcm
