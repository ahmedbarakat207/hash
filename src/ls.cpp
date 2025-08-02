#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <unistd.h>
#include "exec.h"
#include "colors.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <filesystem>
#include "dir.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "ls.h"
#include <unordered_set>

using namespace std;
namespace fs = std::filesystem;

void print_files(const char* file, const char* color){
    cout << "║" << endl;
    cout << "╠══ ";
    print_color(file, color, "endl");
    //cout << "║" << endl;
}
bool is_executable(const fs::path& p) {
    auto perms = fs::status(p).permissions();
    return (perms & fs::perms::owner_exec) != fs::perms::none ||
           (perms & fs::perms::group_exec) != fs::perms::none ||
           (perms & fs::perms::others_exec) != fs::perms::none;
}
void ls() {
    std::string path = fs::current_path().string();
    static const std::unordered_set<std::string> build_files = {
        "Makefile", "meson.build", "CMakeLists.txt", "CMakeCache.txt"
    };
    
    static const std::unordered_set<std::string> source_exts = {
        ".cpp", ".cc", ".cxx", ".h", ".hpp", ".hxx", ".c", ".py", ".java",
        ".js", ".mjs", ".cjs", ".ts", ".tsx", ".html", ".htm", ".css", ".php",
        ".rb", ".rs", ".sh", ".bash", ".zsh", ".ksh", ".go", ".swift", ".kt",
        ".kts", ".scala", ".sbt", ".sql", ".json", ".yaml", ".yml", ".jl"
    };
    
    static const std::unordered_set<std::string> image_exts = {
        ".jpg", ".jpeg", ".jpe", ".jfif", ".png", ".gif", ".bmp", ".dib",
        ".tif", ".tiff", ".webp", ".svg", ".ico", ".heic", ".heif", ".raw",
        ".cr2", ".nef", ".orf", ".sr2", ".arw", ".dng"
    };
    
    static const std::unordered_set<std::string> document_exts = {
        ".doc", ".docx", ".dot", ".dotx", ".xls", ".xlsx", ".xlsm", ".csv",
        ".ppt", ".pptx", ".pps", ".ppsx", ".pdf", ".txt", ".log", ".md", ".rtf",
        ".odt", ".ods", ".odp", ".epub", ".mobi", ".azw", ".azw3"
    };
    
    static const std::unordered_set<std::string> archive_exts = {
        ".zip", ".tar", ".gz", ".gzip", ".bz2", ".bzip2", ".xz", ".lzma",
        ".7z", ".rar", ".iso", ".cab", ".arj", ".lzh", ".lz4", ".img", ".zst"
    };
    
    static const std::unordered_set<std::string> disk_image_exts = {
        ".vhd", ".vhdx", ".qcow2", ".vdi", ".vmdk", ".hdd", ".raw"
    };

    for (const auto& entry : fs::directory_iterator(path)) {
        const auto filename = entry.path().filename().string();
        const auto ext = entry.path().extension().string();

        if (!filename.empty() && filename.front() == '.') {
            continue;
        }

        if (entry.is_directory()) {
            print_files(filename.c_str(), "bright_cyan");
        }
        else if (is_executable(entry) || ext == ".apk") {
            print_files(filename.c_str(), "bright_green");
        }
        else if (build_files.find(filename) != build_files.end()) {
            print_files(filename.c_str(), "bright_yellow");
        }
        else if (source_exts.find(ext) != source_exts.end()) {
            print_files(filename.c_str(), "cyan");
        }
        else if (image_exts.find(ext) != image_exts.end()) {
            print_files(filename.c_str(), "bright_blue");
        }
        else if (ext == ".so") {
            print_files(filename.c_str(), "green");
        }
        else if (document_exts.find(ext) != document_exts.end()) {
            print_files(filename.c_str(), "bright_red");
        }
        else if (archive_exts.find(ext) != archive_exts.end()) {
            print_files(filename.c_str(), "red");
        }
        else if (disk_image_exts.find(ext) != disk_image_exts.end()) {
            print_files(filename.c_str(), "magenta");
        }
        else {
            print_files(filename.c_str(), "white");
        }
    }
    std::cout << "║" << std::endl;
}