#ifndef GUIDialog_GLObjChooser_h
#define GUIDialog_GLObjChooser_h
//---------------------------------------------------------------------------//
//                        GUIDialog_GLObjChooser.h -
//  Class for the window that allows to choose a street, junction or vehicle
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
// Revision 1.7  2005/10/07 11:36:48  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.6  2005/09/15 11:05:28  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.5  2005/07/12 11:55:37  dkrajzew
// fonts are now drawn using polyfonts; dialogs have icons; searching for structures improved;
//
// Revision 1.4  2005/04/27 09:44:26  dkrajzew
// level3 warnings removed
//
// Revision 1.3  2004/11/23 10:00:08  dkrajzew
// new class hierarchy for windows applied
//
// Revision 1.2  2004/04/02 10:58:27  dkrajzew
// visualisation whether an item is selected added
//
// Revision 1.1  2004/03/19 12:33:36  dkrajzew
// porting to FOX
//
// Revision 1.1  2004/03/19 12:32:26  dkrajzew
// porting to FOX
//
// Revision 1.3  2003/05/20 09:23:54  dkrajzew
// some statistics added; some debugging done
//
// Revision 1.2  2003/02/07 10:34:14  dkrajzew
// files updated
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
#include <vector>
#include <fx.h>
#include <utils/gui/globjects/GUIGlObjectTypes.h>


/* =========================================================================
 * class declarations
 * ======================================================================= */
class GUISUMOViewParent;
class GUIGlObjectStorage;
class GUIGlObject;


/* =========================================================================
 * class definition
 * ======================================================================= */
/**
 * @class GUIDialog_GLObjChooser
 * Instances of this class are windows that display the list of instances
 * from a given artifact like vehicles, edges or junctions and allow
 * one of their items
 */
class GUIDialog_GLObjChooser : public FXMainWindow
{
    // FOX-declarations
    FXDECLARE(GUIDialog_GLObjChooser)

public:
    /// constructor
    GUIDialog_GLObjChooser(GUISUMOViewParent *parent, GUIGlObjectType type,
        GUIGlObjectStorage &glStorage);

    /// destructor
    ~GUIDialog_GLObjChooser();

    /// Returns the chosen (selected) object
    GUIGlObject *getObject() const;

    /// The selected item shall be centered within the calling view
    long onCmdCenter(FXObject*,FXSelector,void*);

    /// The dialog shall be closed
    long onCmdClose(FXObject*,FXSelector,void*);

    /// Somthing has been typed into the tet field
    long onCmdTextChanged(FXObject*,FXSelector,void*);

private:
    /// the list that holds the ids
    FXList *myList;

    /// the chosen id
    GUIGlObject *mySelected;

    /// the artifact to choose
    GUIGlObjectType myObjectType;

    /// the parent window
    GUISUMOViewParent *myParent;

    /// The text field
    FXTextField *myTextEntry;

protected:
    /// FOX needs this
    GUIDialog_GLObjChooser() { }

};


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

