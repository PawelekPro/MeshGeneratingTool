/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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

*=============================================================================
* File      : ProcessRunner.hpp
* Author    : Paweł Gilewicz
* Date      : 05/04/2025
*/
#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QFutureWatcher>
#include <QObject>
#include <QThread>
#include <atomic>
#include <future>

class ProcessRunner final : public QThread {
	Q_OBJECT

public:
	explicit ProcessRunner(QObject* parent = nullptr)
		: QThread(parent)
		, stopRequested(false) { }

	template <typename Func> void runAsync(Func&& func) {
		this->task = std::forward<Func>(func);
		start();
	}

	void stop() {
		stopRequested.store(true);
		emit statusMessage("Stopping process...");
	}

	bool isStopRequested() const { return stopRequested.load(); }

signals:
	void progressUpdated(int);
	void statusMessage(const QString&);
	void finishedSuccessfully();
	void failed(const QString& error);

protected:
	void run() override {
		try {
			if (task) {
				task();
				emit finishedSuccessfully();
			}
		} catch (const std::exception& e) {
			emit failed(QString("Error: %1").arg(e.what()));
		} catch (...) {
			emit failed("Unknown error occurred.");
		}
	}

private:
	std::function<void()> task;
	std::atomic<bool> stopRequested;
};

#endif
