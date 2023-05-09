local cobweb = require "cobweb"


function add(a, b)
    return a + b
end
cobweb.start(function ()
    cobweb.name(".recv")
    cobweb.log("recv")
    cobweb.newservice("send")
    cobweb.dispatch("lua", function(session, address, ...)
         local t = {...}
         cobweb.log("session=" .. session .. " address=" .. address .. " t=" .. table.concat(t))
         if t[1] == "add" then
            local ret = add(tonumber(t[2]), tonumber(t[3]))
            local msg, sz = cobweb.pack(ret)
            cobweb.ret(msg, sz)
         end
    end)
end)