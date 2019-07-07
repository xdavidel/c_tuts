a = 10 + 2 + math.sin(23.7)

player = {Name ="David", Age = 29, Level = 20}

function AddStuff( a, b )
	print("[LUA] The function AddStuff("..a..", "..b..") was called\n")
	return a + b
end

function HostCallMe( a, b )
	print("[LUA] The function HostCallMe("..a..", "..b..") was called\n")

	c = HostFunction(a, b)

	return c
end
