/***************************************************************************
                          MSRouteLoaderControl.cpp
    Class responsible for loading of routes from some files
                             -------------------
    begin                : Wed, 06 Nov 2002
    copyright            : (C) 2001 by IVF/DLR (http://ivf.dlr.de)
    author               : Daniel Krajzewicz
    email                : Daniel.Krajzewicz@dlr.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.8  2005/10/07 11:37:45  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.7  2005/09/15 11:10:46  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.6  2005/05/04 08:32:05  dkrajzew
// level 3 warnings removed; a certain SUMOTime time description added
//
// Revision 1.5  2005/02/01 10:10:42  dkrajzew
// got rid of MSNet::Time
//
// Revision 1.4  2004/11/23 10:20:10  dkrajzew
// new detectors and tls usage applied; debugging
//
// Revision 1.3  2003/03/17 14:15:55  dkrajzew
// first steps of network reinitialisation implemented
//
// Revision 1.2  2003/03/03 14:56:25  dkrajzew
// some debugging; new detector types added; actuated traffic lights added
//
// Revision 1.1  2003/02/07 10:41:50  dkrajzew
// updated
//
/* =========================================================================
 * compiler pragmas
 * ======================================================================= */
#pragma warning(disable: 4786)


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif
#endif // HAVE_CONFIG_H

#include <vector>
#include "MSRouteLoader.h"
#include "MSRouteLoaderControl.h"

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


/* =========================================================================
 * method definitions
 * ======================================================================= */
MSRouteLoaderControl::MSRouteLoaderControl(MSNet &net,
                                           int inAdvanceStepNo,
                                           LoaderVector loader)
    : myLastLoadTime(-inAdvanceStepNo),
    myInAdvanceStepNo(inAdvanceStepNo),
    myRouteLoaders(loader),
    myVehCont(inAdvanceStepNo>0 ? inAdvanceStepNo : 10), // !!! einen schalueren Wert!
    myAllLoaded(false)
{
    myLoadAll = myInAdvanceStepNo<=0;
    myAllLoaded = false;
    myLastLoadTime = -1 * (int) myInAdvanceStepNo;
    // initialize all used loaders
    for( LoaderVector::iterator i=myRouteLoaders.begin();
         i!=myRouteLoaders.end(); i++) {
        (*i)->init();
    }
}


MSRouteLoaderControl::~MSRouteLoaderControl()
{
    for( LoaderVector::iterator i=myRouteLoaders.begin();
         i!=myRouteLoaders.end(); i++) {
        delete (*i);
    }
}


MSVehicleContainer &
MSRouteLoaderControl::loadNext(SUMOTime step)
{
    // check whether new vehicles shall be loaded
    //  return if not
    if( (myLoadAll&&myAllLoaded) || (myLastLoadTime>=0&&myLastLoadTime/*+myInAdvanceStepNo*/>=step) ) {
        return myVehCont;
    }
    // load all routes for the specified time period
    size_t run = step;
    bool furtherAvailable = true;
    for(;
         furtherAvailable &&
             (myLoadAll||run<=step+myInAdvanceStepNo);
         run++) {
        furtherAvailable = false;
        for( LoaderVector::iterator i=myRouteLoaders.begin();
             i!=myRouteLoaders.end(); i++) {
            if((*i)->moreAvailable()) {
                (*i)->loadUntil(run, myVehCont);
            }
            furtherAvailable |= (*i)->moreAvailable();
        }
    }
    // no further loading when all was loaded
    if(myLoadAll||!furtherAvailable) {
        myAllLoaded = true;
    }
    // set the step information
    myLastLoadTime = run - 1;
    // return the container with new vehicles
    return myVehCont;
}



/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:
