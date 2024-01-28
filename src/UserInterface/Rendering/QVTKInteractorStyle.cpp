#include "QVTKInteractorStyle.h"

#include <QAction>
#include <QDebug>

Interactor::QVTKInteractorStyle* Interactor::QVTKInteractorStyle::New(Rendering::QVTKRenderWindow* renWin) {
	return new Interactor::QVTKInteractorStyle(renWin);
}

Interactor::QVTKInteractorStyle::QVTKInteractorStyle(Rendering::QVTKRenderWindow* qvtkRenderWindow)
	: _contextMenu(nullptr)
	, _customAction(nullptr)
	, _qvtkRenderWindow(qvtkRenderWindow) { }

Interactor::QVTKInteractorStyle::~QVTKInteractorStyle() {
	if (_contextMenu)
		delete _contextMenu;
}

Rendering::QVTKRenderWindow* Interactor::QVTKInteractorStyle::getRenderWindow() {
	return this->_qvtkRenderWindow;
}

void Interactor::QVTKInteractorStyle::OnRightButtonDown() {
	this->createContextMenu();
	_contextMenu->exec(QCursor::pos());

	vtkInteractorStyle::OnRightButtonDown();
}

void Interactor::QVTKInteractorStyle::createContextMenu() {
	if (!_contextMenu) {
		_contextMenu = new QMenu;
		_customAction = new QAction("Fit view", _contextMenu);

		QObject::connect(_customAction, &QAction::triggered, [this]() {
			this->_qvtkRenderWindow->fitView();
		});

		// Add the custom QAction to the context menu
		_contextMenu->addAction(_customAction);
	}
}