from conans import ConanFile, CMake, tools
import os

class CppobserveTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_paths"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = "conan_paths.cmake"
        cmake.configure()
        cmake.build()

    def test(self):
        if not tools.cross_building(self.settings):
            if self.settings.compiler == "Visual Studio":
                if self.settings.build_type == "Release":
                    os.chdir("Release")
                else:
                    os.chdir("Debug")
            else:
                os.chdir("bin")
            self.run(".%sexample" % os.sep)
