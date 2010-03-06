:%s/\<FtkGlyph\*/FtkGlyphPtr/g
:%s/\<FtkFont\*/FtkFontPtr/g
:%s/\<FtkColor\*/FtkColorPtr/g
:%s/\<FtkBitmap\*/FtkBitmapPtr/g
:%s/\<FtkGc\*/FtkGcPtr/g
:%s/\<FtkCanvas\*/FtkCanvasPtr/g
:%s/\<const char\*/CStrPtr/g
:%s/\<char\*/StrPtr/g
:%s/\<FtkDestroy\*/FtkDestroy/g
:%s/\<FtkEvent\*/FtkEventPtr/g
:%s/\<FtkWidget\*/FtkWidgetPtr/g
:%s/\<FtkListener\*/FtkListener/g
:%s/\<FtkTranslatePath\*/FtkTranslatePath/g
:%s/\<FtkLoadImage\*/FtkLoadImage/g
:%s/\<FtkTranslateText\*/FtkTranslateText/g
:%s/\<FtkDisplay\*/FtkDisplayPtr/g
:%s/\<FtkMainLoop\*/FtkMainLoopPtr/g
:%s/\<FtkWndManager\*/FtkWndManagerPtr/g
:%s/\<FtkBitmapFactory\*/FtkBitmapFactoryPtr/g
:%s/\<FtkSourcesManager\*/FtkSourcesManagerPtr/g
:%s/\<FtkTheme\*/FtkThemePtr/g
:%s/\<FtkAllocator\*/FtkAllocatorPtr/g
:%s/\<FtkInputMethodManager\*/FtkInputMethodManagerPtr/g
:%s/\<FtkImPreeditor\*/FtkImPreeditorPtr/g
:%s/\<FtkImageDecoder\*/FtkImageDecoderPtr/g
:%s/\<FtkSource\*/FtkSourcePtr/g
:%s/\<FtkRect\*/FtkRectPtr/g
:%s/\<FtkIconCache\*/FtkIconCachePtr/g
:%s/\<FtkIconViewItem\*/FtkIconViewItemPtr/g
:%s/\<FtkIconViewItem\*\*/FtkIconViewItemPtr2/g
:%s/\<FtkListModel\*/FtkListModelPtr/g
:%s/\<FtkListRender\*/FtkListRenderPtr/g
:%s/\<void\*/voidPtr/g
:%s/(Ftk/(inout Ftk/g
:%s/, /, in /g
:wq
