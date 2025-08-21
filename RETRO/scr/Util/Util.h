#pragma once


#include <string>
#include <filesystem>
#include <vector>

struct FileInfo{
    std::string path;
    std::string filename;
    std::string extension;

    FileInfo(const std::filesystem::directory_entry& entry){
        path = entry.path().string();
        filename = entry.path().filename().string();
        extension = entry.path().extension().string();


        if (!extension.empty() && extension[0] == '.')
            extension.erase();

        
    };

};