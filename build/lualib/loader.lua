local args = { ... }

package.path = package.path .. ";" .. args[2]
package.cpath = package.cpath .. ";" .. args[3]

local project = args[4]
local lua_name = args[1]

local filename = "./" .. project .. "/" .. lua_name .. ".lua"
dofile(filename)
