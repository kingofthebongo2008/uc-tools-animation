// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <cstdint>
#include <iostream>
#include <filesystem>

#include <uc/import/anm/animation.h>
#include <uc/lzham/lzham_compressor.h>

#include "uc_animation_command_line.h"
#include "uc_animation_options.h"
#include "uc_animation_lip.h"
#include "uc_animation_animation.h"


#include <uc/import/fbx/animation.h>

#include "pch.h"

inline std::ostream& operator<<(std::ostream& s, const std::string& str)
{
    s << str.c_str();
    return s;
}

inline std::string get_environment()
{
    return "x64";
}


int32_t main(int32_t argc, const char* argv[])
{
    using namespace uc::animation;
    
    try
    {
        auto&& om = build_option_map(argc, argv);
        auto&& vm = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        std::cout << "Command line:" << std::endl;
        for (auto i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        auto input_animation    = get_input_animation(vm);
        auto output_animation   = get_output_animation(vm);

        std::cout << "building animation input (" << input_animation << ") output ( " << output_animation << " )" << std::endl;

        auto make_left_handed = get_make_left_handed(vm);

        std::filesystem::path path(input_animation);
        auto e = path.extension().wstring();

        std::vector<uc::gx::import::anm::joint_animations> animations;

        if (e == L".fbx")
        {
            animations = uc::gx::import::fbx::animation::create_animations(input_animation);
        }

        {
            auto&& a = animations.front();
            //take the 1st one only
            uc::lip::serialize_object( uc::animation::animation(a), output_animation, uc::lzham::compress_buffer);
        }
    }
    
    catch (const std::exception& e)
    {
        std::string  input_model = "uc_dev_animation_r.exe";
        std::cerr << input_model << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

