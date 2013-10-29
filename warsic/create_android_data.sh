#!/bin/sh 
#-xv
echo "Creating android data..."
PLATFORM=android
OUTPUT_FOLDER=data_android
OUTPUT_FOLDER_PACKAGES=android/assets

BASE_FOLDER=`pwd`
FULL_FOLDER=$BASE_FOLDER/$OUTPUT_FOLDER
FULL_FOLDER_PACKAGES=$BASE_FOLDER/$OUTPUT_FOLDER_PACKAGES

CLEAN_REBUILD=NO
BUILD_QUALITY=NORMAL
COPY_DATA=NO
PACKAGE_DATA=NO
VERSION=NORMAL

for var in "$@"
do
	if test "$var" = "clean" ; then
	CLEAN_REBUILD=YES
	fi
	if test "$var" = "fast" ; then
	BUILD_QUALITY=FAST
	fi
	if test "$var" = "final" ; then
	BUILD_QUALITY=FINAL
	fi
	if test "$var" = "copy" ; then
	COPY_DATA=YES
	fi
	if test "$var" = "chinese" ; then
	VERSION=CHINESE
	fi
	if test "$var" = "korean" ; then
	VERSION=KOREAN
	fi
	if test "$var" = "vietnam" ; then
	VERSION=VIETNAM
	fi
	if test "$var" = "package" ; then
	PACKAGE_DATA=YES
	fi
done

#if test $# -gt 0 ; then
#fi

echo "=========================================================="
printf "%-20s%s\n" "CLEAN_REBUILD"	$CLEAN_REBUILD
printf "%-20s%s\n" "BUILD_QUALITY"	$BUILD_QUALITY
printf "%-20s%s\n" "COPY_DATA"		$COPY_DATA
printf "%-20s%s\n" "PACKAGE_DATA"	$PACKAGE_DATA
printf "%-20s%s\n" "PLATFORM:"		$PLATFORM
printf "%-20s%s\n" "OUTPUT_FOLDER:"	$OUTPUT_FOLDER
printf "%-20s%s\n" "BASE_FOLDER:"	$BASE_FOLDER
printf "%-20s%s\n" "FULL_FOLDER:"	$FULL_FOLDER
printf "%-20s%s\n" "VERSION"		$VERSION
echo "=========================================================="

if test "$CLEAN_REBUILD" = "YES" ; then
# Clear old data
rm -r $OUTPUT_FOLDER
rm -r android
fi

# Copy all original data into data folder
echo "Copy data_org ..."
cp -rfp data_org/. $OUTPUT_FOLDER/

if test "$VERSION" = "CHINESE" ; then
	echo "Copy data_org_chinese ..."
	cp -rfp data_org_chinese/. $OUTPUT_FOLDER/
fi
if test "$VERSION" = "KOREAN" ; then
	echo "Copy data_org_korean ..."
	cp -rfp data_org_korean/. $OUTPUT_FOLDER/
fi
if test "$VERSION" = "VIETNAM" ; then
	echo "Copy data_org_vietnam ..."
	cp -rfp data_org_vietnam/. $OUTPUT_FOLDER/
fi

# Copy android specific data
#echo "Copy data_org_android ..."
#cp -rfp data_org_android/. $OUTPUT_FOLDER/

# Copy music files into data folder
#mkdir $OUTPUT_FOLDER/music
#cp -rfp music_org/small/*.mp3 $OUTPUT_FOLDER/music/
# Copy movie files into data folder
#mkdir $OUTPUT_FOLDER/movies
#rm $OUTPUT_FOLDER/movies/*.ivf
#if test "$VERSION" = "CHINESE" ; then
#	cp movies_org/*_yodo1.mp4 $OUTPUT_FOLDER/movies/
#elif test "$VERSION" = "KOREAN" ; then
#	cp movies_org/*_gravity.mp4 $OUTPUT_FOLDER/movies/
#else
#	cp movies_org/logo_1024_600.mp4 $OUTPUT_FOLDER/movies/
#fi

# Give warnings for all textures without tex files!
MISSING_TEX=NO
for var in `find ./$OUTPUT_FOLDER -name '*.png' -o -name '*.tga'`
do
if [ ! -e ${var%.*}.tex ]; then
	echo MISSING TEX FILE $var
	MISSING_TEX=YES
	#djtex $BASE_FOLDER/textures.desc $BASE_FOLDER/$PLATFORM.desc $var
