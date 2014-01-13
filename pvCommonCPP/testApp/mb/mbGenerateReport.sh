#! /bin/bash

COMMAND=`basename $0`
FULL_CMD_LINE="$0 $*"

# environment variables
# MB_OUTPUT_DIR
# MB_REPORT_DIR

IGNORE_FIRST_N=0

###
### ------------ Command Line Parsing ------------
###
LONGOPTS=help,output_dir:,report_dir:,ignore_first_n:
SHORTOPTS=ho:r:i:

#
# Usage help.
#
function printUsage {
    cat << EOF
Reads micro-benchmark generated .csv filed and generates HTML report.

Usage $COMMAND [OPTIONS]
Options:
    -o | --output_dir        directory where .csv files are located, overrides MB_OUTPUT_DIR and current directory
    -r | --report_dir        directory where to put generated HTML report, overrides MB_REPORT_DIR and current directory
    -i | --ignore_first_n    ignore first n samples
    -h | --help              prints this help and exits

EOF
}


export POSIXLY_CORRECT=1
getopt -n $COMMAND -Q -u -a -l $LONGOPTS $SHORTOPTS "$@" || {
    printUsage
    exit 1;
}

set -- `getopt -u -a -l $LONGOPTS $SHORTOPTS "$@"`

while :
do
    case "$1" in
        --output_dir)      MB_OUTPUT_DIR=$2 ; shift ;;
        -o)                MB_OUTPUT_DIR=$2 ; shift ;;
        --report_dir)      MB_REPORT_DIR=$2 ; shift ;;
        -r)                MB_REPORT_DIR=$2 ; shift ;;
        --ignore_first_n)  IGNORE_FIRST_N=$2 ; shift ;;
        -i)                IGNORE_FIRST_N=$2 ; shift ;;
        --help)            printUsage ; exit 0 ;;
        -h)                printUsage ; exit 0 ;;
        --)                if [ X"$2" != X ] ; then printUsage ; exit 2 ; fi ; break ;;
    esac
    shift
done

unset POSIXLY_CORRECT

###
### ------------ Arguments Checking and Setup ------------
###

# Put the RE in a var for backward compatibility with versions <3.2
intregexp='^[0-9]*$'
if [[ ! $IGNORE_FIRST_N =~ $intregexp ]]
then
    echo "IGNORE_FIRST_N='$IGNORE_FIRST_N' is not a valid positive number"
    exit 1
fi

if [ X"$MB_OUTPUT_DIR" = X ]
then
    export MB_OUTPUT_DIR=$PWD
fi

if [ X"$MB_REPORT_DIR" = X ]
then
    export MB_REPORT_DIR=$PWD
fi

if [ ! -d $MB_OUTPUT_DIR ]
then
    echo "MB_OUTPUT_DIR directory '$MB_OUTPUT_DIR' does not exists."
    exit 2
fi

if [ ! -d $MB_REPORT_DIR ]
then
    mkdir -p $MB_REPORT_DIR || (echo "Failed to create MB_REPORT_DIR, existing..." && exit 2)
fi

###
### ------------ Main Logic ------------
###

INPUT_LIST=`ls -1 $MB_OUTPUT_DIR/mb_*.csv 2> /dev/null`
if [ X"$INPUT_LIST" = X ]
then
    echo "No mb_*.csv files found in $MB_OUTPUT_DIR."
    exit 3
fi

echo "Merging input files..."

# merge same named .csv files
# sort its content by stage
echo $INPUT_LIST | grep -v merged | cut -d '_' -f 2 | sort -u | xargs -I {} sh -c "cat $MB_OUTPUT_DIR/mb_{}*.csv | sort -t ',' -n -k2 > $MB_REPORT_DIR/merged_{}.csv"

echo "  done."

PROCESS_LIST=`ls $MB_REPORT_DIR/merged*.csv 2> /dev/null`
if [ X"$PROCESS_LIST" = X ]
then
    echo "No merged*.csv files found in $MB_REPORT_DIR, internal error!"
    exit 4
fi

# number format
FORMAT="%.3f"

for FILE in $PROCESS_LIST
do
    echo "Processing $FILE..."

    NAME=`basename $FILE | cut -d '_' -f 2 | cut -d '.' -f 1`
    DATE=`date`
    HTML_FILE="$FILE.html"

