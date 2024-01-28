#ifndef QVTKINTERACTORSTYLE_H
#define QVTKINTERACTORSTYLE_H

#include "QVTKRenderWindow.h"

#include <vtkInteractorStyleTrackballCamera.h>

#include <QAction>
#include <QMenu>

namespace Interactor {
class QVTKInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	static QVTKInteractorStyle* New(Rendering::QVTKRenderWindow* renWin);

	virtual void OnRightButtonDown() override;
	Rendering::QVTKRenderWindow* getRenderWindow();

protected:
	QVTKInteractorStyle(Rendering::QVTKRenderWindow* qvtkRenderWindow);
	~QVTKInteractorStyle();

private:
	void createContextMenu();

	Rendering::QVTKRenderWindow* _qvtkRenderWindow;
	QMenu* _contextMenu;
	QAction* _customAction;
};
};

#endif