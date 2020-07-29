
function fibonacci(n)
  if n >= 3 then
  	return fibonacci(n - 2) + fibonacci(n - 1)
  else
    return 1
  end
end

function main()
	print("Enter a number [1-45]: ")
	local n = io.read("*number")

	print("");
	local startTime = os.clock()
	local fib_n = fibonacci(n)
	local endTime = os.clock()

	print("fibonacci(40) = "..fib_n.."\n")
	print("Used time: "..(endTime - startTime).." seconds")
end

main()
