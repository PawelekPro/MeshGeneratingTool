/*
* Copyright (C) 2024 Krystian Fudali
*
* This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <cstdlib>
#include <iostream>

#include <XmlXCAFDrivers.hxx>

int main()
{
Handle(TDocStd_Application) app = new TDocStd_Application;
Handle(TDocStd_Document) doc;
app->NewDocument("XmlXCAF", doc);
app = Handle(TDocStd_Application)::DownCast(doc->Application());

XmlXCAFDrivers::DefineFormat(app);
if(!app->IsDriverLoaded()){
    std::cout << "Could not load driver";
    return 0;
} else {
    std::cout << "Storage format: " << doc->StorageFormat() << std::endl;
}

doc->SetUndoLimit(10);
TDF_Label rootLabel = doc->Main();
doc->NewCommand();
TDF_Label childLabel = rootLabel.NewChild();
Handle(TDataStd_Name) nameAttribute = TDataStd_Name::Set(childLabel, TCollection_ExtendedString("MyLabel"));
doc->CommitCommand();

const TCollection_ExtendedString fileName("document.xml");
app->SaveAs(doc, fileName);

std::cout<< "Undo count before undo: " << doc->GetAvailableUndos() << std::endl;
doc->Undo();
std::cout<< "Undo count after undo: " << doc->GetAvailableUndos() << std::endl;

Handle(TDocStd_Document) loadedDoc;
app->Open(fileName, loadedDoc);

if (loadedDoc.IsNull()) {
    std::cerr << "Failed to load document." << std::endl;
    return 1;
} else {
    std::cout << "Document loaded successfully!" << std::endl;
}

TDF_Label loadedRootLabel = loadedDoc->Main();
TDF_Label loadedChildLabel = loadedRootLabel.FindChild(1);

Handle(TDataStd_Name) loadedNameAttribute;
if (!loadedChildLabel.FindAttribute(TDataStd_Name::GetID(), loadedNameAttribute)) {
    std::cerr << "Failed to retrieve name attribute from loaded document." << std::endl;
}
return 0;
}