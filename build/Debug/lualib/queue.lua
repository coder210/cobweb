local queue = {}
function queue.new ()
    return {first = 0, last = -1}
end

function queue.eq (q, value)
    local first = q.first - 1
    q.first = first
    q[first] = value
end

function queue.length (q)
    if q.first > q.last then
        return 0
    elseif q.first == q.last then
        return 1
    else
        return q.last - q.first + 1
    end
end

function queue.dq(q)
    local last = q.last
    if q.first > last then 
        error("queue is empty") 
    end
    local value = q[last]
    q[last] = nil -- to allow garbage collection
    q.last = last - 1
    return value
end

return queue
