local cobweb = require "cobweb"
local cjson = require "cjson"

cobweb.start(function ()
    cobweb.log("boostrap start")
    local str = cjson.encode({
        name = "liwei",
        age = 10
    })
    cobweb.log(str)
    --cobweb.newservice("box2d")
    --cobweb.newservice("ev")
    --cobweb.newservice("pbtest")

    
    cobweb.newservice("adventure")

    --cobweb.newservice("tcpserver")
    --cobweb.launch("echoserver", address)
    --cobweb.newservice("recv")
    
    -- function f() 
    --     cobweb.log("ok")
    --     cobweb.timeout(100, f)
    -- end
    -- cobweb.timeout(100, f)
    
    cobweb.exit()
end)