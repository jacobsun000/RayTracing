#pragma once

#include <iostream>
#include <string>

void showProgressBar(double progress) {
    std::cout << "\033[?25l";
    const int barWidth = 50;
    int filledWidth = static_cast<int>(progress * barWidth);

    std::string progressBar;
    progressBar += "Rendering [";
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
    std::cout << "\033[?25h";
}