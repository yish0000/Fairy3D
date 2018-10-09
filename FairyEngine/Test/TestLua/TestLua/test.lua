
local test_a = 555

local test_tab = 
{
	name = "xiaoming",
	age = 17,
	height = 170,
	weight = 140,
}

local test_str = "hello world!"

function string_to_bytes(str)
    local ret = ""
    for i=1, string.len(str) do
        ret = ret .. ("%02x"):format(str:byte(i))
    end
    return ret
end

function bytes_to_string(bytes)
    local str = ""
    for i=1, string.len(bytes), 2 do
        local char = string.sub(bytes, i, i + 1)
        local num = tonumber("0x"..char)
        str = str .. string.char(num)
    end
    return str
end

local function trim(s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

local function Main()
    --[[
	print(test_tab.name)
	print(test_tab.age)
	print(test_tab.weight)
	print(test_tab.height)

	local str = string.format("test_a = %d", test_a)
	print(str)
    ]]

    --local n1 = test.newCounter()
    --local n2 = test.newCounter()
    --local n3 = test.counter()
    --local n4 = test.counter()
    --print(n1, n2, n3, n4)
    --print(n1, n3)

    --[[
    local bytes = string_to_bytes(test_str)
    print("string_to_bytes = " .. bytes)
    str = bytes_to_string(bytes)
    print("bytes_to_string = " .. str)]]

    --local str = "       [p:1775, 1556, 16]  6    "
    --print(trim(str))

    --local s = "abc \"it\'s a cat\""
    --print(string.find(s, "^(%a*%s)(.-)$"))

    --print(string.gsub("hello, world", "(.-)(, )(.-)", "%2%3%1"))
    --print(string.gsub("hello, world", "l+", function(s) return "xxx" end))
    --for s in string.gfind(" a=b c=d ", "[^%s+]=[^%s+]") do print(s) end
    print(string.find("Hello, World!", "^(.-), %f[%u].-!$"))
    print(string.gsub("hello bill, (you eat)?", "%b()", "fuck you"))
    print(string.match("fflaaap", "^f+"))
    print(string.match("xw_2", "^[%l%d_]+$"))
end

do
    Main()
end