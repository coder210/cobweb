local c = require "lcobweb"
local tostring = tostring
local tonumber = tonumber
local coroutine = coroutine
local assert = assert
local pairs = pairs
local pcall = pcall

local coroutine_resume = coroutine.resume
local coroutine_yield = coroutine.yield


local proto = {}
local cobweb = {
	PTYPE_TEXT = 0,
	PTYPE_RESPONSE = 1,
	PTYPE_MULTICAST = 2,
	PTYPE_CLIENT = 3,
	PTYPE_SYSTEM = 4,
	PTYPE_HARBOR = 5,
	PTYPE_SOCKET = 6,
	PTYPE_ERROR = 7,
	PTYPE_QUEUE = 8,
	PTYPE_DEBUG = 9,
	PTYPE_LUA = 10,
	PTYPE_SNAX = 11,

	STYPE_DATA = 101, -- socket type
	STYPE_CONNECT = 102,
	STYPE_CLOSE = 103,
	STYPE_ACCEPT = 104,
	STYPE_ERROR = 105,
	STYPE_WARNING = 106
}

cobweb.tointeger = c.tointeger

function cobweb.error(msg)
	local currentlineInfo = debug.getinfo(2, "l")
	local sourceInfo = debug.getinfo(2, "S")
	c.error(sourceInfo.short_src .. ":" .. currentlineInfo.currentline .. " " .. tostring(msg))
end

function cobweb.log(msg)
	local currentlineInfo = debug.getinfo(2, "l")
	local sourceInfo = debug.getinfo(2, "S")
	c.log(sourceInfo.short_src .. ":" .. currentlineInfo.currentline .. " " .. tostring(msg))
end

function cobweb.register_protocol(class)
	local name = class.name
	local id = class.id
	assert(proto[name] == nil)
	assert(type(name) == "string" and type(id) == "number" and id >=0 and id <=255)
	proto[name] = class
	proto[id] = class
end

local session_id_coroutine = {}
local session_coroutine_id = {}
local session_coroutine_address = {}
local session_response = {}
local unresponse = {}

local wakeup_session = {}
local sleep_session = {}

local dead_service = {}
local error_queue = {}
local fork_queue = {}


local function _error_dispatch(error_session, error_source)
	if error_session == 0 then
		
	else
		table.insert(error_queue, error_session)
	end
end


-- suspend is function
local suspend

-- coroutine reuse

local coroutine_pool = setmetatable({}, { __mode = "kv" })

