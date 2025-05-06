

shapeKey{
    int labelTag;
    int tnamingId;
}





ShapeCore.addFreeShape(TopoDS_Shape aShape){
    IntShapeKey key = OcafShapeRegistry.addShape(aShape); //Creates label and attr. based on attr it returns key
    ShapeId id = shapeMap.atShape(aShape);
    keyIdStorage.add(key, id); //Stores id based on key
}

ShapeCore.removeFreeShape(TopoDS_Shape aShape){
    IntShapeKey key = OcafShapeRegistry.shapeKey(aShape); //Creates label and attr. based on attr it returns key
    keyIdStorage.remove(key)
    OcafShapeRegistry.removeShape(aShape);
}

ShapeCore.reviewDelta(){
    int i = removedShapeAttr;
    key = IntShapeKey::createKey(i);
    ShapeId id = keyIdStorage.atKey(key);
    publishShapeRemoved(id);
}

ShapeCore.labelSubShape(TopoDS_Shape aShape){
    IntShapeKey key = OcafShapeRegistry.addShape()
}


ShapeCore.removeShape(TopoDS_Shape aShape){
    IntShapeKey key = OcafShapeRegistry.addShape()
}