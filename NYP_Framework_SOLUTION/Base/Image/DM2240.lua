function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

title = "DM2240 - Week 14 Scripting"
width = 1200
height = 600
PlayerPos_x = 100
PlayerPos_z = 200
keyFORWARD = "Z";




function Add(x,y)
    print(x+y)
	return x+y
end
