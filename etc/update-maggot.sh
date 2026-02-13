#!/bin/bash

# Application update script from the Git repository
# (Bash scripts to move under /usr/local/bin)

# List of files to be preserved
# see ./etc/preserve.list

# List of directory to be preserved
# ./web/conf/
# ./web/cvlist/
# ./web/cache
# ./web/js/autocomplete/
# ./web/inc/mapping/

REPOS=pgd-mmdt     # name of the github repository
 
ROOTDIR=/opt/apps  # the path of the root directory where the maggot application is installed
APP=pgd-mmdt       # name of the directory under the root directory corresponding to the maggot application
SYSTEMCTL=0        # indicates if systemctl is used for stopping / starting the maggot application

GITREPOS=https://github.com/inrae/${REPOS}.git

(
  cd $ROOTDIR
  # Stop APP
  [ $SYSTEMCTL -eq 0 ] && sudo sh ./$APP/run stop
  [ $SYSTEMCTL -eq 1 ] && sudo systemctl stop $APP

  echo
  echo "----------------------------------"
  echo "update $APP"
  echo "----------------------------------"

  # Add 'prev' as postfix to the current APP directory
  sudo mv $APP ${APP}.prev
  PREV=${APP}.prev

  # Get the source code from github
  sudo git clone $GITREPOS $APP

  if [ $? -eq 0 ]; then
     # Copy the list of files to be preserved
     LISTFILE=$APP/etc/preserve.list
     for F in $(cat $LISTFILE | grep -v -E "^(#|$)"); do
          [ -f "$PREV/$F" ] && echo sudo cp ./$PREV/$F ./$APP/$(dirname $F)/
     done

     # Cache
     sudo cp -rf ./$PREV/web/cache ./$APP/web/
     sudo chmod -R 777 ./$APP/web/cache

     # cvlist
     sudo rm -rf ./$APP/web/cvlist/*
     sudo cp -rf ./$PREV/web/cvlist/* ./$APP/web/cvlist/
     find $APP/web/cvlist/ -name "*.txt" -a ! -name "*_format.txt" -exec sudo chmod 777 {} \;

     # Autocomplete
     for f in $(ls ./$PREV/web/js/autocomplete/*.js); do
         F=$(basename $f)
         [ ! -f ./$APP/web/js/autocomplete/$F ] && cp -f $f ./$APP/web/js/autocomplete/$F;
     done
     sudo chmod 755 ./$APP/web/js/autocomplete/
     sudo chmod 644 ./$APP/web/js/autocomplete/*

     # Mapping
     for f in $(ls ./$PREV/web/inc/mapping/*.inc); do
         F=$(basename $f)
         [ ! -f ./$APP/web/inc/mapping/$F ] && cp -f $f ./$APP/web/inc/mapping/$F;
     done
     sudo chmod 755 ./$APP/web/inc/mapping/
     sudo chmod 644 ./$APP/web/inc/mapping/*

     # Docs
     sudo chmod 775 ./$APP/web/docs/
     sudo chmod 664 ./$APP/web/docs/*

     # Web interface customization
     #sudo cp ./$PREV/web/inc/home/home_left.inc ./$APP/web/inc/home/
     #sudo cp ./$PREV/web/inc/home/home_right.inc ./$APP/web/inc/home/
     #sudo cp ./$PREV/web/inc/footer.inc ./$APP/web/inc/
     #sudo cp ./$PREV/web/img/banner-background.png ./$APP/web/img/

     # Delete the .git directory
     ( cd $APP; sudo rm -rf ./.git )
     ( cd $APP; sudo rm -f .gitignore AUTHORS codemeta.json LICENSE README.md CHANGES.md )

     # Restart APP
     [ $SYSTEMCTL -eq 0 ] && ( cd $APP; sudo sh ./run fullstart )
     [ $SYSTEMCTL -eq 1 ] && systemctl start $APP

     # Delete old APP directory
     sudo rm -rf ./$PREV
  fi
)
