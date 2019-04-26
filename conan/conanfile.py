from conans import ConanFile, CMake


class CppobserveConan(ConanFile):
    name = "cpp-observe"
    version = "0.2.0"
    description = "A templated observer for modern C++"
    url = "https://github.com/Ansoulom/cpp-observe"
    license = "MIT"
    author = "Anton Södergren (anton.sodergren@gmail.com)"
    topics = ("observer", "signals", "slots")
    settings = None
    exports_sources = "../cmake*", "../source*", "!source/tests*", "../CMakeLists.txt", "../LICENSE"
    generators = "cmake_paths"
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTS"] = "OFF"
        cmake.configure()
        cmake.install()

    def package(self):
        self.copy("*", dst="", src="install")

    def package_id(self):
        self.info.header_only()
