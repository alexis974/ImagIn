#!/bin/bash

download_raw()
{
	mkdir "../samples/Raw_files"

	declare -a img_name
	declare -a img_link
	declare -a img_exif
	img_name=(
		# Canon RAW file
		"Canon_80D.CR2"
		"Canon_6D.CR2"
		"Canon_6Dm2.CR2"
		"Canon_5Dm3.CR2"
		"Canon_1DXm2.CR2"

		#Nikon RAW file
		"Nikon_D7100_14bit_compressed(lossless).NEF"
		"Nikon_D7100_12bit_compressed(lossless).NEF"
		"Nikon_D750_14bit_compressed(lossless).NEF"
		"Nikon_D750_12bit_compressed(lossless).NEF"
		"Nikon_D850_14bit_uncompressed.NEF"

		# Canon DNG file
		"Canon_5Dm3_16bit_uncompressed.DNG"

		# Leica DNG file
		"Leica_M9_16bit_uncompressed.DNG"
		"Leica_M10_16bit_uncompressed.DNG"

		# Fujifilm RAW file
		"Fujifilm_GFX50S_14bit_uncompressed.RAF")

	img_link=(
		# Canon RAW file
		"https://raw.pixls.us/getfile.php/1294/nice/Canon%20-%20EOS%2080D%20-%20RAW%20(3:2).CR2"
		"https://raw.pixls.us/getfile.php/799/nice/Canon%20-%20EOS%206D%20-%20RAW.CR2"
		"https://raw.pixls.us/getfile.php/1625/nice/Canon%20-%20EOS%206D%20Mark%20II%20-%20RAW%20(3:2).CR2"
		"https://raw.pixls.us/getfile.php/771/nice/Canon%20-%20EOS%205D%20Mark%20III.CR2"
		"https://raw.pixls.us/getfile.php/1058/nice/Canon%20-%20EOS-1D%20X%20Mark%20II%20-%20RAW%20(3:2).CR2"

		#Nikon RAW file
		"https://raw.pixls.us/getfile.php/1855/nice/Nikon%20-%20D7100%20-%2014bit%20compressed%20(Lossless)%20(3:2).NEF"
		"https://raw.pixls.us/getfile.php/1856/nice/Nikon%20-%20D7100%20-%2012bit%20compressed%20(Lossless)%20(3:2).NEF"
		"https://raw.pixls.us/getfile.php/898/nice/Nikon%20-%20D750%20-%2014bit%20compressed%20(Lossless)%20(3:2).NEF"
		"https://raw.pixls.us/getfile.php/897/nice/Nikon%20-%20D750%20-%2012bit%20compressed%20(Lossless)%20(3:2).NEF"
		"https://raw.pixls.us/getfile.php/1842/nice/Nikon%20-%20D850%20-%2014bit%20uncompressed%20(3:2).NEF"

		# Canon DNG file
		"https://raw.pixls.us/getfile.php/1022/nice/Adobe%20DNG%20Converter%20-%20Canon%20EOS%205D%20Mark%20III%20-%2016bit%20uncompressed%20(3:2).DNG"

		# Leica DNG file
		"https://raw.pixls.us/getfile.php/752/nice/Leica%20-%20M9%20Digital%20Camera%20-%20uncompressed%2016bit.DNG"
		"https://raw.pixls.us/getfile.php/1603/nice/Leica%20-%20LEICA%20M10%20-%2016bit%20uncompressed%20(3:2).dng"

		# Fujifilm RAW file
		"https://raw.pixls.us/getfile.php/1436/nice/Fujifilm%20-%20GFX%2050S%20-%2014bit%20uncompressed%20(3:2).RAF")

	img_exif=(
		# Canon RAW file
		"https://raw.pixls.us/getfile.php/1294/exif/IMG_0098.CR2.exif.txt"
		"https://raw.pixls.us/getfile.php/799/exif/EOS_6D_RAW.CR2.exif.txt"
		"https://raw.pixls.us/getfile.php/1625/exif/6DII-RAW.CR2.exif.txt"
		"https://raw.pixls.us/getfile.php/771/exif/5G4A9394.CR2.exif.txt"
		"https://raw.pixls.us/getfile.php/1058/exif/AZ1I2270.CR2.exif.txt"

		#Nikon RAW file
		"https://raw.pixls.us/getfile.php/1855/exif/JCR_4305.NEF.exif.txt"
		"https://raw.pixls.us/getfile.php/1856/exif/JCR_4306.NEF.exif.txt"
		"https://raw.pixls.us/getfile.php/898/exif/lossless_compressed_14_bit.NEF.exif.txt"
		"https://raw.pixls.us/getfile.php/897/exif/lossless_compressed_12_bit.NEF.exif.txt"
		"https://raw.pixls.us/getfile.php/1842/exif/Nikon-D850-14bit-uncompressed.NEF.exif.txt"

		# Canon DNG file
		"https://raw.pixls.us/getfile.php/1022/exif/5G4A9394-uncompressed.DNG.exif.txt"

		# Leica DNG file
		"https://raw.pixls.us/getfile.php/752/exif/L1049391.DNG.exif.txt"
		"https://raw.pixls.us/getfile.php/1603/exif/f5381888.dng.exif.txt"

		# Fujifilm RAW file
		"https://raw.pixls.us/getfile.php/1436/exif/20170525_0036TEST.RAF.exif.txt")

	for i in {0..13}
	do
		wget --output-document="${img_name[$i]}" "${img_link[$i]}"
		if [ -f "${img_name[$i]}" ]; then
			mv "${img_name[$i]}" "../samples/Raw_files"
		fi
		wget --output-document="${img_name[$i]}_exif.txt" "${img_exif[$i]}"
		if [ -f "${img_name[$i]}_exif.txt" ]; then
			mv "${img_name[$i]}_exif.txt" "../samples/Raw_files"
		fi
	done
}


