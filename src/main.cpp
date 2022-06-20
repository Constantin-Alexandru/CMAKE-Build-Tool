#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

#include <filesystem>

#include "../ANSI-Library/ansi_lib.h"

namespace fs = std::filesystem;

enum class Compile_Tool
{
    VISUAL_STUDIO_2022,
    VISUAL_STUDIO_2019,
    VISUAL_STUDIO_2017,
    VISUAL_STUDIO_2015,
    MAKEFILE,
    MINGW_MAKEFILE,
    INVALID
};

struct Info
{
    std::string list_folder_path;
    std::string build_folder_path;
    Compile_Tool compile_tool;
};

void help();
char *const_char_to_char(const char *str);
bool in_folder(const char *filename, std::string folder_path, std::string &file_path);
Compile_Tool convert_compile_tool(const char *compile_tool);
bool read_config(const char *file_path, Info &details);
std::string get_compile_tool(Compile_Tool tool);
std::string find_file_with_extension(std::string extension, std::string dir_path);
int compile(Info details, std::string base_path);
void create_details(std::string dir_path, std::string file_name, Info details);

int main(int argc, const char *argv[])
{
    std::string current_dir = fs::current_path().string();
    std::string config_file_name = "cmake-build-tool-config.txt";

    if (argc > 1 && strcmp(strlwr(const_char_to_char(argv[1])), "--help") == 0)
    {
        help();
        return 0;
    }
    

    
    if (argc == 1)
    {
        std::string file_path;

        if (!in_folder(config_file_name.c_str(), current_dir, file_path))
        {
            help();
            return -1;
        }

        Info details;

        if(!read_config(file_path.c_str(), details))
            return -1;

        return compile(details, current_dir);
    }

    if (argc != 4)
    {
        help();
        return -1;
    }


    Info details;

    details.list_folder_path = std::string(argv[1]);
    details.build_folder_path = std::string(argv[2]);
    details.compile_tool = convert_compile_tool(strlwr(const_char_to_char(argv[3])));

    int res = compile(details, current_dir);

    if(res != 0)
        return res;

    create_details(current_dir, config_file_name, details);
}

void help()
{
#ifdef _WIN32
    setupConsole();
#endif

    SET_8_VALUE_COLOUR(TXT_RED);
    printf("========= CMAKE-BUILD-TOOL =========\n");

    MOVE_CURSOR_BEGINNING_LINE_DOWN_BY_LINES(1);

    SET_GRAPHIC_MODE(BOLD_MODE);
    SET_8_VALUE_COLOUR(TXT_WHITE);

    printf("COMMANDS:\n");

    SET_GRAPHIC_MODE(BOLD_MODE_RESET);

    printf("   - cmake-build-tool ");

    SET_GRAPHIC_MODE(DIM_MODE);
    printf("--help ");
    SET_GRAPHIC_MODE(DIM_MODE_RESET);
    printf(": Shows this menu\n");

    MOVE_CURSOR_BEGINNING_LINE_DOWN_BY_LINES(1);

    printf("   - cmake-build-tool ");
    
    SET_GRAPHIC_MODE(DIM_MODE);
    printf("<source-folder> <build-folder> <compile-tool> ");
    SET_GRAPHIC_MODE(DIM_MODE_RESET);

    printf(": Compiles a CMAKE project\n");

    SET_GRAPHIC_MODE(DIM_MODE);
    printf("      - <source-folder> : the folder containing the CMakeLists.txt file\n");
    printf("      - <build-folder> : the folder where the final build will be stored in\n");
    printf("      - <compile-tool> : Which compiler will be used for the project\n");

    SET_8_VALUE_COLOUR(TXT_WHITE);
    SET_GRAPHIC_MODE(DIM_MODE_RESET);

    MOVE_CURSOR_BEGINNING_LINE_DOWN_BY_LINES(1);

    SET_GRAPHIC_MODE(BOLD_MODE);
    SET_8_VALUE_COLOUR(TXT_GREEN);

    printf("SUPPORTED COMPILE TOOLS:\n");

    SET_GRAPHIC_MODE(BOLD_MODE_RESET);

    printf("   - Visual-Studio-2022\n");
    printf("   - Visual-Studio-2019\n");
    printf("   - Visual-Studio-2017\n");
    printf("   - Visual-Studio-2015\n");
    printf("   - Makefile\n");
    printf("   - MinGW-Makefile\n");

    SET_GRAPHIC_MODE(BOLD_MODE);

    printf("Note: ");

    SET_GRAPHIC_MODE(BOLD_MODE_RESET);

    printf("The argument for compile tools isn't case sensitive");

    resetConsole();
}

char *const_char_to_char(const char *str)
{
    char *out_str = (char*)malloc(strlen(str) * sizeof(char));

    strcpy(out_str, str);

    return out_str;
}

bool in_folder(const char *filename, std::string folder_path, std::string &file_path)
{
    folder_path.append("\\");
    folder_path.append(filename);

    if(fs::exists(folder_path))
    {
        file_path = folder_path;
        return true;
    }

    file_path = "";

    return false;
}

