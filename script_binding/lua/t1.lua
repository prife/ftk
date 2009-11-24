local xml="<?xml version=\"1.0\" encoding=\"utf-8\"?><window value=\"Entry Label\">	<label  id=\"1\" x=\"5\" y=\"5\" w=\"$ww/4\" h=\"30\" value=\"Name\" />	<entry  id=\"2\" x=\"$ww/4+5\" y=\"5\" w=\"3*$ww/4-15\" h=\"30\" value=\"Li XianJing\" />	<label  id=\"3\" x=\"5\" y=\"40\" w=\"$ww/4\" h=\"30\" value=\"EMail\" />	<entry  id=\"4\" x=\"$ww/4+5\" y=\"40\" w=\"3*$ww/4-15\" h=\"30\" value=\"xianjimli@hotmail.com\" />		<label  id=\"5\" x=\"5\" y=\"75\" w=\"$ww/4\" h=\"30\" value=\"Mobile\" />	<entry  id=\"6\" x=\"$ww/4+5\" y=\"75\" w=\"3*$ww/4-15\" h=\"30\" value=\"+8613911112222\" />		<button id=\"99\" x=\"5\" y=\"3*$wh/4\" w=\"$ww/2-5\" h=\"50\" value=\"Save\" />	<button id=\"100\" x=\"$ww/2\" y=\"3*$wh/4\" w=\"$ww/2-5\" h=\"50\" value=\"Quit\" /></window>"

function button_quit_on_clicked(button)
	ftk_quit()
	print("button_quit_on_clicked")
	return 0
end

function button_save_on_clicked(button)
	print("button_save_on_clicked")
	return 0
end

ftk_init()

win=ftk_xul_load(xml)
ftk_widget_show_all(win, 1)

quit=ftk_widget_lookup(win, 99)
ftk_button_set_clicked_listener(quit, "button_save_on_clicked")

quit=ftk_widget_lookup(win, 100)
ftk_button_set_clicked_listener(quit, "button_quit_on_clicked")

ftk_run()


