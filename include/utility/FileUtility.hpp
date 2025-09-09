#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace utility {

class FileUtility {
  public:
    static std::string getFileToString(const std::string &filename);

    static std::string getFilenameFromPath(const std::string &path);

    static std::string getBasenameFromPath(const std::string &path);

    static std::string getParentDirectory(const std::string &path);

    static bool pathExists(const std::string &path);

    static void createFile(const std::string &path);

    static void loadJson(nlohmann::json &json, const std::string &path);

    static void saveJson(const nlohmann::json &json, const std::string &path);

    static std::vector<std::string> getFilesInDirectory(std::string directory);

    static std::vector<std::string> getPathsInDirectory(std::string directory);

    static bool isFile(const std::string &path);

    static bool isDirectory(const std::string &path);
};

} // namespace utility
