
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

function Main()
	print(test_tab.name)
	print(test_tab.age)
	print(test_tab.weight)
	print(test_tab.height)

	local str = string.format("test_a = %d", test_a)
	print(str)

    --local n1 = test.newCounter()
    --local n2 = test.newCounter()
    --local n3 = test.counter()
    --local n4 = test.counter()
    --print(n1, n2, n3, n4)
    --print(n1, n3)

    local bytes = string_to_bytes(test_str)
    print("string_to_bytes = " .. bytes)
    str = bytes_to_string(bytes)
    print("bytes_to_string = " .. str)
end

Main()