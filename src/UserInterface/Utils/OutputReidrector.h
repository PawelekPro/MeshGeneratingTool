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
#ifndef OUTPUTREDIRECTOR_H
#define OUTPUTREDIRECTOR_H

#include "ProgressBar.h"

#include <streambuf>
#include <iostream>
#include <string>
#include <functional>
#include <regex>
#include <QProgressBar>

class OutputRedirector : public std::streambuf {
public:
    OutputRedirector(ProgressBar *progressBar);

    ~OutputRedirector();

    void progressMeshing();

protected:
    virtual std::streamsize xsputn(const char* s, std::streamsize n) override;
    virtual int overflow(int c) override;

private:
    void parseGmshMeshing(const std::string &output);

    std::function<void(const std::string &)> _outputHandler;
    ProgressBar *_progressBar;
    std::streambuf *_oldStdoutBuf;
    std::streambuf *_oldStderrBuf;
};

#endif OUTPUTREDIRECTOR_H