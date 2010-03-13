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
	win:SetText("Expanding")
	width=win:Width()
	height=win:Height()

	button=FtkButton.Create(win, 0, height/3, width, 60);
	button:SetClickedListener("OnClose")
	button:SetText("Close");
	
	ani=FtkAnimatorExpand.Create(1)
	if type == FTK_ANI_TO_RIGHT then
		ani:SetParam(type, 100, width, width/8, 200);
	else
		ani:SetParam(type, height - 100, win:Top(), height/6, 200);
	end
	ani:Start(win, 0)

	return RET_OK
end

function OnRight(button)
	CreateAnimateWin(FTK_ANI_TO_RIGHT)
	return RET_OK
end

function OnUp(button)
	CreateAnimateWin(FTK_ANI_TO_UP)
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
	button:SetText("Right")
	button:SetClickedListener("OnRight")

	button=FtkButton.Create(win, 2*width/3, 30, width/3, 50);
	button:SetText("Up")
	button:SetClickedListener("OnUp")
	
	button=FtkButton.Create(win, width/3, height/2, width/3, 50);
	button:SetText("Quit")
	button:SetClickedListener("OnQuit")

	win:ShowAll(1)

	return 1
end

Ftk.Init(1, {"alpha"})
AppInit()
Ftk.Run()

