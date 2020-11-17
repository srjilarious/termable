from conans import ConanFile, CMake

class TermableConan(ConanFile):
    name = "termable"
    version = "0.1"
    license = "MIT"
    author = "Jeff DeWall"
    url = "https://gitlab.com/sr.jilarious/termable"
    description = "A cross platform c++ terminal control library."
    topics = ("C++17", "argument", "option", "cli")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports_sources = "termable/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="termable")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["termable"]
