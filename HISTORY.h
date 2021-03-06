/* 
 * -----------------------------------
 * p4 Daemon / p4d -  Revision History
 * -----------------------------------
 *
 */

#define _VERSION     "0.1.37"
#define VERSION_DATE "04.03.2016"

#ifdef GIT_REV
#  define VERSION _VERSION "-GIT" GIT_REV
#else
#  define VERSION _VERSION
#endif

/*
 * ------------------------------------

2016-03-04:  version 0.1.37
   - bugfix: remove steering chars in unit (delivered by S-3200)

2016-03-04:  version 0.1.36
   - bugfix: fixed mode display in state mails

2016-03-02:  version 0.1.35
   - added:  update of table smartconf

2016-03-01:  version 0.1.34
   - change: ported to new db api
   - added:  added table smartconfig (by S.Döring)

2016-03-01:  version 0.1.33
   - bugfix: minor fix for html header
   - added:  git handling to makefile
   
2016-03-01:  version 0.1.32
   - change: html header of status mails now configurable in "/etc/p4d/mail-head.html"
             example can be found in configs

2016-02-29:  version 0.1.31
   - change: source environment in update.sh

2016-02-24:  version 0.1.30
   - change: update of after-update.sh example
   - change: updated html (outfit) of status mails

2016-02-18:  version 0.1.29
   - change: added support of usrtitle to after-update.sh example

2016-02-17:  version 0.1.28
   - bugfix: fixed ddl

2016-02-17:  version 0.1.27
   - added: homematic example script
   - change: updated webif

2016-02-17:  version 0.1.26
   - bugfix: fixed call of script <confdir>/after-update.sh

2016-02-16:  version 0.1.25
   - added: added script hook called after update cycle is performed

2015-06-24:  version 0.1.24
   - change: don't touche value facts during init for 'none' updates 

2015-04-14:  version 0.1.23
   - bugfix: fixed displayed timezone

2015-03-24:  version 0.1.22
   - change: Ported to horchi's new DB API ;)

2015-03-23:  version 0.1.21
   - change: moved p4d config from /etc/ to /etc/p4d/

2015-03-12:  version 0.1.20
   - added:  web config for sensor alerts by 'Abholzer'
   - added:  %range% and %delta% telpmates for alert mails

2015-03-10:  version 0.1.19
   - added:  implemented sensor range alert check

2015-03-05:  version 0.1.18
   - added:  implementation of sensor alert mails

2015-03-05:  version 0.1.17
   - added:  Started implementation of sensor alerts

2015-02-27:  version 0.1.16
   - added:  New table for sensor alerts (not implemented yet)

2015-02-27:  version 0.1.15
   - change: Improved trouble handling
   - added:  Setup of aggregation to README
   - added:  Setup of One-Wire-Sensors to README

2015-01-12:  version 0.1.14
   - change: On missing one-wire sensor show 'Info' instead of 'Error'

2014-12-01:  version 0.1.13
   - change: implemented aggregation
   - change: support of html mails

2014-12-01:  version 0.1.12
   - change: p4d 'prepared' for aggregation of samples

2014-11-28:  version 0.1.11
   - added:  support for 1W sensors
   - change: moved webif images to subfolder
   - change: minor changes and fixes

2014-02-07:  version 0.1.10
   - change: supporting WEBIF communication event if 
             serial line is disturbed

2014-02-05:  version 0.1.9
   - bugfix: fixed problem with negative parameter values

2014-02-01:  version 0.1.8
   - change: improved recover handling of serial communication
   - added:  more state and heater images.
   - added:  config of heating type to WEBIF
   - added:  more heater states
   - added:  configuration of schema image to WENIF

2014-01-30:  version 0.1.7
   - added :  index for jobs table
   - bugfix:  fixed mail body of 'STÖRUNGs' mails

2014-01-24:  version 0.1.6
   - change: fixed table shadow for firefox
   - bugfix: fixed passwort check fpr initial login

2014-01-07:  version 0.1.5
   - change: added default unit % for analog outputs
   - change: update of README thx Thomas!

2014-01-07:  version 0.1.4
   - added:  recording of digital inputs
   - added:  recording of analog outouts
   - bugfix: Fixed another security issue

2014-01-06:  version 0.1.3
   - bugfix: Fixed security issues reported by b_a_s_t_i !Thx!

2014-01-03:  version 0.1.2
   - added:  time synchronisation to adjust s3200's system time
   - added:  error history to WEBIF
   - added:  auto refresh to view 'Aktuell'
   - change: improved status display

2013-12-23: version 0.1.1
   - added: settings to WEBIF

2013-12-18: version 0.1.0
   - first official release

2010-11-04: version 0.0.1
   - start of implementation

 * ------------------------------------
 */
