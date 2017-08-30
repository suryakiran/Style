#include <mainwindow.h>
#include <QtGui/QPixmap>
#include <QtWidgets/QStyleFactory>
#include <fstream>
#include <string>
#include <iostream>
#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <cout.h>

MainWindow::MainWindow (QWidget* parent)
    : QMainWindow(parent)
{
    setupUi(this);
    
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate(comboBox);
    comboBox->setItemDelegate(itemDelegate);
    
    initPalette();
    populateMenu();
    readStyleSheet();

    initTreeView();
}

void
MainWindow::initTreeView()
{
    model = new QStandardItemModel(treeView);

    for (int row = 0; row < 4; ++row) {
        QList<QStandardItem*> items;
        for (int col = 0; col < 2; ++col) {
            QString text ("Item %1-%2");
            text = text.arg(row+1).arg(col+1);
            items << new QStandardItem(text);
        }
        model->appendRow(items);
    }

    model->setHorizontalHeaderItem(0, new QStandardItem("Column 1"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Column 2"));

    treeView->setRootIsDecorated(false);
    treeView->setModel(model);
}

void
MainWindow::setStyleSheet()
{
    QStringList ssList;

    for (const auto& item: styleSheet) {
        ssList << substituteColors(item);
    }

    qApp->setStyleSheet(ssList.join('\n'));
}

void
MainWindow::readStyleSheet()
{
    std::string ssFile = STYLESHEET_FILE;
    std::fstream fin (ssFile, std::ios_base::in);

    if (!fin) {
        std::cout << "Unable to open file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fin, line, '\n')) {
        styleSheet.push_back(line);
    }
}

QString
MainWindow::substituteColors (const std::string& line)
{
    QString ret (QString::fromStdString (line));
    auto& pal = palette[currentTheme];
    auto& bg = backgrounds[currentBackground];
    ret
        .replace("@top-to-bottom@", "x1: 0, y1: 0, x2: 0, y2: 1")
        .replace("@bottom-to-top@", "x1: 0, y1: 1, x2: 0, y2: 0")
        .replace("@left-to-right@", "x1: 0, y1: 0, x2: 1, y2: 0")
        .replace("@reverse-gradient@", pal.reverseGradient())
        .replace("@bg:reverse-gradient@", bg.reverseGradient())
        .replace("@gradient@", pal.gradient())
        .replace("@gradient-lighter@", pal.gradientLighter())
        .replace("@gradient-darker@", pal.gradientDarker())
        .replace("@bg-gradient@", bg.gradient())
        .replace("@bg-gradient-lighter@", bg.gradientLighter())
        .replace("@bg-gradient-darker@", bg.gradientDarker())
        .replace("@bg-light@", bg.light.name())
        .replace("@bg-dark@", bg.dark.name())
        .replace("@bg-medium@", bg.medium.name())
        .replace("@selected-text-color@", pal.text.name())
        .replace("@text-color@", bg.text.name())
        .replace("@pal-dark@", pal.dark.name())
        .replace("@pal-medium@", pal.medium.name())
        .replace("@pal-light@", pal.light.name())
        .replace("@pal-light-lighter@", pal.light.lighter().name())
        ;

    // std::cout << ret << std::endl;
    return ret;
}

void
MainWindow::initPalette()
{
    palette["Illustrious"]  = Palette("#f9a1ac", "#dc6472", "#c6464b");
    palette["Butter"]       = Palette("#fce94f", "#edd400", "#c4a000");
    palette["Wise"]         = Palette("#97bf60", "#709937", "#51751e");
    palette["Human"]        = Palette("#faa546", "#e07a1f", "#ce5c00");
    palette["Dust"]         = Palette("#b49372", "#906e4c", "#745536");
    palette["Brave"]        = Palette("#729fcf", "#3465a4", "#204a87", "#ffffff");
    palette["Noble"]        = Palette("#ad7fa8", "#75507b", "#5c3566", "#ffffff");
    palette["Wine"]         = Palette("#df5757", "#c22f2f", "#a40000", "#ffffff");
    
    backgrounds["Light"]    = Palette("#eeeeec", "#d3d7cf", "#babdb6");
    backgrounds["Dark"] = Palette("#888a85", "#555753", "#2e3436", "#ffffff");
    backgrounds["Medium"]      = Palette("#bcbea7", "#9c9e8a", "#7d7e60");
}

MainWindow::~MainWindow()
{
}

QIcon
MainWindow::iconFromTheme(const std::string& theme)
{
    QPixmap pixmap(15, 15);

    auto iter = palette.find(theme);
    QColor color;
    bool found {false};
    if (iter != palette.end()) {
        pixmap.fill(palette[theme].dark);
    }

    if (!found) {
        iter = backgrounds.find(theme);
        if (iter != backgrounds.end()) {
            pixmap.fill(backgrounds[theme].dark);
        }
    }

    return QIcon(pixmap);
}

void
MainWindow::populateMenu()
{
    for (const auto& item: palette) {
        auto str = item.first;
        auto action = selectTheme->addAction(iconFromTheme(str), QString::fromStdString(str));
        QObject::connect(action, &QAction::triggered, this, &MainWindow::changeTheme);
    }
    
    for (const auto& item: backgrounds) {
        auto str = item.first;
        auto action = selectBackground->addAction(iconFromTheme(str), QString::fromStdString(str));
        QObject::connect(action, &QAction::triggered, this, &MainWindow::changeBackground);
    }

}

void
MainWindow::changeBackground()
{
    QAction* action = qobject_cast<QAction*>(sender());
    currentBackground = action->text().toStdString();
    setStyleSheet();
}

void
MainWindow::changeTheme()
{
    QAction* action = qobject_cast<QAction*>(sender());
    currentTheme = action->text().toStdString();
    setStyleSheet();
}

void
MainWindow::savePalette()
{
    std::fstream fout("gnome-icon-theme.gpl", std::ios_base::out);

    if (!fout) {
        std::cout << "Unable to open file for writing" << std::endl;
        return;
    }

    fout << "GIMP Palette" << std::endl;
    fout << "Name: Gnome Icon Theme Palette\n#" << std::endl;

    auto lamfun = [&](const auto& item) {
        fout
        << item.second.light.red()  << ' '
        << item.second.light.green() << ' '
        << item.second.light.blue() << ' '
        << item.second.light.name() << '\n'
        << item.second.medium.red() << ' '
        << item.second.medium.green() << ' '
        << item.second.medium.blue() << ' '
        << item.second.medium.name() << '\n'
        << item.second.dark.red() << ' '
        << item.second.dark.green() << ' '
        << item.second.dark.blue() << ' '
        << item.second.dark.name()
        << std::endl;
    };

    for (const auto& item: palette) {
        lamfun(item);
    }

    for (const auto& item: backgrounds) {
        lamfun(item);
    }

    fout.close();
    
}

void
MainWindow::showEvent (QShowEvent* event)
{
    static bool first = true;

    if (first)
    {
        qApp->setStyle(QStyleFactory::create("Windows"));
        setStyleSheet();
        savePalette();
        first = false;
    }

    QMainWindow::showEvent(event);
}
