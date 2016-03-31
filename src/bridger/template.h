#ifndef TEMPLATE_FILE_H_
#define TEMPLATE_FILE_H_

#include <QString>

static const QString TEMPLATE_HEADER =
QObject::tr(
        "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n");

static const QString TEMPLATE_README = QObject::tr("<hr>"
"Basic Usage:"
"<br/>"
"<br/>"
"The application is used for local file keyword search<br/>"
"Feel free to send email to <B>WaterLin.org@gmail.com</B> if you have any questions!<br/>"
"Copyright (C) WaterLin.org. All rights reserved.");

static const QString TEMPLATE_INDEX = TEMPLATE_HEADER + QObject::tr(
"<a href=\"/readme\">Readme</a> "
"<a href=\"/config\">Config</a> "
"<br>"
"<hr/>"
"<em>This is the new line of Local Search Engine</em>"
"<br>"
"<b> Search the knowledge base</b>"
"<br>"
"<form action=\"search\" method=\"get\">"
"Keyword: <input type=\"text\" name=\"keyword\">"
"<input type=\"submit\" value=\"Submit\">"
"</form>"
"<hr/>");

static const QString TEMPLATE_ADDPATH = QObject::tr(
"<form action=\"config\" method=\"get\">"
"Add Path: <input type=\"text\" name=\"addpath\">"
"<input type=\"submit\" value=\"Submit\">"
"</form>"
);

#endif /* TEMPLATE_FILE_H_ */