HTML_HEADER=$( cat << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
      "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <meta name="keywords"
  content="EPICS, EPICSv4, performance, benchmark, report" />
  <title>$NAME micro-benchmark report</title>
  <link rel="stylesheet" type="text/css" href="base.css" />
  <link rel="stylesheet" type="text/css" href="epicsv4.css" />
  <style type="text/css">
/*<![CDATA[*/
     .about { margin-left: 3em; margin-right: 3em; font-size: .83em}
     table { margin-left: auto; margin-right: auto }
     .diagram { text-align: center; margin: 2.5em 0 }
p.ed.priv { display: none }
span.ed.priv { display: none }
/*]]>*/</style>
<!-- p and span class "ed priv" is used to specify private editor class. Private -->
<!-- editor class are not displayed when display: none. Turn them to visible with -->
<!-- display: inline. -->
</head>

<body>

<div class="head">
<h1>$NAME micro-benchmark report</h1>
<p>
This report was generated on $DATE using the following command-line:
</p>
<pre>
    $FULL_CMD_LINE
</pre>

<p>
Default unit is nanosecond.
</p>

<hr />
</div>

EOF
)
    echo $HTML_HEADER > $HTML_FILE

    echo "    Determining number of stages..."
    # get stages, skip 0 stage
    STAGES=`cat $FILE | cut -d ',' -f 2 | sort -u -n | grep -v '^0'`
    STAGE_COUNT=`echo $STAGES | wc -w`
    echo "      done."

    STAT_FILE="$FILE.stat"
    rm $STAT_FILE 2> /dev/null

    IGNORE_FIRST_N_PLUS_ONE=`expr $IGNORE_FIRST_N + 1`

    for STAGE in $STAGES
    do
        echo "    Processing stage $STAGE of $STAGE_COUNT..."

        STAGE_FILE="$FILE.$STAGE"
        mb_stat $FILE -n -s $STAGE | tail -n +$IGNORE_FIRST_N_PLUS_ONE > $STAGE_FILE
        if [ ! -s $STAGE_FILE ]
        then
            echo "Stage $STAGE contains no samples, skipping..."
            continue
        fi
        STAT=`gnuplot -e "set print \"-\"; set datafile separator \",\"; stat \"$STAGE_FILE\" using 3 prefix \"s0\" nooutput; print $STAGE, s0_min, s0_lo_quartile, s0_mean, s0_stddev, s0_up_quartile, s0_max"`
        echo $STAT >> $STAT_FILE

        MIN_Y=`echo $STAT | cut -d ' ' -f 2`
        LO_QUART_Y=`echo $STAT | cut -d ' ' -f 3`
        MEAN_Y=`echo $STAT | cut -d ' ' -f 4`
        STDDEV_Y=`echo $STAT | cut -d ' ' -f 5`
        UP_QUART_Y=`echo $STAT | cut -d ' ' -f 6`
        MAX_Y=`echo $STAT | cut -d ' ' -f 7`

        # format
        MIN_Y=`printf $FORMAT $MIN_Y`
        LO_QUART_Y=`printf $FORMAT $LO_QUART_Y`
        MEAN_Y=`printf $FORMAT $MEAN_Y`
        STDDEV_Y=`printf $FORMAT $STDDEV_Y`
        UP_QUART_Y=`printf $FORMAT $UP_QUART_Y`
        MAX_Y=`printf $FORMAT $MAX_Y`

        #STAGE_GRAPH="$STAGE_FILE.png"
        STAGE_GRAPH="$STAGE_FILE.svg"

        #ITERATIONS=`cat $STAGE_FILE | wc -l`
        gnuplot << EOF
        #set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 1024, 768
        set terminal svg enhanced font "arial,10" size 1024, 768
        set output '$STAGE_GRAPH'

        set datafile separator ","
        set title "Stage $STAGE sampling"
        plot $MEAN_Y-$STDDEV_Y with filledcurves y1=$MEAN_Y lt 1 lc rgb "#bbbbdd" title "stddev range", \
             $MEAN_Y+$STDDEV_Y with filledcurves y1=$MEAN_Y lt 1 lc rgb "#bbbbdd" notitle, \
             $MEAN_Y w l lt 3 title "mean", \
             '$STAGE_FILE' using 3 w p pt 7 lt 1 ps 1 notitle
EOF


STAGE_STATS=$( cat << EOF
<tr>
  <td style="text-align:right">$STAGE</td>
  <td style="text-align:right">$MIN_Y</td>
  <td style="text-align:right">$LO_QUART_Y</td>
  <td style="text-align:right">$MEAN_Y</td>
  <td style="text-align:right">$STDDEV_Y</td>
  <td style="text-align:right">$UP_QUART_Y</td>
  <td style="text-align:right">$MAX_Y</td>
</tr>
EOF
)
SUMMARY_STATS="$SUMMARY_STATS $STAGE_STATS"

