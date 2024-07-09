#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include "IniFile.h"

#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

struct stat _lasttime;
time_t lastUpdateTime(const std::string &filename) {
    if(stat(filename.c_str(), &_lasttime)==0) {
        return _lasttime.st_mtime;
    }
    return time(NULL);
}


std::vector<std::string> cpp_source, compilation_flags;
std::string target, outdir, indir;
IniFile Handler("build/build.ini");
/*
    "-lglew32",
    "-lfreeglut",
    "-lopengl32",
    "-lglu32",
    "\"-Wl,--subsystem,windows\""
*/

inline bool error(const std::string &s) {
    std::cout << s << std::endl;
    return false;
}

inline bool file_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;   
}

inline bool dir_exists(const std::string& name) {
    return std::filesystem::is_directory(name);
}

bool cmd_exec(const std::string &s) {
    int result = system(s.c_str());
    return result==0;
}

bool in_vector(const std::string &s, std::vector<std::string> &v) {
    for (auto it = v.begin(); it != v.end(); it++) {
        if (s == *it) return true;
    }
    return false;
}

void print_info_about() {
    std::cout << "-b \t builds the binaries\n--build\n\n"
        << "-fb \t forcefully recompiles to obj files, even if already exists, then builds the binaries\n--force_build\n\n"
        << "-r \t recompiles obj files\n--recompile\n\n"
        << "-fr \t forcefully recompiles to obj files, even if already exists\n--force_recompile\n\n"
    << std::endl;
}

bool spec_recompile(const std::string &filename) {
    return cmd_exec("g++ " + indir + filename + ".cpp -c " + " -o " + outdir + filename + ".o");
}

bool recompile(bool force=false) {
    if (!dir_exists(indir)) return error("Indir does not exist");
    if ((outdir != "" || outdir != ".\\") && !dir_exists(outdir)) {
        std::filesystem::create_directory(outdir);
    }
    std::cout << "Compilation:" << std::endl;
    for (auto it = cpp_source.begin(); it != cpp_source.end(); it++) {
        //std::cout << *it + " " << lastUpdateTime(outdir + *it + ".o") << " " << lastUpdateTime(indir + *it + ".cpp") << std::endl;
        if (file_exists(outdir + *it + ".o") 
            && lastUpdateTime(outdir + *it + ".o") >= lastUpdateTime(indir + *it + ".cpp")
            && !force) continue;
        auto start = std::chrono::steady_clock::now();
        if (!spec_recompile(*it)) return error("Cannot compile file " + *it + ".cpp");
        auto end = std::chrono::steady_clock::now();
        std::cout << "\t\033[34m" << *it << ".cpp:\033[0m\t" 
        << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;
    }
    return true;
}

bool build(bool force=false) {
    if (!recompile(force)) return error("Compilation error");
    
    std::string command = "g++ ";
    for (auto it = cpp_source.begin(); it != cpp_source.end(); it++) {
        command += outdir + *it + ".o ";
    }
    for (auto it = compilation_flags.begin(); it != compilation_flags.end(); it++) {
        command += *it + " ";
    }
    command += "-o " + outdir + target;
    std::cout << "Linking:" << std::endl;
    auto start = std::chrono::steady_clock::now();
    bool result = cmd_exec(command);
    if (result) {
        auto end = std::chrono::steady_clock::now();
        std::cout << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;
    }
    return result;
}

bool load_build_data() {
    if (!Handler.isSectionExist("settings") 
    || !Handler.isSectionExist("cpp_source")) return error("Does not exist needed segment(s)");

    if (!Handler.isKeysExist("settings", "target")) return error("No key \"target\" exists");
    target = Handler.readString("settings", "target");
    if (!Handler.isKeysExist("settings", "outdir")) return error("No key \"outdir\" exists");
    outdir = Handler.readString("settings", "outdir");
    if (!Handler.isKeysExist("settings", "indir")) return error("No key \"indir\" exists");
    indir = Handler.readString("settings", "indir");

    auto src = Handler.getKeys("cpp_source");
    for (auto it = src.begin(); it != src.end(); it++) {
        if (std::stoi(it->second))
            cpp_source.push_back(it->first);
    }

    if (Handler.isSectionExist("compilation_flags")) {
        auto flags = Handler.getKeys("compilation_flags");
        for (auto it = flags.begin(); it != flags.end(); it++) {
            if (std::stoi(it->second))
                compilation_flags.push_back(it->first);
        }
    }

    return true;
}

int main(int argc, char** argv) {
    system("");
    if (!load_build_data()) {
        std::cout << "\033[31;1m Cannot read ini-file \033[0m" << std::endl;
        return 1;
    }

    std::vector<std::string> args;
    int count = 0;
    while (count < argc) {
        args.push_back( std::string( argv[count] ) );
        count++;
    }
    bool result = true;
    auto start = std::chrono::steady_clock::now();
    if (count > 1) {
        if (in_vector("--help", args)) {
            print_info_about();
            return 0;
        }

        if (in_vector("-d", args) || in_vector("--delete", args)) {
            if (!cmd_exec("rm " + outdir)) {
                std::cout << "\033[31m Deletion has failed \033[0m" << std::endl;
                return 1;
            };
        }

        if (in_vector("-fb", args) || in_vector("-bf", args) || in_vector("--force_build", args)) {
            result = build(true);
        }
        else if (in_vector("-fr", args) || in_vector("-rf", args) || in_vector("--force_recompile", args)) {
            result = recompile(true);
        }
        else if (in_vector("-b", args) || in_vector("--build", args)) {
            result = build();
        }
        else if (in_vector("-r", args) || in_vector("--recompile", args)) {
            result = recompile();
        }
    } else {
        result = build();
        //print_info_about();
    }
    auto end = std::chrono::steady_clock::now();
    
    if (!result) {
        std::cout << "\033[31;1m Error occurred \033[0m" << std::endl;
    } else {
        std::cout << "\033[32m Finished\033[0m total in " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;
    }

    return result;
}   