#ifndef QVTKRENDERWINDOW_H
#define QVTKRENDERWINDOW_H

#include <QVTKOpenGLNativeWidget.h>

namespace Rendering {
    
    class QVTKRenderWindow {
        public:
            QVTKRenderWindow(QVTKOpenGLNativeWidget* vtkWidget);


        private:
            QVTKOpenGLNativeWidget* _vtkWidget;
        
    };
};

#endif