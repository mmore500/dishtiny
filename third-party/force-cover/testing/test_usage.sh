#!/usr/bin/bash

result=$(coverage run -a fix_coverage.py)
if [ "$result" != "Usage: python fix_coverage.py [name_of_coverage_report_file]" ];
then 
    exit 1; 
fi