download_jpg()
{
	name="Canon_90D"
	link="https://img.photographyblog.com/reviews/canon_eos_90d/photos/canon_eos_90d"

	mkdir "../samples/"$name"_jpg"
	mkdir "../samples/"$name"_bmp"
	mkdir "../samples/"$name"_tiff"
	mkdir "../samples/"$name"_ppm"
    nb_image=$1

    for i in $(seq 1 $nb_image)
	do
		if [ $i -lt 10 ]; then
			wget --output-document=""$name"_0$i.jpg" ""$link"_0$i.jpg"
			if [ -f "Canon_90D_0"$i".jpg" ]; then
				convert -compress none ""$name"_0$i.jpg" ""$name"_0$i.bmp"
				convert -compress none ""$name"_0$i.jpg" ""$name"_0$i.tiff"
				convert ""$name"_0$i.jpg" ""$name"_0$i.ppm"
				mv ""$name"_0$i.jpg" "../samples/"$name"_jpg/"
				mv ""$name"_0$i.bmp" "../samples/"$name"_bmp/"
				mv ""$name"_0$i.tiff" "../samples/"$name"_tiff/"
				mv ""$name"_0$i.ppm" "../samples/"$name"_ppm/"
			fi
		else
			wget --output-document=""$name"_$i.jpg" ""$link"_$i.jpg"
			if [ -f ""$name"_$i.jpg" ]; then
				convert -compress none ""$name"_$i.jpg" ""$name"_$i.bmp"
				convert -compress none ""$name"_$i.jpg" ""$name"_$i.tiff"
				convert ""$name"_$i.jpg" ""$name"_$i.ppm"
				mv ""$name"_$i.jpg" "../samples/"$name"_jpg/"
				mv ""$name"_$i.bmp" "../samples/"$name"_bmp/"
				mv ""$name"_$i.tiff" "../samples/"$name"_tiff/"
				mv ""$name"_$i.ppm" "../samples/"$name"_ppm/"
			fi
		fi
	done
}


Help()
{
	echo "You came here for help, I see"
    echo "Run './sample -l' to download only 5 images"
    echo "Run './sample -f' to download all images with raw"
}


################################################################################
# Main program                                                                 #
################################################################################

if [ -d ../samples ]; then
rm -rf ../samples
fi
mkdir ../samples

# Get the options
while getopts "hlf" option; do
	case $option in
	h) # display Help
		Help
		exit;;
	l) #litte
		download_jpg 5
		printf "\e[96m\e[4mDone! 5 images have been download\n\e[0m"
		exit;;
	f) #full
		download_jpg 59
		download_raw
		printf "\e[96m\e[4mDone! All images have been download\n\e[0m"
		exit;;
	\?) # incorrect option
		echo "Error: Invalid option"
		exit;;
	esac
done


# File download thanks to this website : https://raw.pixls.us/#repo for the RAW
# File download thanks to this website : https://www.photographyblog.com/reviews/canon_eos_90d_review/preview_images for the jpeg
