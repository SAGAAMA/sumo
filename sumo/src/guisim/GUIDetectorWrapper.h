#ifndef GUIDetectorWrapper_h
#define GUIDetectorWrapper_h

#include <utils/geom/HaveBoundery.h>
#include <gui/GUIGlObject.h>

class QGLObjectPopupMenu;

class GUIDetectorWrapper
    : public HaveBoundery, public GUIGlObject {
public:
    GUIDetectorWrapper(GUIGlObjectStorage &idStorage,
        std::string id);
    ~GUIDetectorWrapper();
    QGLObjectPopupMenu *getPopUpMenu(GUIApplicationWindow &app,
        GUISUMOAbstractView &parent);
    virtual void drawGL_FG(double scale) const = 0;
    virtual void drawGL_SG(double scale) const = 0;
    virtual double getXCoordinate() const = 0;
    virtual double getYCoordinate() const = 0;

};

#endif
