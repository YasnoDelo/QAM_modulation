#ifndef PYTHON_FILE_CREATOR_H
#define PYTHON_FILE_CREATOR_H

#include <string>

class PythonFileCreator {
public:
    PythonFileCreator();
    std::string double_array_to_string(double* arr, int array_size);
    void add_lines_to_python_file(const std::string& filename, const std::string& string);
    void clear_file_content(const std::string& filename);
};

#endif // PYTHON_FILE_CREATOR_H