STAGE_GRAPH_BASENAME=`basename $STAGE_GRAPH`
STAGE_REPORT=$( cat << EOF

<div>
<h2>Stage $STAGE</h1>

<div class="image" style="text-align:center">
<!--img src="./$STAGE_GRAPH_BASENAME" alt="Stage $STAGE samples graph" width="100%" /-->
<object data="./$STAGE_GRAPH_BASENAME"></object>
<div style="font-weight:bold">
Stage $STAGE samples graph</div>
</div>

<table style="caption-side:bottom">
  <caption style="font-weight:bold; padding:1em">Stage $STAGE samples statistics table</caption>
  <tbody>
    <tr>
      <th>stage</th>
      <th>min</th>
      <th>lower quartile</th>
      <th>mean</th>
      <th>stddev</th>
      <th>upper quatrile</th>
      <th>max</th>
    </tr>
    $STAGE_STATS
  </tbody>
</table>

</div>

EOF
)
    echo $STAGE_REPORT >> $HTML_FILE





        echo "      done."
    done

    if [ ! -s $STAT_FILE ]
    then
        echo "Statistics file contains no data, exiting..."
        exit 5
    fi

    #REPORT_GRAPH="$FILE.png"
    REPORT_GRAPH="$FILE.svg"

    echo "    Generating report..."
    gnuplot << EOF
    #set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 1024, 768
    set terminal svg enhanced font "arial,10" size 1024, 768
    set output '$REPORT_GRAPH'

    set boxwidth 0.2 absolute
    set title "min/lo quartile/mean/hi quartile/max per stage"
    set xrange [ 0 : $STAGE_COUNT+1 ] noreverse nowriteback
    plot '$STAT_FILE' using 1:3:2:7:6:xticlabels(1) with candlesticks lt 3 lw 2 title 'Quartiles' whiskerbars, \
         ''         using 1:4:4:4:4 with candlesticks lt -1 lw 2 notitle
EOF




# overrall stats
STAGE_COUNT_PLUS_ONE=`expr $STAGE_COUNT + 1`

ITERATION_FILE="$FILE.iteration"

FILE_BY_ITER="$FILE.by_iter"
cat $FILE | sort -t ',' -n -k1 > $FILE_BY_ITER

mb_stat $FILE_BY_ITER -s $STAGE_COUNT_PLUS_ONE | tail -n +$IGNORE_FIRST_N_PLUS_ONE > $ITERATION_FILE
STAT=`gnuplot -e "set print \"-\"; stat \"$ITERATION_FILE\" using 1 prefix \"s0\" nooutput; print \"iteration \", s0_min, s0_lo_quartile, s0_mean, s0_stddev, s0_up_quartile, s0_max"`

MIN_Y=`echo $STAT | cut -d ' ' -f 2`
LO_QUART_Y=`echo $STAT | cut -d ' ' -f 3`
MEAN_Y=`echo $STAT | cut -d ' ' -f 4`
STDDEV_Y=`echo $STAT | cut -d ' ' -f 5`
UP_QUART_Y=`echo $STAT | cut -d ' ' -f 6`
MAX_Y=`echo $STAT | cut -d ' ' -f 7`

IPS=`gnuplot -e "set print \"-\"; print 1000000000.0/$MEAN_Y"`
IPS_STDDEV=`gnuplot -e "set print \"-\"; print $IPS-1000000000.0/($MEAN_Y+$STDDEV_Y)"`

# format
MIN_Y=`printf $FORMAT $MIN_Y`
LO_QUART_Y=`printf $FORMAT $LO_QUART_Y`
MEAN_Y=`printf $FORMAT $MEAN_Y`
STDDEV_Y=`printf $FORMAT $STDDEV_Y`
UP_QUART_Y=`printf $FORMAT $UP_QUART_Y`
MAX_Y=`printf $FORMAT $MAX_Y`

IPS=`printf $FORMAT $IPS`
IPS_STDDEV=`printf $FORMAT $IPS_STDDEV`

ITERATION_STATS=$( cat << EOF
<tr>
  <td style="text-align:right;font-weight:bold">iteration</td>
  <td style="text-align:right;font-weight:bold">$MIN_Y</td>
  <td style="text-align:right;font-weight:bold">$LO_QUART_Y</td>
  <td style="text-align:right;font-weight:bold">$MEAN_Y</td>
  <td style="text-align:right;font-weight:bold">$STDDEV_Y</td>
  <td style="text-align:right;font-weight:bold">$UP_QUART_Y</td>
  <td style="text-align:right;font-weight:bold">$MAX_Y</td>
</tr>
EOF
)
SUMMARY_STATS="$SUMMARY_STATS $ITERATION_STATS"






REPORT_GRAPH_BASENAME=`basename $REPORT_GRAPH`
SUMMARY_REPORT=$( cat << EOF

<div>
<h2>Summary</h1>

<div class="image" style="text-align:center">
<!--img src="./$REPORT_GRAPH_BASENAME" alt="Summary statistics graph" width="100%" /-->
<object data="./$REPORT_GRAPH_BASENAME"></object>
<div style="font-weight:bold">
Summary statistics graph</div>
</div>

<table style="caption-side:bottom">
  <caption style="font-weight:bold; padding:1em">Summary statistics table</caption>
  <tbody>
    <tr>
      <th>stage</th>
      <th>min</th>
      <th>lower quartile</th>
      <th>mean</th>
      <th>stddev</th>
      <th>upper quatrile</th>
      <th>max</th>
    </tr>
    $SUMMARY_STATS
  </tbody>
</table>

</div>

<p style="font-size:+1">This results in $IPS +- $IPS_STDDEV iteration(s)/second.</p>
</p>
EOF
)
    echo $SUMMARY_REPORT >> $HTML_FILE



    echo "      done."


    echo "  </body></html>" >> $HTML_FILE

    echo "  done."
done


