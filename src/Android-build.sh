ANDROID_ROOT=/work/android/froyo/
cd $ANDROID_ROOT;. build/envsetup.sh;cd -
ONE_SHOT_MAKEFILE=$PWD/Android.mk make -C $ANDROID_ROOT files

