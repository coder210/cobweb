local cobweb = require "cobweb"

cobweb.start(function ()
    cobweb.log("boostrap start")
    cobweb.newservice("center")
    cobweb.exit()
end)