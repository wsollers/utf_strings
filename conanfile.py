
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMakeDeps, CMakeToolchain

class UtfStrings(ConanFile):
    name = "utf_strings"
    version = "0.0.2"
    settings = "os", "arch", "compiler", "build_type"
    package_type = "application"
    exports = "LICENSE"
    options = {"with_gperftools": [True, False]}
    default_options = {
        "with_gperftools": True,
        "*:cppstd": "23"
    }

    def requirements(self):
        self.requires("gtest/1.14.0", run=True)
        self.requires("benchmark/1.9.4", run=True)
        if self.options.with_gperftools and self.settings.os != "Windows":
            self.requires("gperftools/2.13.0", run=True)

    def generate(self):
        tc = CMakeToolchain(self); tc.generate()
        deps = CMakeDeps(self); deps.generate()

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.cpp.source.includedirs = ["include"]
        self.cpp.source.srcdirs = ["src"]

    def build(self): pass

    def package(self):
        copy(self, "LICENSE", self.source_folder, self.package_folder)
