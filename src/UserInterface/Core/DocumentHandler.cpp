#include "DocumentHandler.h"

const QMap<DocumentHandler::EntryTag, QString> DocumentHandler::entryTags = {
    { DocumentHandler::EntryTag::MeshSizing, "MeshSizing" },
    { DocumentHandler::EntryTag::STEPImport, "STEPImport" },
};
const QMap<DocumentHandler::RootTag, QString> DocumentHandler::rootTags = {
    { DocumentHandler::RootTag::GeomImport, "GeometryImports" },
    { DocumentHandler::RootTag::GeomModel, "GeometryModel" },
    { DocumentHandler::RootTag::CSYS, "CoordinateSystem" },
    { DocumentHandler::RootTag::Mesh, "Mesh" }
};