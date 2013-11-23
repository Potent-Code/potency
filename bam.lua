-- important paths
sourcePath = "./src/"
objectPath = "./obj/"

-- set up compiler settings
settings = NewSettings()

-- full warning level, treat warnings as errors
settings.cc.flags:Add("-Wall -Wextra -Werror")

-- output objects to obj directory
ExecuteSilent("mkdir obj")
settings.cc.Output = function(settings, input)
	return "./obj/"
end

-- collect sources and compile
source = Collect(sourcePath .. "*.c")
objects = Compile(settings, source)
