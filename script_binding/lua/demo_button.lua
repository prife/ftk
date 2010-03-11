app_win=nil

function OnQuit(button)
	Ftk.Quit()

	return RET_OK
end

function OnHide(button)
	app_win:Lookup(100):Show(0)
	return RET_OK
end

function OnShow(button)
	app_win:Lookup(100):Show(1)
	return RET_OK
end

function OnClicked(button)
	print("Clicked")
	return RET_OK
end

function AppInit()
	win=FtkAppWindow.Create()
	win:SetAttr(FTK_ATTR_QUIT_WHEN_CLOSE)
	win:SetText("Demo button")
	app_win=win

	width=win:Width()
	height=win:Height()

	width = width/3 - 10;
	button=FtkButton.Create(win, 0, 30, width, 50)
	button:SetText("show")
	button:SetClickedListener("OnShow")

	button=FtkButton.Create(win, width + 10, 30, width, 50);
	button:SetText("hide")
	button:SetClickedListener("OnHide")

	button=FtkButton.Create(win, 2*(width + 10), 30, width, 50)
	button:SetText("按钮测试");
	button:SetId(100)
	button:SetClickedListener("OnClicked")

	button=FtkButton.Create(win, 0, 130, width, 40);
	button:SetClickedListener("OnClicked")

	button:SetText("yes")
	button:SetClickedListener("OnClicked")

	button=FtkButton.Create(win, 2*(width + 10), 130, width, 40);
	button:SetText("no")

	button=FtkButton.Create(win, width + 10, height/2, width, 60);
	button:SetText("Quit")
	button:SetClickedListener("OnQuit")

	win:SetFocus(button)
	win:ShowAll(1)

	return 1
end

Ftk.Init(1, {"demo button"})
AppInit()
Ftk.Run()

