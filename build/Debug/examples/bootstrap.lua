local cobweb = require "cobweb"

cobweb.start(function ()
    cobweb.log("boostrap")
    cobweb.newservice("recv")
    cobweb.newservice("send")
    cobweb.exit()
end)