local function co_create(f)
	local co = table.remove(coroutine_pool)
	if co == nil then
		co = coroutine.create(function(...)
			--函数执行完毕
			f(...)
			while true do
				f = nil
				coroutine_pool[#coroutine_pool+1] = co
				--挂起协程
				f = coroutine_yield "EXIT"
				f(coroutine_yield())
			end
		end)
	else
		-- 如果消息类型不为response,那么将再次调用co_create().此时将执行这里
		coroutine_resume(co, f)
	end
	return co
end

local function dispatch_error_queue()
	local session = table.remove(error_queue,1)
	if session then
		local co = session_id_coroutine[session]
		session_id_coroutine[session] = nil
		return suspend(co, coroutine_resume(co, false))
	end
end

local function dispatch_wakeup()
	local co = next(wakeup_session)
	if co then
		wakeup_session[co] = nil
		local session = sleep_session[co]
		if session then
			session_id_coroutine[session] = "BREAK"
			return suspend(co, coroutine_resume(co, false, "BREAK"))
		end
	end
end

-- suspend is local function
function suspend(co, result, command, param, size)
	if not result then
		local session = session_coroutine_id[co]
		if session then -- coroutine may fork by others (session is nil)
			local addr = session_coroutine_address[co]
			if session ~= 0 then
				-- only call response error
				c.send(addr, cobweb.PTYPE_ERROR, session, "")
			end
			session_coroutine_id[co] = nil
			session_coroutine_address[co] = nil
		end
		error(debug.traceback(co,tostring(command)))
	end
	if command == "CALL" then
		session_id_coroutine[param] = co
	elseif command == "SLEEP" then
		session_id_coroutine[param] = co
		sleep_session[co] = param
	elseif command == "RETURN" then
		local co_session = session_coroutine_id[co]
		local co_address = session_coroutine_address[co]
		if param == nil or session_response[co] then
			error(debug.traceback(co))
		end
		session_response[co] = true
		local ret
		local session
		if not dead_service[co_address] then
			ret, session = c.send(co_address, cobweb.PTYPE_RESPONSE, co_session, param, size)
			if not ret then
				-- If the package is too large, returns nil. so we should report error back
				c.send(co_address, cobweb.PTYPE_ERROR, co_session, "")
			end
		elseif size ~= nil then
			c.trash(param, size)
			ret = false
		end
		return suspend(co, coroutine_resume(co, session))
	elseif command == "RESPONSE" then
		local co_session = session_coroutine_id[co]
		local co_address = session_coroutine_address[co]
		if session_response[co] then
			error(debug.traceback(co))
		end
		local f = param
		local function response(ok, ...)
			if ok == "TEST" then
				if dead_service[co_address] then
					unresponse[response] = nil
					f = false
					return false
				else
					return true
				end
			end
			if not f then
				if f == false then
					f = nil
					return false
				end
				error "Can't response more than once"
			end

			local ret
            local session
			if not dead_service[co_address] then
				if ok then
					ret, session = c.send(co_address, cobweb.PTYPE_RESPONSE, co_session, f(...))
					if not ret then
						-- If the package is too large, returns false. so we should report error back
						c.send(co_address, cobweb.PTYPE_ERROR, co_session, "")
					end
				else
					ret, session = c.send(co_address, cobweb.PTYPE_ERROR, co_session, "") ~= nil
				end
			else
				ret = false
			end
			unresponse[response] = nil
			f = nil
			return ret
		end

		session_response[co] = true
		unresponse[response] = true
		return suspend(co, coroutine_resume(co, response))
	elseif command == "EXIT" then
		-- coroutine exit
		local address = session_coroutine_address[co]
		session_coroutine_id[co] = nil
		session_coroutine_address[co] = nil
		session_response[co] = nil
	elseif command == "QUIT" then
		-- service exit
		return
	elseif command == "USER" then
		-- See cobweb.coutine for detail
		error("Call cobweb.coroutine.yield out of cobweb.coroutine.resume\n" .. debug.traceback(co))
	elseif command == nil then
		-- debug trace
		return
	else
		error("Unknown command : " .. command .. "\n" .. debug.traceback(co))
	end
	dispatch_wakeup()
	dispatch_error_queue()
end

function cobweb.wait(co)
	local session = c.genid()
	local ret, msg = coroutine_yield("SLEEP", session)
	co = co or coroutine.running()
	sleep_session[co] = nil
	session_id_coroutine[session] = nil
end

local function string_to_handle(str)
	return tonumber("0x" .. string.sub(str , 2))
end

local self_handle
function cobweb.self()
	if self_handle then
		return self_handle
	end
	self_handle = string_to_handle(c.command("REG"))
	return self_handle
end

function cobweb.localname(name)
	local addr = c.command("QUERY", name)
	if addr then
		return string_to_handle(addr)
	end
end

function cobweb.exit()
	fork_queue = {}	-- no fork coroutine can be execute after cobweb.exit
	for co, session in pairs(session_coroutine_id) do
		local address = session_coroutine_address[co]
		if session~=0 and address then
			c.redirect(address, 0, cobweb.PTYPE_ERROR, session, "")
		end
	end
	for resp in pairs(unresponse) do
		resp(false)
	end
	c.command("EXIT")
	-- quit service
	coroutine_yield "QUIT"
end

function cobweb.getenv(key)
	return c.command("GETENV",key)
end

function cobweb.setenv(key, value)
	c.command("SETENV",key .. " " ..value)
end

cobweb.genid = assert(c.genid)
cobweb.pack = assert(c.pack)
cobweb.packstring = assert(c.packstring)
cobweb.unpack = assert(c.unpack)
cobweb.tostring = assert(c.tostring)
cobweb.trash = assert(c.trash)

cobweb.redirect = function(dest, source, typename, ...)
	return c.redirect(dest, source, proto[typename].id, ...)
end

local function yield_call(service, session)
	local succ, msg, sz = coroutine_yield("CALL", session)
	if not succ then
		error "call failed"
	end
	return msg,sz
end

function cobweb.call(addr, typename, ...)
	local p = proto[typename]
	local ret, session = c.send(addr, p.id , nil , p.pack(...))
	if not ret then
		error("call to invalid address " .. cobweb.address(addr))
	end
	return p.unpack(yield_call(addr, session))
end

function cobweb.rawcall(addr, typename, msg, sz)
	local p = proto[typename]
	local ret, session = c.send(addr, p.id , nil , msg, sz)
	assert(ret, "call to invalid address")
	return yield_call(addr, session)
end

function cobweb.ret(msg, sz)
	msg = msg or ""
	return coroutine_yield("RETURN", msg, sz)
end

function cobweb.retpack(...)
	return cobweb.ret(cobweb.pack(...))
end

function cobweb.address(addr)
	if type(addr) == "number" then
		return string.format(":%08x",addr)
	else
		return tostring(addr)
	end
end

function cobweb.sleep(ti)
	local session = c.intcommand("TIMEOUT",ti)
	assert(session)
	local succ, ret = coroutine_yield("SLEEP", session)
	sleep_session[coroutine.running()] = nil
	if succ then
		return
	end
	if ret == "BREAK" then
		return "BREAK"
	else
		error(ret)
	end
end

function cobweb.yield()
	return cobweb.sleep(0)
end

----- register protocol
do
	local REG = cobweb.register_protocol

	REG {
		name = "lua",
		id = cobweb.PTYPE_LUA,
		pack = cobweb.pack,
		unpack = cobweb.unpack,
	}

	REG {
		name = "response",
		id = cobweb.PTYPE_RESPONSE
	}

	REG {
		name = "error",
		id = cobweb.PTYPE_ERROR,
		unpack = function(...) return ... end,
		dispatch = _error_dispatch,
	}
end

function cobweb.send(addr, typename, ...)
	local p = proto[typename]
	return c.send(addr, p.id, 0, p.pack(...))
end

function cobweb.rawsend(addr, typename, msg, sz)
	local p = proto[typename]
	return c.send(addr, p.id, 0, msg, sz)
end

function cobweb.timeout(ti, func)
	local session = c.intcommand("TIMEOUT",ti)
	assert(session)
	local co = co_create(func)
	assert(session_id_coroutine[session] == nil)
	session_id_coroutine[session] = co
end

function cobweb.name(aliasname)
	c.command("REG", aliasname)
end

function cobweb.newservice(name, ...)
	return cobweb.launch("luax", name)
end

function cobweb.queryservice(global, ...)
	if global == true then
		return assert(cobweb.call(".service", "lua", "GQUERY", ...))
	else
		return assert(cobweb.call(".service", "lua", "QUERY", global, ...))
	end
end

function cobweb.launch(...)
	local addr = c.command("LAUNCH", table.concat({...}," "))
	if addr then
		return tonumber("0x" .. string.sub(addr , 2))
	end
end

function cobweb.wakeup(co)
	if sleep_session[co] and wakeup_session[co] == nil then
		wakeup_session[co] = true
		return true
	end
end

function cobweb.dispatch(typename, func)
	local p = proto[typename]
	if func then
		local ret = p.dispatch
		p.dispatch = func
		return ret
	else
		return p and p.dispatch
	end
end

local function unknown_response(session, address, msg, sz)
	cobweb.error(string.format("Response message : %s" , c.tostring(msg,sz)))
	error(string.format("Unknown session : %d from %x", session, address))
end

local function unknown_request(session, address, msg, sz, prototype)
	cobweb.error(string.format("Unknown request (%s): %s", prototype, c.tostring(msg,sz)))
	error(string.format("Unknown session : %d from %x", session, address))
end

function cobweb.dispatch_unknown_request(unknown)
	local prev = unknown_request
	unknown_request = unknown
	return prev
end

function cobweb.fork(func,...)
	local args = table.pack(...)
	local co = co_create(function()
		func(table.unpack(args,1,args.n))
	end)
	table.insert(fork_queue, co)
	return co
end

local function raw_dispatch_message(prototype, msg, sz, session, source)
	-- cobweb.PTYPE_RESPONSE = 1, read lcobweb.h
	if prototype == 1 then
		local co = session_id_coroutine[session]
		if co == "BREAK" then
			session_id_coroutine[session] = nil
		elseif co == nil then
			unknown_response(session, source, msg, sz)
		else
			session_id_coroutine[session] = nil
			suspend(co, coroutine_resume(co, true, msg, sz))
		end
	else
		local p = proto[prototype]
		if p == nil then
			if session ~= 0 then
				c.send(source, cobweb.PTYPE_ERROR, session, "")
			else
				unknown_request(session, source, msg, sz, prototype)
			end
			return
		end
		local f = p.dispatch
		if f then
			local co = co_create(f)
			session_coroutine_id[co] = session
			session_coroutine_address[co] = source
			suspend(co, coroutine_resume(co, session,source, p.unpack(msg,sz)))
		else
			unknown_request(session, source, msg, sz, proto[prototype].name)
		end
	end
end

function cobweb.dispatch_message(...)
	--cobweb.error("dispatch_message")
	local succ, err = pcall(raw_dispatch_message,...)
	while true do
		local key,co = next(fork_queue)
		if co == nil then
			break
		end
		fork_queue[key] = nil
		local fork_succ, fork_err = pcall(suspend,co,coroutine_resume(co))
		if not fork_succ then
			if succ then
				succ = false
				err = tostring(fork_err)
			else
				err = tostring(err) .. "\n" .. tostring(fork_err)
			end
		end
	end
	assert(succ, tostring(err))
end

local init_func = {}

function cobweb.init(f, name)
	assert(type(f) == "function")
	if init_func == nil then
		f()
	else
		table.insert(init_func, f)
		if name then
			assert(type(name) == "string")
			assert(init_func[name] == nil)
			init_func[name] = f
		end
	end
end

local function init_all()
	local funcs = init_func
	init_func = nil
	if funcs then
		for _,f in ipairs(funcs) do
			f()
		end
	end
end

local function ret(f, ...)
	f()
	return ...
end

local function init_template(start, ...)
	init_all()
	init_func = {}
	return ret(init_all, start(...))
end

function cobweb.pcall(start, ...)
	return xpcall(init_template, debug.traceback, start, ...)
end

function cobweb.init_service(start)
	local ok, err = cobweb.pcall(start)
	if not ok then
		cobweb.error("init service failed: " .. tostring(err))
		--cobweb.send(".launcher","lua", "ERROR")
		cobweb.exit()
	else
		--cobweb.send(".launcher","lua", "LAUNCHOK")
	end
end

function cobweb.start(start_func, ...)
	c.callback(cobweb.dispatch_message)
	cobweb.timeout(0, function()
		cobweb.init_service(start_func)
	end)
end

return cobweb