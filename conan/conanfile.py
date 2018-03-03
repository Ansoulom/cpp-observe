from conans import ConanFile


class CppobserveConan(ConanFile):
    name = "cpp-observe"
    version = "0.1.0"
    license = "MIT"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "A templated observer for modern C++"
    #generators = "cmake"
    exports_sources = "../*"

    #def build(self):
        #cmake = CMake(self)
        #cmake.configure(source_folder="")
        #cmake.build()

        # Explicit way:
        # self.run('cmake %s/src %s' % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include/observe", src="source/library/include/observe")
        self.copy("LICENSE", dst="", src="")

    def package_id(self):
        self.info.requires.clear()
