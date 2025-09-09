#include "utility/FileUtility.hpp"

#include <stack>
#include <sstream>
#include <fstream>
#include <filesystem>

namespace utility {

std::string FileUtility::getFileToString(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

std::string FileUtility::getFilenameFromPath(const std::string &path) {
    std::filesystem::path fullPath(path);

    std::string filename = fullPath.filename().string();

    return filename;
}

std::string FileUtility::getBasenameFromPath(const std::string &path) {
    std::filesystem::path fullPath(path);

    std::string basename = fullPath.stem().string();

    return basename;
}

std::string FileUtility::getParentDirectory(const std::string &path) {
    std::filesystem::path fullPath(path);

    std::string parentDirectory = fullPath.parent_path().string();

    return parentDirectory;
}

bool FileUtility::pathExists(const std::string &path) {
    return std::filesystem::exists(path);
}

void FileUtility::createFile(const std::string &path) {
    if (FileUtility::pathExists(path)) {
        return;
    }

    std::ofstream(path).close();
}

void FileUtility::loadJson(nlohmann::json &json, const std::string &path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return;
    }

    file >> json;
}

void FileUtility::saveJson(const nlohmann::json &json, const std::string &path) {
    std::ofstream file(path);

    if (!file.is_open()) {
        return;
    }

    file << std::setw(2) << json;
}

// Assume starts with name/
std::vector<std::string> FileUtility::getFilesInDirectory(std::string directory) {
    std::vector<std::string> files;

    std::stack<std::string> directories;

    directories.push(directory);

    while (!directories.empty()) {
        std::string directory = directories.top();

        directories.pop();

        if (!FileUtility::pathExists(directory)) {
            continue;
        }

        std::vector<std::string> pathsInDirectory = FileUtility::getPathsInDirectory(directory);

        for (std::string path : pathsInDirectory) {
            if (FileUtility::isFile(path)) {
                files.push_back(path);
            } else if (FileUtility::isDirectory(path)) {
                directories.push(path);
            }
        }
    }

    return files;
}

std::vector<std::string> FileUtility::getPathsInDirectory(std::string directory) {
    std::vector<std::string> paths;

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        paths.push_back(entry.path().string());
    }

    return paths;
}

bool FileUtility::isFile(const std::string &path) {
    return std::filesystem::is_regular_file(path);
}

bool FileUtility::isDirectory(const std::string &path) {
    return std::filesystem::is_directory(path);
}

} // namespace utility
