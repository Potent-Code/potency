-- important paths
sourcePath = "src/"
objectPath = "obj/"

-- set up compiler settings
settings = NewSettings()

-- full warning level, treat warnings as errors
settings.cc.flags:Add("-Wall -Wextra -Werror -Wno-unused-function")

-- output objects to obj directory
ExecuteSilent("mkdir obj")
settings.cc.Output = function(settings, input)
	return objectPath .. PathFilename(PathBase(input))
end

-- collect sources and compile
source = Collect(sourcePath .. "*.c")
objects = Compile(settings, source)
