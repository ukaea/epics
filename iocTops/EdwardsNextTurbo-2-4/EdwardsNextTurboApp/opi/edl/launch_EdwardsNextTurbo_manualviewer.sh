#!/bin/sh
#
# Script launch_EdwardsNextTurbo_manualview.sh
#
# To launch a pdf viewer on one of the Edwards manuals in the
# documentation directory.  Attempting to get a script which works
# when run from the source code location or the built code location
# over different releases of the module without using the problematic
# TOPDIR variable or unix .. path in the context of the EPICS build.
# We know that we in this Edwards script here we are somewhere below
# the EdwardsNextTurbo directory, and the documentation directory is
# just below it, therefore we share a common parent directory
# somewhere up the tree with the Edwards documentation directory, so
# find out our path and go up it until we find the documentation
# directory.
#
# The pdf viewer tool available on RHEL6 is evince.
# Not using browsers which can do their own thing and find a pdf tool,
# but firefox has this annoying local profile setup thing which breaks
# it if it is already running on another host and the profile is not
# set up and other browsers are less familiar to the users.
#

manual_viewer="evince"
relative_doc_dirname="documentation"
#relative_doc_dirname="doc"
relative_manual_dirname="private/Manufacturer/Edwards"
rootdir="/"
my_absolute_dirpath=$(dirname $(readlink -m $0))
relative_manual_filename=$(basename $1)

#echo "my_absolute_dirpath: $my_absolute_dirpath"

# Initialise for directory search loop.
absolute_EdwardsNextT_dirpath=$my_absolute_dirpath
absolute_doc_dirname=""

while [ "$absolute_EdwardsNextT_dirpath" != "$rootdir" ]; do

  # Construct possible absolute documentation directory name.
  absolute_doc_dirname=$absolute_EdwardsNextT_dirpath/$relative_doc_dirname

#  echo "absolute_doc_dirname: $absolute_doc_dirname"

  # See if the directory exists.
  if [ -d $absolute_doc_dirname ]; then
    # If it does exist, append the relative name of the manual directory name below it.
    absolute_manual_dirname="$absolute_doc_dirname/$relative_manual_dirname"

#    echo "absolute_manual_dirname: $absolute_manual_dirname"

    if [ -d $absolute_manual_dirname ]; then
      # We found it, short circuit the while loop
      absolute_EdwardsNextT_dirpath=$rootdir
    else
      # Set the manual directory name back to null because it is not valid.
      absolute_manual_dirname=""
    fi
  fi
  # Construct new path ready for next pass of while loop.
  absolute_EdwardsNextT_dirpath=$(dirname $absolute_EdwardsNextT_dirpath)
done

#echo "absolute_manual_dirname: $absolute_manual_dirname"

# Construct proposed absolute manual filename.
absolute_manual_filename=$absolute_manual_dirname/$relative_manual_filename

# We could do some obsessive checks on what type of file the manual file is but we dont.
# Instead we just check if it exists.
if [ ! -e $absolute_manual_filename ]; then
    # Last ditch attempt to recover - assume the original argument was the full pathname - wont
    # work from the GUI.
    absolute_manual_filename=$1
fi

#echo "absolute_manual_filename: $absolute_manual_filename"

$manual_viewer $absolute_manual_filename &
