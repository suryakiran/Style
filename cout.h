#pragma once

#include <ostream>
#include <QtCore/QStringList>

std::ostream& operator<< (std::ostream& os, const QString& str);
std::ostream& operator<< (std::ostream& os, const QStringList& strList);
