from conans import ConanFile, CMake


class CppobserveConan(ConanFile):
    name = "cpp-observe"
    version = "0.1.0"
    license = "MIT"
    generators = "cmake_paths"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "A templated observer for modern C++"
    exports_sources = "../*"

    def build(self):
        cmake = CMake(self)
        self.run('conan install %s --build missing' % self.source_folder)
        self.run('cmake %s -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake %s' % (self.source_folder, cmake.command_line))
        cmake.install()

    def package(self):
        self.copy("*.hpp", dst="include/observe", src="source/library/include/observe")
        self.copy("cpp_observeConfig.cmake", dst="", src="")
        self.copy("cpp_observeConfigVersion.cmake", dst="", src="")
        self.copy("cpp_observeTargets.cmake", dst="", src="")
        self.copy("LICENSE", dst="", src="")

    def package_id(self):
        self.info.header_only()
