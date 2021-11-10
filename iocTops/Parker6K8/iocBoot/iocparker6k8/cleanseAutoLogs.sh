read -p "Are you sure you want to blank autosaves for all 6K8 Axis? " -n 1 -r
echo
source ./envSet

if [[ $REPLY =~ ^[Yy]$ ]]
then
  #WARNING:  Those users are going to have to reinput all their Limits and descriptions
  rm $SAVE_DIR/iocparker6k8.sav
  touch $SAVE_DIR/iocparker6k8.sav
  chmod 666 $SAVE_DIR/iocparker6k8.sav
  echo "Autosave config erased..."
fi
