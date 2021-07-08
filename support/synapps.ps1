<#
.SYNOPSIS
   Prints out the first line of every 'VERSION' file from a root directory.
 
.DESCRIPTION
  Find EPICS VERSION files in configure directory.
 
.NOTES
   File Name: synapps.ps1
   Author   : Tim Speight
   Version  : 1.1
   Date     : 7/7/2021
 
.EXAMPLE
   PS > .\synapps.ps1
#>

Get-ChildItem -Filter "VERSION" -Recurse -File | select-object Name, @{l='first line'; e={$_ |Get-Content -First 1}}