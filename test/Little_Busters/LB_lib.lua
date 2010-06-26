-- Library functions for Little Busters testing.

-- namespace
LB = { }

function LB:installMainMenuHandler ()
    state = 0
    World:addHandler(9012, 370, function ()
        if state == 0 then
            origin =
                System:graphics():getChildFgObject(171, 21):getClickPointHack()
            System:event():injectMouseMovement(origin)
            state = 1
        elseif state == 1 then
            System:event():injectMouseDown()
            state = 2
        elseif state == 2 then
            System:event():injectMouseUp()
            state = 3
        elseif state == 4 then
            origin =
                System:graphics():getChildFgObject(171, 26):getClickPointHack()
            System:event():injectMouseMovement(origin)
            state = 5
        elseif state == 5 then
            System:event():injectMouseDown()
            state = 6
        elseif state == 6 then
            System:event():injectMouseUp()
            state = 7
        end
    end)

    -- Once we've started a new game, the next interaction with the main menu
    -- should quit.
    World:addHandler(513, 51, function ()
        if state == 3 then
            state = 4
        end
    end)
end