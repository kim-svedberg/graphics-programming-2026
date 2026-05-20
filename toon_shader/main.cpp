#include "ViewerApplication.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    // Fix broken .mtl file 
    std::string inputPath = "models/miku/Default.mtl";

    std::fstream inputFile(inputPath);

    if (!inputFile.is_open())
    {
        std::cout << "Failed to open file.\n";
        return 1;
    }

    std::string outputText;
    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line.rfind("map_Kd ", 0) == 0)
        {
            std::string fullPath = line.substr(7);

            fs::path p(fullPath);

            std::string filename = p.filename().string();

            line = "map_Kd " + filename;
        }

        outputText += line + "\n";
    }

    inputFile.close();

    std::ofstream outputFile(inputPath);
    outputFile << outputText;
    outputFile.close();

    ViewerApplication viewerApplication;
    return viewerApplication.Run();
}
