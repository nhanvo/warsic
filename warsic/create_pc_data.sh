#!/bin/sh 
#-xv
echo "Creating pc data..."
PLATFORM=pc
OUTPUT_FOLDER=data_pc
OUTPUT_FOLDER_PACKAGES=data_pc_packaged

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
fi

# Copy all original data into data folder
echo "Copy data_org ..."
cp -rfp data_org/. $OUTPUT_FOLDER/

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

if test "$PACKAGE_DATA" = "YES" ; then
echo "Packaging data..."
cd $OUTPUT_FOLDER
rm *.djz
zip arch0.djz -rq animations/ fonts/ materials/ shaders/ text/ ui/
zip arch1.djz -rq sound/ music /
zip arch2.djz -rq textures/ backgrounds/ sprites/
zip arch3.djz -rq enemies /levels/ cutscenes / players /

rm -r $FULL_FOLDER_PACKAGES
mkdir $FULL_FOLDER_PACKAGES
mv *.djz $FULL_FOLDER_PACKAGES
cd $BASE_FOLDER
fi

# Copy data folder if selected
if test "$COPY_DATA" = "YES" ; then
	echo "Copying data..."
	if test "$PACKAGE_DATA" = "YES" ; then
		rm -r data
		cp -r $OUTPUT_FOLDER_PACKAGES data
		rm -r movies
		cp -rfp movies_org/ movies/
	else
		rm -r data
		cp -r $OUTPUT_FOLDER data
	fi
fi

echo "Done!"
