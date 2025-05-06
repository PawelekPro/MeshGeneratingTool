#include "XmlShapeLibDrivers.hpp"
#include "XmlShapeLibDrivers_ShapeIdDriver.hpp"

#include <Message_Messenger.hxx>
#include <Plugin_Macro.hxx>
#include <Standard_GUID.hxx>
#include <XmlMDF_ADriverTable.hxx>

#include "XmlShapeLibDrivers.hpp"
#include "XmlShapeLibDrivers_ShapeIdDriver.hpp"
#include "ShapeIdAttribute.hpp"                  // <— your TDF_Attribute subclass
#include <Message_Messenger.hxx>
#include <Plugin_Macro.hxx>
#include <XmlMDF_ADriverTable.hxx>

const Handle(Standard_Transient)&
XmlShapeLibDrivers::Factory(const Standard_GUID& theGUID)
{
  static Handle(Standard_Transient) nullDriver;
  // single source of truth: ShapeId::GetID()
  if (theGUID == ShapeIdAttribute::GetID()) {
    static Handle(Standard_Transient) driver =
      new XmlShapeLibDrivers_ShapeIdDriver(
        new Message_Messenger(),
        "ShapeId"
      );
    return driver;
  }
  return nullDriver;
}

void XmlShapeLibDrivers::AttributeDrivers(
  const Handle(XmlMDF_ADriverTable)& theTable,
  const Handle(Message_Messenger)&   theMsg)
{
  // register your driver under the same ShapeId::GetID()
  theTable->AddDriver(
    new XmlShapeLibDrivers_ShapeIdDriver(theMsg, "ShapeId")
  );
}
PLUGIN(XmlShapeLibDrivers)