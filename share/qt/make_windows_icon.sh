#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/loonie.png
ICON_DST=../../src/qt/res/icons/loonie.ico
convert ${ICON_SRC} -resize 16x16 loonie-16.png
convert ${ICON_SRC} -resize 32x32 loonie-32.png
convert ${ICON_SRC} -resize 48x48 loonie-48.png
convert loonie-16.png loonie-32.png loonie-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/loonie_testnet.png
ICON_DST=../../src/qt/res/icons/loonie_testnet.ico
convert ${ICON_SRC} -resize 16x16 loonie-16.png
convert ${ICON_SRC} -resize 32x32 loonie-32.png
convert ${ICON_SRC} -resize 48x48 loonie-48.png
convert loonie-16.png loonie-32.png loonie-48.png ${ICON_DST}
rm loonie-16.png loonie-32.png loonie-48.png
