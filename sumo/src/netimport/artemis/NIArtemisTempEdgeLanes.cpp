//---------------------------------------------------------------------------//
//                        NIArtemisTempEdgeLanes.cpp -
//  A temporary storage for artims edge definitions
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
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.13  2005/10/07 11:39:05  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.12  2005/09/23 06:01:53  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.11  2005/09/15 12:03:37  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.10  2005/07/12 12:35:21  dkrajzew
// elmar2 importer included; debugging
//
// Revision 1.9  2005/04/27 12:24:25  dkrajzew
// level3 warnings removed; made netbuild-containers non-static
//
// Revision 1.8  2004/08/02 12:44:11  dkrajzew
// using Position2D instead of two SUMOReals
//
// Revision 1.7  2004/01/12 15:30:47  dkrajzew
// node-building classes are now lying in an own folder
//
// Revision 1.6  2003/07/07 08:26:33  dkrajzew
// adapted the importer to the new node type description
//
// Revision 1.5  2003/06/18 11:14:13  dkrajzew
// new message and error processing: output to user may be a message, warning or an error now; it is reported to a Singleton (MsgHandler); this handler puts it further to output instances. changes: no verbose-parameter needed; messages are exported to singleton
//
// Revision 1.4  2003/06/05 11:44:51  dkrajzew
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
#include <cassert>
#include <utils/common/DoubleVector.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/ToString.h>
#include <bitset>
#include <algorithm>
#include <netbuild/NBEdge.h>
#include <netbuild/NBEdgeCont.h>
#include <netbuild/nodes/NBNode.h>
#include <netbuild/nodes/NBNodeCont.h>
#include "NIArtemisTempEdgeLanes.h"
#include <utils/common/UtilExceptions.h>

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * static member definitions
 * ======================================================================= */
NIArtemisTempEdgeLanes::Link2LaneDesc NIArtemisTempEdgeLanes::myLinkLaneDescs;
NIArtemisTempEdgeLanes::Link2Positions NIArtemisTempEdgeLanes::myLinkLanePositions;


/* =========================================================================
 * method definitions
 * ======================================================================= */
NIArtemisTempEdgeLanes::LinkLaneDesc::LinkLaneDesc(int lane, int section,
                                                   SUMOReal start, SUMOReal end,
                                                   const std::string &mvmt)
    : myLane(lane), mySection(section), myStart(start), myEnd(end)
{
}

NIArtemisTempEdgeLanes::LinkLaneDesc::~LinkLaneDesc()
{
}


SUMOReal
NIArtemisTempEdgeLanes::LinkLaneDesc::getStart() const
{
    return myStart;
}


SUMOReal
NIArtemisTempEdgeLanes::LinkLaneDesc::getEnd() const
{
    return myEnd;
}


int
NIArtemisTempEdgeLanes::LinkLaneDesc::getLane() const
{
    return myLane;
}


void
NIArtemisTempEdgeLanes::LinkLaneDesc::patchPosition(SUMOReal length)
{
    if(myStart!=-1) {
        myStart = length - myStart;
    }
    if(myEnd!=-1) {
        myEnd = length - myEnd;
    }
}







void
NIArtemisTempEdgeLanes::add(const std::string &link, int lane,
                            int section, SUMOReal start, SUMOReal end,
                            const std::string &mvmt)
{
    myLinkLaneDescs[link].push_back(
        new LinkLaneDesc(lane, section, start, end, mvmt));
    myLinkLanePositions[link].push_back(start);
    myLinkLanePositions[link].push_back(end);
}


