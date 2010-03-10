xml=[[
<?xml version="1.0" encoding="utf-8"?>
<window value="Hello FTK!" attrs="$FTK_ATTR_QUIT_WHEN_CLOSE">
</window>
]]

Ftk.Init(1, {"demo1"})
win=FtkXul.Load(xml, #xml)
win:ShowAll(1)
Ftk.Run()


