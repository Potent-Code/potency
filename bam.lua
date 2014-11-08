-- input paths
sourcePath = "src/"
testPath = "test/"
testObjectPath = "obj/test/"

-- output paths
outputPath = "build/"
objectPath = outputPath .. "obj/"
outputLibPath = outputPath .. "usr/local/lib/"
outputBinPath = outputPath .. "usr/local/bin/"
outputIncludePath = outputPath .. "usr/local/include/"


-- common settings for all Potency outputs
function NewPotencySettings()
	local potency_settings = NewSettings()

	-- use clang (TODO: bootstrap this properly with compiler detection)
	potency_settings.cc.exe_c = "clang"
	potency_settings.cc.exe_cxx = "clang++"
	potency_settings.link.exe = "clang"
	potency_settings.dll.exe = "clang"

	-- full warning level, treat warnings as errors, position independent code
	potency_settings.cc.flags:Add("-Wall", "-Wextra", "-Werror", "-fPIC")

	-- set optimization level
	if (potency_settings.debug > 0) then
	        potency_settings.cc.flags:Add("-O0")
	else
	        potency_settings.cc.flags:Add("-O3")
	end

	-- set version string
	potency_settings.cc.defines:Add("PACKAGE_VERSION=\\\"`git describe --always --tags --dirty=-modified --abbrev=10`-`git rev-parse --abbrev-ref HEAD`\\\"")

	-- output objects to obj directory
	potency_settings.cc.Output = function(potency_settings, input)
	        return objectPath .. PathFilename(PathBase(input))
	end

	return potency_settings
end

-- set up output directories
ExecuteSilent("mkdir -p " .. outputLibPath)
ExecuteSilent("mkdir -p " .. outputBinPath)
ExecuteSilent("mkdir -p " .. outputIncludePath)


-- set up compiler settings
settings = NewPotencySettings()

-- collect sources and compile
source = Collect(sourcePath .. "*.c")
objects = Compile(settings, source)

-- make shared objects
libpotency = SharedLibrary(settings, outputLibPath .. "libpotency", objects)

-- build test binary
settings = NewPotencySettings()

settings.cc.includes:Add(sourcePath)

-- compile test_potency test suite
source = Collect(testPath .. "*.c")
objects = Compile(settings, source)
exe = Link(settings, outputBinPath .. "test_potency", objects, libpotency)
