# Microsoft Developer Studio Project File - Name="ftk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ftk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ftk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ftk.mak" CFG="ftk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ftk - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ftk - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ftk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../.." /I "../../os/win32" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ftk" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ftk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../.." /I "../../os/windows" /I "../../im" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "VC6" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ftk - Win32 Release"
# Name "ftk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\fontdata.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_allocator.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_allocator_default.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_allocator_profile.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_animator_alpha.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_animator_expand.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_animator_zoom.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_app_window.c
# End Source File
# Begin Source File

SOURCE=..\..\backend\win32\ftk_backend_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_bitmap.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_bitmap_factory.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_button.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_canvas.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_check_button.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_clipboard.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_combo_box.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_config.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_dialog.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_display.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_display_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_display_rotate.c
# End Source File
# Begin Source File

SOURCE=..\..\backend\win32\ftk_display_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ftk_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_expr.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_font.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_font_default.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_font_desc.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_font_manager.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_gesture.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_globals.c
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_handwrite_engine_dummy.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_icon_cache.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_icon_view.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_image.c
# End Source File
# Begin Source File

SOURCE=..\..\os\windows\ftk_image_win32_decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_hw.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_input_method_manager.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_input_method_preeditor.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_input_method_preeditor_default.c
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_py.c
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_util.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_label.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_list_model_default.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_list_render_default.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_list_view.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_log.c
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_lookup_table.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_main_loop.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_main_loop_select.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_menu_item.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_menu_panel.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_message_box.c
# End Source File
# Begin Source File

SOURCE=..\..\os\windows\ftk_mmap_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_painter.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_pipe_socket.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_popup_menu.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_progress_bar.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_radio_group.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_scroll_bar.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_source_idle.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_source_primary.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_source_timer.c
# End Source File
# Begin Source File

SOURCE=..\..\backend\win32\ftk_source_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_sources_manager.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_sprite.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_status_item.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_status_panel.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_text_buffer.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_text_layout_normal.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_text_view.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_theme.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_translator.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_util.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_wait_box.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_widget.c
# End Source File
# Begin Source File

SOURCE=..\..\os\windows\ftk_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_window.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_wnd_manager.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_wnd_manager_default.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_xml_parser.c
# End Source File
# Begin Source File

SOURCE=..\..\ftk_xul.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\backend\win32\ftk_display_win32.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_handwrite_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\os\windows\ftk_image_win32_decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_hw.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_py.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_input_method_util.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_lookup_table.h
# End Source File
# Begin Source File

SOURCE=..\..\backend\win32\ftk_source_win32.h
# End Source File
# Begin Source File

SOURCE=..\..\im\ftk_stroke_painter.h
# End Source File
# Begin Source File

SOURCE=..\..\os\windows\ftk_win32.h
# End Source File
# End Group
# End Target
# End Project
