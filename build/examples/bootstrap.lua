local cobweb = require "cobweb"
local cjson = require "cjson"

cobweb.start(function ()
    cobweb.log("boostrap start")
    local str = cjson.encode({
        name = "liwei",
        age = 10
    })
    cobweb.log(str)
    cobweb.newservice("recv")
    cobweb.exit()
end)