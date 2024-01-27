#ifndef STEPFILEREADER_H
#define STEPFILEREADER_H

#include <iostream>
#include <map>
#include <string>

#include <STEPCAFControl_Reader.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

namespace Importing {
using namespace std::string_literals;

using d_vec3 = std::tuple<double, double, double>;
using d_vec4 = std::tuple<double, double, double, double>;

using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
using LabelsMap = std::map<std::string, std::string>;
using PartsMap = std::map<std::string, TopoDS_Shape>;

using Trinity = std::tuple<STEPCAFControl_Reader, Handle(TDocStd_Document), PartsMap>;

class STEPFileReader {

public:
	STEPFileReader() { }
	~STEPFileReader() { }
	PartsMap load(const std::string& fileName);

	PartsMap partsMap;

private:
	std::string getUniqueObjectName(std::string prefix, const Importing::PartsMap& partsMap);
};

};

#endif