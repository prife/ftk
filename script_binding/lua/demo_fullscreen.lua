app_win=nil

function OnQuit(button)
	print("OnQuit: " .. button:GetText())
	Ftk.Quit()

	return RET_OK
end

function OnFullscreen(button)
	app_win:SetFullscreen(1)	
	return RET_OK
end

function OnUnfullscreen(button)
	app_win:SetFullscreen(0)	
	return RET_OK
end

function AppInit()
	win=FtkAppWindow.Create()
	win:SetText("Demo button")
	win:SetAttr(FTK_ATTR_QUIT_WHEN_CLOSE)
	
	app_win=win
	width=win:Width()
	height=win:Height()

	button=FtkButton.Create(win, 0, 30, width/3, 50)
	button:SetText("全屏")
	button:SetClickedListener("OnFullscreen")

	button=FtkButton.Create(win, 2*width/3, 30, width/3, 50);
	button:SetText("正常")
	button:SetClickedListener("OnUnfullscreen")
	
	button=FtkButton.Create(win, width/3, height/2, width/3, 50);
	button:SetText("Quit")
	button:SetClickedListener("OnQuit")

	win:ShowAll(1)

	return 1
end

Ftk.Init(1, {"demo button"})
AppInit()
Ftk.Run()

