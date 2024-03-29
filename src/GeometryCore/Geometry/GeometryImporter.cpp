#include "GeometryImporter.h"

std::string GeometryCore::GeometryImporter::getUniqueObjectName(const std::string& prefix, const PartsMap& objectMap){
 	int i = 0;
    std::string uniqueName;
    for (const auto& pair : objectMap) {
        const std::string& stringObj = pair.first;

        if (stringObj.find(prefix) != std::string::npos) {
            std::stringstream stringStream;
            stringStream << prefix << std::setfill('0') << std::setw(3) << i;
            uniqueName = stringStream.str();
            if (objectMap.find(uniqueName) == objectMap.end()) {
                break;
            }
            i++;
        }
    }
	return uniqueName;
};


