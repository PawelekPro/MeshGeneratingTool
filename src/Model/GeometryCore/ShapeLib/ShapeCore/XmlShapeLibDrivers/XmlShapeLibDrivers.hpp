#ifndef XmlShapeLibDrivers_HPP
#define XmlShapeLibDrivers_HPP

#include <Standard_Handle.hxx>

class Standard_GUID;
class Standard_Transient;
class XmlMDF_ADriverTable;
class Message_Messenger;

class XmlShapeLibDrivers {
  public:
  Standard_EXPORT static const Handle(Standard_Transient)& Factory(const Standard_GUID& theGUID);

  Standard_EXPORT static void AttributeDrivers(
    const Handle(XmlMDF_ADriverTable)& theTable,
    const Handle(Message_Messenger)& theMsg
  );

};

#endif