void
NIArtemisTempEdgeLanes::close(NBDistrictCont &dc,
                              NBEdgeCont &ec, NBNodeCont &nc)
{
    Link2LaneDesc::iterator i;
    // go through the list of edges to which infomration about lanes have
    //  been added and patch the position information
    for(i=myLinkLaneDescs.begin(); i!=myLinkLaneDescs.end(); i++) {
        string name = (*i).first;
        NBEdge *edge = ec.retrieve(name);
        SUMOReal length = edge->getLength();
        // patch the information within the lane descriptions
        LaneDescVector defs = (*i).second;
        for(LaneDescVector::iterator j=defs.begin(); j!=defs.end(); j++) {
            (*j)->patchPosition(length);
        }
        // patch the information within the lane positions
        DoubleVector &poses = myLinkLanePositions[name];
        for(DoubleVector::iterator k=poses.begin(); k!=poses.end(); k++) {
            if((*k)>0) {
                (*k) = length - (*k);
            }
        }
    }
    // go through the list of edges to which infomration about lanes have
    //  been added and compute the lane changings using the patched values
    for(i=myLinkLaneDescs.begin(); i!=myLinkLaneDescs.end(); i++) {
        // get the name of the link
        string name = (*i).first;
        // get the edge and check it
        NBEdge *edge = ec.retrieve(name);
        if(edge==0) {
            MsgHandler::getErrorInstance()->inform(
                string("Trying to assign lanes to the unknown edge '")
                + name + string("'."));
            continue;
        }
        // retrieve the number of maximum lanes
        size_t maxLaneNo = edge->getNoLanes();
        // retrieve all positions the edge changes at
        DoubleVector poses = myLinkLanePositions[name];
        // remove SUMOReal information then convert and sort the list
        poses.push_back(edge->getLength());
        DoubleVectorHelper::removeDouble(poses);
        sort(poses.begin(), poses.end());
        // get the definitions for this edge
        LaneDescVector defs = myLinkLaneDescs[name];
        // build some kind of a matrix containing lane definitions
        std::vector<std::bitset<64> > setLanes(poses.size());
        for(LaneDescVector::iterator j=defs.begin(); j!=defs.end(); j++) {
            LinkLaneDesc *def = *j;
            size_t begidx = getBeginIndex(def->getStart(), poses);
            size_t endidx = getEndIndex(def->getEnd(), poses);
            size_t laneNo = def->getLane();
            for(size_t k=begidx; k<endidx+1; k++) {
                setLanes[k].set(laneNo);
            }
        }
        // go through the build container and split the edge
        SUMOReal lengthRemoved = 0;
        for(size_t k=1; k<setLanes.size()-1; k++) {
            // get the position of the lane changing
            string nodename = name + toString<int>(k-1) + string("x") + name + toString<int>(k);
            assert(edge->getLength()>(poses[k]-lengthRemoved));
            Position2D pos =
                edge->getGeometry().positionAtLengthPosition(poses[k]-lengthRemoved);
            assert(pos.x()>0&&pos.y()>0);
            // build the node and try to insert it into the net description
            NBNode *node = nc.retrieve(pos);
            if(node==0) {
                node = new NBNode(nodename, pos,
                    NBNode::NODETYPE_PRIORITY_JUNCTION);
                if(!nc.insert(node)) {
                    MsgHandler::getErrorInstance()->inform(
                        string("Problems on adding a lane-splitting node for edge '")
                        + name + string("'."));
                    delete node;
                    return;
                }
            }
            // split the edge
            string name1 =
                name + string("[") + toString<int>(k-1) + string("]");
            string name2 =
                name + string("[") + toString<int>(k) + string("]");
            size_t laneNo1 = count(setLanes[k-1]);
            size_t laneNo2 = count(setLanes[k]);
            ec.splitAt(dc,
                edge, poses[k]-lengthRemoved, node, name1, name2,
                laneNo1, laneNo2);
            lengthRemoved = poses[k];
            // get the build edges
            NBEdge *edge1 = ec.retrieve(name1);
            NBEdge *edge2 = ec.retrieve(name2);
            // assign the lane directions
            //  given only for lanes which end at this point
            //  other are simply connected to each other
            size_t runLaneNo1 = 0;
            size_t runLaneNo2 = 0;
            for(size_t l=0; l<maxLaneNo; l++) {
                // if both lanes exist, connect
                if(setLanes[k-1].test(l)&&setLanes[k].test(l)) {
                    if(!edge1->addLane2LaneConnection(runLaneNo1, edge2, runLaneNo2)) {
                        MsgHandler::getErrorInstance()->inform("Could not set connection!!!");
                        throw ProcessError();
                    }
                }
                // if a new lane starts, connect checking the direction
                if(!setLanes[k-1].test(l)&&setLanes[k].test(l)) {
                    // lane is a new rightmost lane
                    if(l>0&&setLanes[k-1].test(l-1)) {
                        if(!edge1->addLane2LaneConnection(0, edge2, runLaneNo2)) {
                            MsgHandler::getErrorInstance()->inform("Could not set connection!!!");
                            throw ProcessError();
                        }
                    }
                    // lane is a new leftmost lane
                    if(setLanes[k-1].test(l+1)) {
                        if(!edge1->addLane2LaneConnection(edge1->getNoLanes()-1, edge2, runLaneNo2)) {
                            MsgHandler::getErrorInstance()->inform("Could not set connection!!!");
                            throw ProcessError();
                        }
                    }
                }
                // if a lane ends, or no lanes are given do nothing
                // increase the current lane no
                if(setLanes[k-1].test(l)) {
                    runLaneNo1++;
                }
                if(setLanes[k].test(l)) {
                    runLaneNo2++;
                }
            }
            // the next edge to split will be the one build last
            edge = edge2;
        }
    }
}


size_t
NIArtemisTempEdgeLanes::getBeginIndex(SUMOReal start, const DoubleVector &poses)
{
    if(start==-1) {
        return 0;
    }
    DoubleVector::const_iterator i =
        find(poses.begin(), poses.end(), start);
    return distance(poses.begin(), i);
}


size_t
NIArtemisTempEdgeLanes::getEndIndex(SUMOReal end, const DoubleVector &poses)
{
    if(end==-1) {
        return poses.size()-1;
    }
    DoubleVector::const_iterator i =
        find(poses.begin(), poses.end(), end);
    return distance(poses.begin(), i);
}

size_t
NIArtemisTempEdgeLanes::count(const std::bitset<64> &lanes)
{
    size_t no = 0;
    for(size_t i=0; i<64; i++) {
        if(lanes.test(i)) {
            no++;
        }
    }
    return no;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


