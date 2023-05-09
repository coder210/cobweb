local Vector3 = {x = 0, y = 0, z = 0}

--这里是构造方法
function Vector3:new(x,y,z)
   local o = {}
   setmetatable(o, self)
   self.__index = self

   o.x = x
   o.y = y
   o.z= z
end

--定义一个新的方法
function Vector3:go()
   self.x = self.x + 10
end

return Vector3