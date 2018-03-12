# .bashrc

# Source global definitions

if [ -f /etc/bashrc ]; then

. /etc/bashrc

fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:

# export SYSTEMD_PAGER=

# User specific aliases and functions

source /opt/share/intel/compilers_and_libraries_2017.4.196/linux/bin/compilervars.sh intel64

source /opt/share/intel/compilers_and_libraries_2017.4.196/linux/mkl/bin/mklvars.sh mic

export SINK_LD_LIBRARY_PATH=/opt/share/intel/compilers_and_libraries/linux/lib/mic/:/opt/share/intel/compilers_and_libraries_2017.4.196/linux/mkl/lib/mic/
