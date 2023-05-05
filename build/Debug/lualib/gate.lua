local c = require "lcobweb"
local cobweb = require "cobweb"

local gate = {}

do
	local REG = cobweb.register_protocol
	REG {
        name = "socket",
        id = cobweb.PTYPE_SOCKET,
        pack = cobweb.pack,
        unpack = cobweb.socket_unpack,
    }
end


function gate.open()
	c.send(".gate", cobweb.PTYPE_CLIENT, 0, "")
end

function gate.unpack(socket_msg)
    local type, fd, buffer, sz = c.socket_unpack(socket_msg)
    return type, fd, buffer, sz
end

function gate.send(fd, buffer, sz)
    local ret  = c.socket_send(fd, buffer, sz)
    return ret
end

return gate