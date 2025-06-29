
#include "XmlShapeLibDrivers_ShapeIdDriver.hpp"

#include "LabelPathAttr.hpp"
#include <Message_Messenger.hxx>
#include <Standard_Type.hxx>
#include <XCAFDoc_Note.hxx>
#include <XmlMXCAFDoc_NoteDriver.hxx>
#include <XmlObjMgt_Persistent.hxx>

IMPLEMENT_STANDARD_RTTIEXT(XmlShapeLibDrivers_ShapeIdDriver, XmlMDF_ADriver)
IMPLEMENT_DOMSTRING(LabelTag, "labelTag")
IMPLEMENT_DOMSTRING(ParentLabelTag, "parentLabelTag")

XmlShapeLibDrivers_ShapeIdDriver::XmlShapeLibDrivers_ShapeIdDriver(
  const Handle(Message_Messenger)& theMsgDriver,
  Standard_CString                theName)
: XmlMDF_ADriver(theMsgDriver, theName)
{
}

Handle(TDF_Attribute) XmlShapeLibDrivers_ShapeIdDriver::NewEmpty() const {
    return new LabelPathAttr();
}

Handle(Standard_Type) XmlShapeLibDrivers_ShapeIdDriver::SourceType() const
{
  return STANDARD_TYPE(LabelPathAttr);
}

Standard_Boolean XmlShapeLibDrivers_ShapeIdDriver::Paste(
    const XmlObjMgt_Persistent&  theSource,
    const Handle(TDF_Attribute)& theTarget,
    XmlObjMgt_RRelocationTable&  /*theRelocTable*/
) const {

    const XmlObjMgt_Element& anElement = theSource;
    
    XmlObjMgt_DOMString labelTag = anElement.getAttribute(::LabelTag());
    XmlObjMgt_DOMString parentLabelTag = anElement.getAttribute(::ParentLabelTag());

    if (labelTag == NULL || parentLabelTag == NULL) {
        return Standard_False;
    }

    Handle(LabelPathAttr) shapeIdAttr = Handle(LabelPathAttr)::DownCast(theTarget);
    if (shapeIdAttr.IsNull()) {
        return Standard_False;
    }
    Standard_Integer labelTagInt, parentLabelTagInt;
    
    Standard_Boolean labelTagRead = labelTag.GetInteger(labelTagInt);
    Standard_Boolean parentLabelTagRead = parentLabelTag.GetInteger(parentLabelTagInt);

    if (labelTagRead == NULL || parentLabelTagRead == NULL) {
        return Standard_False;
    }
    
    shapeIdAttr->Set(labelTagInt, parentLabelTagInt);
    return Standard_True;
}

void XmlShapeLibDrivers_ShapeIdDriver::Paste(
    const Handle(TDF_Attribute)&   theSource,
    XmlObjMgt_Persistent&          theTarget,
    XmlObjMgt_SRelocationTable&    /*theRelocTable*/
) const {
    
    Handle(LabelPathAttr) attr = Handle(LabelPathAttr)::DownCast(theSource);
    if (attr.IsNull()) {
        return;
    }

    Standard_Integer labelTagInt, parentLabelTagInt;
    attr->Get(labelTagInt, parentLabelTagInt);
    
    XmlObjMgt_Element& anElement = theTarget.Element();
    
    anElement.setAttribute(::LabelTag(), labelTagInt);
    anElement.setAttribute(::ParentLabelTag(), parentLabelTagInt);
}