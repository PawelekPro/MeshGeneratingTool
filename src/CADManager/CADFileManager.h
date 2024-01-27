#ifndef CADFILEMANAGER_H
#define CADFILEMANAGER_H

#include <iostream>
#include <string>
#include <mutex>
#include <map>

#include <vtkSmartPointer.h>
#include <vtkActor.h>

namespace CAD {
    using namespace std::string_literals;

    using vec_3 = std::tuple<double, double, double>;
    using vec_4 = std::tuple<double, double, double, double>;

    using ActorsMap = std::map<std::string, vtkSmartPointer<vtkActor>>;
    using LabelsMap = std::map<std::string, std::string>;

    class CADFileManager {

        private:
        static CADFileManager *m_instance;
        static std::mutex m_mutex;

        protected:
        CADFileManager() {}
        ~CADFileManager() {}

        public:
        static CADFileManager* instance();

        std::string load(const std::string& stepName);
        std::string draw(vtkRenderer* renderer);

        
    };
}

#endif 