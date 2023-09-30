#!/bin/bash

# Application update script from the Git repository
# (Bash scripts to move under /usr/local/bin)

ROOTDIR=/opt/data
APP=pgd-mmdt

GITREPOS=https://github.com/inrae/pgd-mmdt.git

(
  cd $ROOTDIR
  # Stop APP
  sh ./$APP/run stop
  echo
  echo "----------------------------------"
  echo "update $APP"
  echo "----------------------------------"
  # Move the APP directory to old
  mv $APP ${APP}.old
  OLD=${APP}.old
  # Get the source code from github
  git clone $GITREPOS $APP
  if [ $? -eq 0 ]; then
     # Config
     [ -f $OLD/local.conf ] && cp ./$OLD/local.conf ./$APP/
     cp ./$OLD/etc/.htpasswd ./$APP/etc/
     cp ./$OLD/dockerscanpart/scripts/config.py ./$APP/dockerscanpart/scripts/
     cp ./$OLD/web/inc/config/mongodb.inc ./$APP/web/inc/config/
     [ -f ./$OLD/web/inc/config/local.inc ] && cp ./$OLD/web/inc/config/local.inc ./$APP/web/inc/config/
     cp ./$OLD/web/conf/* ./$APP/web/conf/
     cp -rf ./$OLD/web/cache ./$APP/web/
     rm -rf ./$APP/web/cvlist/*
     cp -rf ./$OLD/web/cvlist/* ./$APP/web/cvlist/

     # Autocomplete
     #cp ./$OLD/web/js/autocomplete/*.js ./${APP}/web/js/autocomplete/
     chmod 755 ./$APP/web/js/autocomplete/
     chmod 644 ./$APP/web/js/autocomplete/*

     # Docs
     cp ./$OLD/web/docs/doc.md ./$APP/web/docs/
     chmod 775 ./$APP/web/docs/
     chmod 664 ./$APP/web/docs/*

     # Delete the .git directory
     ( cd $APP; rm -rf ./.git )
     ( cd $APP; rm -f .gitignore AUTHORS codemeta.json LICENSE README.md )

     # Restart APP
     ( cd $APP; sh ./run fullstart )

     # Delete old APP directory
     rm -rf ./$OLD
  fi
)
