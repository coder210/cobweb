local cobweb = require "cobweb"
local gate = require "gate"


function add(a, b)
    return a + b
end 

cobweb.start(function ()
    cobweb.name(".recv")
    gate.open()
    cobweb.log("recv")

    cobweb.dispatch("lua", function(session, address, ...)
         local t = {...}
         cobweb.log("session=" .. session .. " address=" .. address .. " t=" .. table.concat(t))
         if t[1] == "add" then
            local ret = add(tonumber(t[2]), tonumber(t[3]))
            local msg, sz = cobweb.pack(ret)
            cobweb.ret(msg, sz)
         end
    end)

    cobweb.dispatch("socket", function(session, address, type, fd, buffer, sz)
         if type == cobweb.STYPE_ACCEPT then
             cobweb.log("accept=" .. fd)
         elseif type == cobweb.STYPE_DATA then
             cobweb.log("recv msg =" .. data .. " sz=" .. sz)
         elseif type == cobweb.STYPE_CLOSE then
             cobweb.log("close=" .. fd)
         end
    end)
end)