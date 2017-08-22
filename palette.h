#pragma once

#include <QtGui/QColor>
#include <string>
#include <map>

struct Palette
{
    QColor light;
    QColor medium;
    QColor dark;
    QColor text;

    Palette (const std::string& l,
             const std::string& m,
             const std::string& d,
             const std::string& t = "#000000"
        )
        {
            light.setNamedColor(QString::fromStdString(l));
            medium.setNamedColor(QString::fromStdString(m));
            dark.setNamedColor(QString::fromStdString(d));
            text.setNamedColor(QString::fromStdString(t));
        }

    Palette ()
        {
            light.setNamedColor("#000000");
            medium.setNamedColor("#000000");
            dark.setNamedColor("#000000");
            text.setNamedColor("#000000");
        }

    QString gradient()
        {
            QString ret ("stop: 0 %1, stop: 0.5 %2, stop: 1 %3");
            return ret.arg(light.name()).arg(medium.name()).arg(dark.name());
        }

    QString gradientDarker()
        {
            QString ret ("stop: 0 %1, stop: 0.5 %2, stop: 1 %3");
            int fact {150};
            return ret
                .arg(light.darker(fact).name())
                .arg(medium.darker(fact).name())
                .arg(dark.darker(fact).name())
                ;
        }

    QString gradientLighter()
        {
            QString ret ("stop: 0 %1, stop: 0.5 %2, stop: 1 %3");
            int fact {125};
            return ret
                .arg(light.lighter(fact).name())
                .arg(medium.lighter(fact).name())
                .arg(dark.lighter(fact).name())
                ;
        }

        QString reverseGradient()
        {
            QString ret ("stop: 0 %1, stop: 0.5 %2, stop: 1 %3");
            return ret.arg(dark.name()).arg(medium.name()).arg(light.name());
        }
};

using PaletteMap = std::map <std::string, Palette>;
