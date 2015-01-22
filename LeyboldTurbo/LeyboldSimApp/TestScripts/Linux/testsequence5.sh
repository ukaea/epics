export EPICS_CA_SERVER_PORT=5071

gnome-terminal -e ./camonitor5.sh

./testsequence1.sh 1
sleep 10

./testsequence1.sh 2
sleep 10

./testsequence1.sh 3
sleep 10

./testsequence1.sh 4
sleep 10

./testsequence1.sh 5
