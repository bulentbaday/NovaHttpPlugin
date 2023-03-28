# NovaHttpPlugin
This plugin allows you to make http requests from your MySQL database. It is suitable for Windows Servers. The following guide helps you how to set it up.

* Clone or download the project files into your machine.

* Please do not forget to change include directory (..\mysql-8.0.23-winx64\include) according to your machine.

* Build it by using any C++ compiler to get dll file.

* Copy NovaHttpPlugin.dll file into the plugin folder of your MySQL server. It is located in ..\mysql-8.0.23-winx64\lib\plugin.

* In order to create an UDF Function, execute following script.

CREATE FUNCTION HttpRequest RETURNS STRING
  SONAME 'NovaHttpPlugin.dll';

* Examples:

SELECT HttpRequest('https://www.novayazilim.com.tr', 'GET', 'text/html', null) data;

SELECT convert(HttpRequest('https://www.novayazilim.com.tr', 'GET', 'text/html', null) using utf8) data;

https://www.youtube.com/watch?v=mVJvchaI9RQ
