#!bin/bash

cd /home/once/coding/main_projects/orm/editor/doc/
rm schedule.xml
unoconv -f odt schedule.docx 
mv schedule.odt schedule.zip 
mkdir unziped 
unzip schedule.zip -d unziped/ 
cp unziped/content.xml ./schedule.xml
rm -Rf unziped schedule.zip *~


