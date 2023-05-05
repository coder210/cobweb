local config = {}

config.root = "./"
config.project = "examples"
config.thread = 1
config.harbor = 1
config.afterend = 2
config.parse = 0
config.logservice = "logger"
config.address = "127.0.0.1:2526"
config.master = "127.0.0.1:2013"
config.start = "main"	-- main script
config.bootstrap = "bootstrap"	-- The service for bootstrap
config.luaservice = config.root.."service/?.lua;"
config.lualoader = config.root.."lualib/loader.lua"
config.lua_path = config.root.."lualib/?.lua;"..config.root.."lualib/?/init.lua;"..config.root..config.project.."/?.lua"
config.lua_cpath = config.root.."luaclib/?.so;"..config.root.."luaclib/?.dll"
config.cpath = config.root.."cservice/?.so;"..config.root.."cservice/?.dll"
--config.daemon = "./examples.pis"
config.daemon = nil

return config