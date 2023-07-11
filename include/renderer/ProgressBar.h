#pragma once

#include <iostream>
#include <string>

void showProgressBar(double progress) {
    const int barWidth = 70;
    int filledWidth = static_cast<int>(progress * barWidth);

    std::string progressBar;
    progressBar += "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filledWidth)
            progressBar += "=";
        else if (i == filledWidth)
            progressBar += ">";
        else
            progressBar += " ";
    }
    progressBar +=
        "] " + std::to_string(static_cast<int>(progress * 100.0)) + "%";

    std::cout << progressBar << "\r";
    std::cout.flush();
}