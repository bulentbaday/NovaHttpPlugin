# NovaHttpPlugin
This plugin allows you to make http requests from your MySQL database. It is suitable for Windows based MySQL servers. The following guide helps you how to set it up.

* Clone or download the project files into your machine.

* Please do not forget to change include directory (\include) according to your machine.

* Build it by using any C++ compiler to generate dll library.

* Copy NovaHttpPlugin.dll library into the plugin folder of your MySQL server. It is located in \lib\plugin.

* In order to create a UDF Function, execute following script.

CREATE FUNCTION HttpRequest RETURNS STRING
  SONAME 'NovaHttpPlugin.dll';

## Examples:

SELECT HttpRequest('https://www.novayazilim.com.tr', 'GET', 'text/html', null) data;

SELECT convert(HttpRequest('https://www.novayazilim.com.tr', 'GET', 'text/html', null) using utf8) data;

## YouTube

You can also watch the video tutorial below that shows you how to perform each steps.

https://youtu.be/mVJvchaI9RQ
