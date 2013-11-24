-- important paths
sourcePath = "src/"
objectPath = "obj/"
testPath = "src/test/"
testObjectPath = "obj/test/"

-- set up compiler settings
settings = NewSettings()

-- full warning level, treat warnings as errors
settings.cc.flags:Add("-Wall -Wextra -Werror -Wno-unused-function")
-- use fPIC for making a shared object
settings.cc.flags:Add("-fPIC")

-- output objects to obj directory
ExecuteSilent("mkdir obj")
settings.cc.Output = function(settings, input)
	return objectPath .. PathFilename(PathBase(input))
end

-- collect sources and compile
source = Collect(sourcePath .. "*.c")
objects = Compile(settings, source)

-- make shared objects
libpotency = SharedLibrary(settings, "libpotency", objects)

-- build test binary
settings = NewSettings()
-- full warning level, treat warnings as errors
settings.cc.flags:Add("-Wall -Wextra -Werror -Wno-unused-function")
-- use fPIC for making a shared object
settings.cc.flags:Add("-fPIC")
settings.cc.includes:Add(sourcePath)
settings.link.libpath:Add(".")
settings.link.libs:Add("potency")

-- output objects to obj directory
ExecuteSilent("mkdir obj/test")
settings.cc.Output = function(settings, input)
	return testObjectPath .. PathFilename(PathBase(input))
end

source = Collect(testPath .. "*.c")
objects = Compile(settings, source)
exe = Link(settings, "test_potency", objects)