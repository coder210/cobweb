local cobweb = require "cobweb"


cobweb.start(function ()
    cobweb.log("send")
    cobweb.send(".recv", "lua", "liwei")
    local ret = cobweb.call(".recv", "lua", "add", 1, 2)
    cobweb.log("ret=" .. ret)
    cobweb.exit()
end)