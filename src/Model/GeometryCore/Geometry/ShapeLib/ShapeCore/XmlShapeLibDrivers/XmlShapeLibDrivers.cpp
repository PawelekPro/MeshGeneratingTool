#include "XmlShapeLibDrivers.hpp"
#include "XmlShapeLibDrivers_ShapeIdDriver.hpp"

#include <Message_Messenger.hxx>
#include <Plugin_Macro.hxx>
#include <Standard_GUID.hxx>
#include <XmlMDF_ADriverTable.hxx>

static Standard_GUID MyStorageDriverGUID  ("77e86faa-f21d-4f14-82dc-a19a55580a2d");
static Standard_GUID MyRetrievalDriverGUID("0caa79eb-774d-4ac3-be37-3edbfc5a1aa7");

const Handle(Standard_Transient)&
XmlShapeLibDrivers::Factory(const Standard_GUID& theGUID)
{
  static Handle(Standard_Transient) nullDriver;
  if (theGUID == MyStorageDriverGUID ||
      theGUID == MyRetrievalDriverGUID)
  {
    Handle(Message_Messenger) msg = new Message_Messenger();
    static Handle(Standard_Transient) driver =
      new XmlShapeLibDrivers_ShapeIdDriver(
        msg,
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
  theTable->AddDriver( new XmlShapeLibDrivers_ShapeIdDriver(theMsg, "ShapeId"));
}

PLUGIN(XmlShapeLibDrivers)
