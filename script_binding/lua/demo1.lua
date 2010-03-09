xml=[[
<?xml version="1.0" encoding="utf-8"?>
<window value="Hello FTK!">
</window>
]]

Ftk.Init(1, {"demo1"})
win=FtkXul.Load(xml, #xml)
FtkWindow.SetFullscreen(win, 1)
win:ShowAll(1)
Ftk.Run()


