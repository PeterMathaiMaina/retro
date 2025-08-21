#include "Util.h"

namespace Util{
    inline std::vector<FileInfo> IterateDirectory(
        const std::string& dirPath,
        const std::vector<std::string>& allowedExtensions  ={}
    ){

        std::vector<FileInfo> results;
        namespace fs = std::filesystem;

        if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
            return results;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (!entry.is_regular_file())
                continue;

            FileInfo fi(entry);

            // if we have extension filters, check them
            if (!allowedExtensions.empty()) {
                bool ok = false;
                for (const auto& ext : allowedExtensions) {
                    if (fi.extension == ext) {
                        ok = true;
                        break;
                    }
                }
                if (!ok) continue;
            }

            results.push_back(fi);
        }

        return results;
    }

    
}