#ifndef NIVissimNodeDef_Poly_h
#define NIVissimNodeDef_Poly_h
//---------------------------------------------------------------------------//
//                        NIVissimNodeDef_Poly.h -  ccc
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Sept 2002
//  copyright            : (C) 2002 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
// $Log$
// Revision 1.9  2005/10/07 11:40:10  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.8  2005/09/23 06:02:57  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.7  2005/04/27 12:24:37  dkrajzew
// level3 warnings removed; made netbuild-containers non-static
//
// Revision 1.6  2003/06/18 11:35:29  dkrajzew
// message subsystem changes applied and some further work done; seems to be stable but is not perfect, yet
//
// Revision 1.5  2003/06/05 11:46:57  dkrajzew
// class templates applied; documentation added
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


#include <string>
#include <map>
#include <utils/geom/Position2DVector.h>
#include "NIVissimNodeDef_Edges.h"

class NIVissimNodeDef_Poly :
        public NIVissimNodeDef_Edges {
public:
    NIVissimNodeDef_Poly(int id, const std::string &name,
        const Position2DVector &poly);
    virtual ~NIVissimNodeDef_Poly();
    static bool dictionary(int id, const std::string &name,
        const Position2DVector &poly);
//    virtual void computeBounding();
//    SUMOReal getEdgePosition(int edgeid) const;
    void searchAndSetConnections(SUMOReal offset);
private:
    Position2DVector myPoly;

};



/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

