from conans import ConanFile, CMake, tools
import os

class CppobserveTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_paths"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("Release")
            self.run(".%sexample" % os.sep)
