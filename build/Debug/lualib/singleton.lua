local singleton = {}

function singleton:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function singleton:instance()
    if self.inst == nil then
        self.inst = self:new()
    end
    return self.inst
end

return singleton