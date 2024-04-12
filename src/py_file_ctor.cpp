#include "../include/PythonFileCreator.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

PythonFileCreator::PythonFileCreator() = default;

std::string PythonFileCreator::double_array_to_string(double* arr, int array_size)  {
    std::stringstream ss;
    for (int i = 0; i < array_size; ++i) {
        if(i == array_size -1){
            ss << arr[i];
            break;
        }
        ss << arr[i] << ", ";
    }
    return ss.str();
}

void PythonFileCreator::add_lines_to_python_file(const std::string& filename, const std::string& string)  {
    std::ofstream outfile(filename, std::ios::app);
    if (outfile.is_open()) {
        outfile << string;
        outfile.close();
        std::cout << "Lines added to Python file '" << filename << "' successfully.\n";
    } else {
        std::cerr << "Unable to open Python file.\n";
    }
}

void PythonFileCreator::clear_file_content(const std::string& filename) {
    std::ofstream outfile(filename, std::ios::trunc);
    if (outfile.is_open()) {
        outfile.close();
        std::cout << "Content of file '" << filename << "' cleared successfully.\n";
    } else {
        std::cerr << "Unable to clear content of file.\n";
    }
}