fi
done

if test "$MISSING_TEX" = "YES" ; then
	#echo "Missing tex files... Abort!"

	# Check if uses wants to abort since we have missing tex files	
	while true
	do
	echo "Abort? (y or n) :"
	read CONFIRM
	case $CONFIRM in
	n|N|no|NO|No) break ;;
	y|Y|YES|yes|Yes)
	echo Aborting - you entered $CONFIRM
	exit
	;;
	*) echo Please enter only y or n
	esac
	done
	echo You entered $CONFIRM. Continuing ...	
fi

# Create dtx files
echo "Create DTX files..."
for var in `find ./$OUTPUT_FOLDER -name '*.tex'`
do
	djtex $BASE_FOLDER/textures.desc $BASE_FOLDER/$PLATFORM.desc $var QUALITY=$BUILD_QUALITY 
done
	
#echo `pwd`

cd $BASE_FOLDER

# Remove all image files except dtx
rm `find ./$OUTPUT_FOLDER -name '*.png'`
rm `find ./$OUTPUT_FOLDER -name '*.tga'`
# Remove all tex files
rm `find ./$OUTPUT_FOLDER -name '*.tex'`
# Remove uncached mesh files
rm `find ./$OUTPUT_FOLDER -name '*.mesh'`
# Remove uncached anim files
rm `find ./$OUTPUT_FOLDER -name '*.anim'`

#rm ./$OUTPUT_FOLDER/fonts/Sony_Sketch_EF.ttf

#if test "$VERSION" = "CHINESE" ; then
#rm ./$OUTPUT_FOLDER/fonts/DroidSansFallback.ttf
#else
#rm ./$OUTPUT_FOLDER/fonts/JZY.ttf
#fi

#if test "$VERSION" = "KOREAN" ; then
#rm ./$OUTPUT_FOLDER/fonts/JZY.ttf
#else
#rm ./$OUTPUT_FOLDER/fonts/DroidSansFallback.ttf
#fi

if test "$PACKAGE_DATA" = "YES" ; then
echo "Packaging data..."
cp -rfp android_org/. android/

#if test "$VERSION" = "KOREAN" ; then
#cp -rfp android_org_korean/. android/
#fi

#if test "$VERSION" = "CHINESE" ; then
#cp -rfp android_org_chinese/. android/
#fi

cd $OUTPUT_FOLDER
#rm *.djz
#zip arch0.djz -rq animations/ effects/ fonts/ materials/ shaders/ text/ ui/
#zip arch1.djz -rq sound/
#zip arch2.djz -rq textures/ backgrounds/ sprites/
#zip arch3.djz -rq objects/
#zip arch4.djz -rq enemies/ enemies_phone/ levels/ levels_phone/
#zip arch5.djz -rq story/
#zip obb.djz -rq animations/ effects/ fonts/ materials/ shaders/ text/ ui/ sound/ textures/ backgrounds/ sprites/ objects/ enemies/ enemies_phone/ levels/ levels_phone/ story/
#zip obb.djz -rq0 movies/
#zip obb.djz -rq0 music/
#rm -r $FULL_FOLDER_PACKAGES
#mkdir $FULL_FOLDER_PACKAGES
#mv *.djz $FULL_FOLDER_PACKAGES
#cp -rfp ../music_org/small/ $FULL_FOLDER_PACKAGES/music/
cp -rfp * $FULL_FOLDER_PACKAGES
rm -r $FULL_FOLDER_PACKAGES/movies
cd $BASE_FOLDER
fi

# Copy data folder if selected
if test "$COPY_DATA" = "YES" ; then
	echo "Copying data..."
	#if test "$PACKAGE_DATA" = "YES" ; then
	#	rm -r data
	#	cp -r $OUTPUT_FOLDER_PACKAGES data
	#	#rm -r movies
	#	#cp -rfp movies_org/ movies/
	#else
	rm -r data
	cp -r $OUTPUT_FOLDER data
		#rm -r movies
		#cp -rfp movies_org/ movies/
	#fi
	# Android specific removal
	#rm movies/*_yodo1.mp4
	#rm movies/*_gravity.mp4
	#rm movies/attract*.*
	#rm movies/*_ios.mp4
	#rm movies/*_iphone*.mp4
	#rm movies/*.ivf
	#rm movies/*.bat
	#rm movies/*.3gp
	#rm movies/*.webm
	#rm movies/*.wav
fi

echo "Done!"
