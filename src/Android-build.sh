ANDROID_ROOT=/broncho/android-eclair2.1/
cd $ANDROID_ROOT;. build/envsetup.sh;cd -
ONE_SHOT_MAKEFILE=$PWD/Android.mk make -C $ANDROID_ROOT files