Compile_Tool convert_compile_tool(const char *compile_tool)
{
    if(strcmp(compile_tool, "visual-studio-2022") == 0 || strcmp(compile_tool, "visual studio 17 2022") == 0)
    {
        return Compile_Tool::VISUAL_STUDIO_2022;
    }
    if(strcmp(compile_tool, "visual-studio-2019") == 0 || strcmp(compile_tool, "visual studio 16 2019") == 0)
    {
        return Compile_Tool::VISUAL_STUDIO_2019;
    }
    if(strcmp(compile_tool, "visual-studio-2017") == 0 || strcmp(compile_tool, "visual studio 15 2017") == 0)
    {
        return Compile_Tool::VISUAL_STUDIO_2017;
    }
    if(strcmp(compile_tool, "visual-studio-2015") == 0 || strcmp(compile_tool, "visual studio 14 2015") == 0)
    {
        return Compile_Tool::VISUAL_STUDIO_2015;
    }
    if(strcmp(compile_tool, "makefile") == 0 || strcmp(compile_tool, "unix makefiles") == 0)
    {
        return Compile_Tool::MAKEFILE;
    }
    if(strcmp(compile_tool, "mingw-makefile") == 0 || strcmp(compile_tool, "mingw makefiles") == 0)
    {
        return Compile_Tool::MINGW_MAKEFILE;
    }

    return Compile_Tool::INVALID;
}

bool read_config(const char *file_path, Info &details)
{
    std::ifstream in(file_path);

    std::getline(in, details.list_folder_path, '\n');
    std::getline(in, details.build_folder_path, '\n');
    
    std::string compile_tool_str;
    std::getline(in, compile_tool_str, '\n');

    std::cout<<details.list_folder_path<< " " << details.build_folder_path << " " << compile_tool_str << "\n";

    details.compile_tool = convert_compile_tool(strlwr(const_char_to_char(compile_tool_str.c_str())));

    if(details.compile_tool == Compile_Tool::INVALID)
    {
        perror("ERROR: Invalid Compile Tool");
        return false;
    }

    in.close();

    return true;
}

std::string get_compile_tool(Compile_Tool tool)
{
    switch(tool){
        case Compile_Tool::VISUAL_STUDIO_2022:
            return "Visual Studio 17 2022";
        case Compile_Tool::VISUAL_STUDIO_2019:
            return "Visual Studio 16 2019";
        case Compile_Tool::VISUAL_STUDIO_2017:
            return "Visual Studio 15 2017";
        case Compile_Tool::VISUAL_STUDIO_2015:
            return "Visual Studio 14 2015";
        case Compile_Tool::MAKEFILE:
            return "Unix Makefiles";
        case Compile_Tool::MINGW_MAKEFILE:
            return "MinGW Makefiles";
        default:
            return "";
    }
}

std::string find_file_with_extension(std::string extension, std::string dir_path){

    if(!fs::exists(dir_path) || !fs::is_directory(dir_path))
    {
        for (auto const & entry : fs::recursive_directory_iterator(dir_path))
        {
            if (entry.path().extension() == extension)
                return entry.path().stem().string();
        }
    }
    return "";
}   

int compile(Info details, std::string base_path)
{
    std::string compile_tool_cmd = get_compile_tool(details.compile_tool);

    std::string cmake_cmd = "cmake -S " + details.list_folder_path + " -B " + details.build_folder_path + " -G \"" + compile_tool_cmd + "\"";

    
    if(system(cmake_cmd.c_str()))
    {
        perror("ERROR: cmake command failed\n");
        return -1;
    }

    fs::current_path(base_path + "/" + details.build_folder_path);

    std::string compile_cmd;

    switch(details.compile_tool)
    {
        case Compile_Tool::VISUAL_STUDIO_2022:
        case Compile_Tool::VISUAL_STUDIO_2019:
        case Compile_Tool::VISUAL_STUDIO_2017:
        case Compile_Tool::VISUAL_STUDIO_2015:
            compile_cmd ="msbuild ./" + details.build_folder_path + "/" + find_file_with_extension(".sln", details.build_folder_path);
            if(system(compile_cmd.c_str()))
            {
                perror("ERROR: msbuild command could not be executed\n");
                return -1;
            }

            break;  
        case Compile_Tool::MAKEFILE:
            if(system("make"))
            {
                perror("ERROR: could not execute make command\n");
                return -1;
            }

            break;
        case Compile_Tool::MINGW_MAKEFILE:
            if(system("mingw32-make"))
            {
                perror("ERROR: could not execute mingw32-make command\n");
                return -1;
            }

            break;
    }

    return 0;
}

void create_details(std::string dir_path, std::string file_name, Info details)
{
    dir_path += "\\";
    dir_path += file_name;

    std::ofstream out(dir_path);

    out << details.list_folder_path << '\n';
    out << details.build_folder_path << '\n';
    out << get_compile_tool(details.compile_tool) << '\n';


    out.close();
}