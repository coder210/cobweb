local config = {}

config.root = "./"
config.project = "center"
config.thread = 2
config.logservice = "logger"
config.address = "127.0.0.1:30010"
config.bootstrap = "bootstrap"	-- The service for bootstrap
config.luaservice = config.root.."service/?.lua;"
config.lualoader = config.root.."lualib/loader.lua"
config.lua_path = config.root.."lualib/?.lua;"..config.root.."lualib/?/init.lua;"..config.root..config.project.."/?.lua"
config.lua_cpath = config.root.."luaclib/?.so;"..config.root.."luaclib/?.dll"
config.cpath = config.root.."cservice/?.so;"..config.root.."cservice/?.dll"

return config