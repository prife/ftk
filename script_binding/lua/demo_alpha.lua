app_win=nil

function OnQuit(button)
	print("OnQuit: " .. button:GetText())
	Ftk.Quit()

	return RET_OK
end

function OnClose(button)
	win=button:Toplevel();
	win:Unref()

	return RET_OK
end

function CreateAnimateWin(type)
	win=FtkAppWindow.Create()
	win:SetText("Alpha")
	width=win:Width()
	height=win:Height()
	
	button=FtkButton.Create(win, width/3, height/3, width/3, 60)
	button:SetClickedListener("OnClose")
	button:SetText("Close")

	ani=FtkAnimatorAlpha.Create(1)
	if type == 0 then
		delta=20
		duration=1000
		ani:SetParam(0, 0, 0xff, 20, 1000);
	else
		delta=60
		duration=200
		ani:SetParam(0, 0, 0xff, 60, 200);
	end
	ani:Start(win, 0)

	return RET_OK
end

function OnFast(button)
	CreateAnimateWin(1)
	return RET_OK
end

function OnSlow(button)
	CreateAnimateWin(0)
	return RET_OK
end

function AppInit()
	win=FtkAppWindow.Create()
	win:SetText("Demo alpha")
	win:SetAttr(FTK_ATTR_QUIT_WHEN_CLOSE)
	
	app_win=win
	width=win:Width()
	height=win:Height()

	button=FtkButton.Create(win, 0, 30, width/3, 50)
	button:SetText("Fast")
	button:SetClickedListener("OnFast")

	button=FtkButton.Create(win, 2*width/3, 30, width/3, 50);
	button:SetText("Slow")
	button:SetClickedListener("OnSlow")
	
	button=FtkButton.Create(win, width/3, height/2, width/3, 50);
	button:SetText("Quit")
	button:SetClickedListener("OnQuit")

	win:ShowAll(1)

	return 1
end

Ftk.Init(1, {"alpha"})
AppInit()
Ftk.Run()

