#include "OutputReidrector.h"

OutputRedirector::OutputRedirector(ProgressBar* progressBar)
    :   _progressBar(progressBar),
        _oldStdoutBuf(std::cout.rdbuf()),
        _oldStderrBuf(std::cerr.rdbuf()){
            std::cout.rdbuf(this);
            std::cerr.rdbuf(this);
        }

OutputRedirector::~OutputRedirector() {
        std::cout.rdbuf(_oldStdoutBuf);
        std::cerr.rdbuf(_oldStderrBuf);
    }

std::streamsize OutputRedirector::xsputn(const char* s, std::streamsize n){
        std::string output(s, n);
        if (_outputHandler) {
            _outputHandler(output);
        }
        return n;
    }

int OutputRedirector::overflow(int c){
        if (c != EOF) {
            char z = static_cast<char>(c);
            if (_outputHandler) {
                _outputHandler(std::string(1, z));
            }
        }
        return c;
    }


void OutputRedirector::progressMeshing(){
        _outputHandler = [this](const std::string &output) {
            parseGmshMeshing(output);
        };
    }

void OutputRedirector::parseGmshMeshing(const std::string &output) {
        std::regex progressRegex(R"(\[\s*(\d+)%\s*\]\s*Meshing\s*surface\s*(\d+))");
        std::smatch match;
        if (std::regex_search(output, match, progressRegex)) {
            int progress = std::stoi(match[1]);
            if (_progressBar) {
                _progressBar->setValue(progress);
            }
        }
    }