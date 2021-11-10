read -p "Are you sure you want to clear autosave history? " -n 1 -r
echo

source ./envSet

if [[ $REPLY =~ ^[Yy]$ ]]
then
  rm $SAVE_DIR/*.req*
  rm $SAVE_DIR/*.sav?
  rm $SAVE_DIR/*.sav_*
  touch $SAVE_DIR/iocparker6k8_pass0.req
  chmod 666 $SAVE_DIR/iocparker6k8_pass0.req
  touch $SAVE_DIR/iocparker6k8.req
  chmod 666 $SAVE_DIR/iocparker6k8.req
fi
