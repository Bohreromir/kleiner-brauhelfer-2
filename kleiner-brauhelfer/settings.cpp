#include "settings.h"
#include <QCoreApplication>
#include <QFileInfo>

Settings::Settings(QObject *parent) :
    QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName(), parent)
{
    initTheme();
}

Settings::Settings(bool dummyPortable, QObject *parent) :
    QSettings(QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + ".ini", QSettings::IniFormat, parent)
{
    Q_UNUSED(dummyPortable)
    initTheme();
}

SettingsPortable::SettingsPortable(QObject *parent) :
    Settings(true, parent)
{
}

Settings::~Settings()
{
    beginGroup("General");
    setValue("version", QCoreApplication::applicationVersion());
    endGroup();
}

void Settings::initTheme()
{
    beginGroup("Style");

    font = value("Font", QFont()).value<QFont>();

    InputBase = QColor(240, 255, 255);
    ErrorBase = QColor(220, 127, 127);
    WarningText = QColor(219, 137, 9);
    ErrorText = QColor(180, 0, 0);

    MekrlisteBackground = QColor(122, 163, 233);
    VerbrauchtBackground = QColor(180, 180, 180);
    AbgefuelltBackground = QColor(193, 225, 178);
    GebrautBackground = QColor(225, 216, 184);
    NichtGebrautBackground = QColor(245, 245, 245);

    HopfenTypBackgrounds = {QColor(255,255,255), QColor(255,200,170), QColor(200,255,200), QColor(200,200,255)};
    HefeTypOgUgBackgrounds = {QColor(255,255,255), QColor(255,200,170), QColor(200,200,255)};
    HefeTypTrFlBackgrounds = {QColor(255,255,255), QColor(255,200,170), QColor(200,200,255)};
    HefeSedBackgrounds = {QColor(255,255,255), QColor(255,200,170), QColor(200,255,200), QColor(200,200,255)};
    WZTypBackgrounds = {QColor(255,200,170), QColor(250,250,230), QColor(200,255,200), QColor(235,255,200), QColor(200,200,255)};

    DiagramLinie1 = QColor(116, 30, 166);
    DiagramLinie2 = QColor(56, 104, 2);
    DiagramLinie3 = QColor(170, 0, 0);
    DiagramLinie1Light = QColor(151, 125, 166);
    DiagramLinie2Light = QColor(141, 154, 127);
    DiagramLinie3Light = QColor(170, 100, 100);

    mTheme = static_cast<Theme>(value("Theme", System).toInt());

    endGroup();

    switch (mTheme)
    {
    case System:
        palette = mSystemPalette;
        palette.setColor(QPalette::Highlight, palette.highlight().color());
        palette.setColor(QPalette::HighlightedText, palette.highlightedText().color());
        break;

    case Bright:
        palette.setColorGroup(QPalette::Disabled,
                              Qt::gray,//windowText
                              QColor(226,226,226),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              Qt::darkGray,//text
                              Qt::red,//bright_text
                              QColor(255,255,255),//base
                              QColor(226,226,226));//window
        palette.setColorGroup(QPalette::Active,
                              QColor(40,40,40),//windowText
                              QColor(226,226,226),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              QColor(40,40,40),//text
                              Qt::red,//bright_text
                              QColor(255,255,255),//base
                              QColor(226,226,226));//window
        palette.setColorGroup(QPalette::Inactive,
                              QColor(40,40,40),//windowText
                              QColor(226,226,226),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              QColor(40,40,40),//text
                              Qt::red,//bright_text
                              QColor(255,255,255),//base
                              QColor(226,226,226));//window
        palette.setColor(QPalette::Highlight, QColor(0,85,255));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::ToolTipBase, QColor(34,34,34));
        palette.setColor(QPalette::ToolTipText, QColor(255,255,255));
        break;

    case Dark:
        palette.setColorGroup(QPalette::Disabled,
                              Qt::gray,//windowText
                              QColor(53,53,53),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              Qt::gray,//text
                              Qt::red,//bright_text
                              QColor(34,34,34),//base
                              QColor(53,53,53));//window
        palette.setColorGroup(QPalette::Active,
                              Qt::white,//windowText
                              QColor(53,53,53),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              Qt::white,//text
                              Qt::red,//bright_text
                              QColor(34,34,34),//base
                              QColor(53,53,53));//window
        palette.setColorGroup(QPalette::Inactive,
                              Qt::white,//windowText
                              QColor(53,53,53),//button
                              QColor(255,255,255),//light
                              QColor(255,255,255),//dark
                              QColor(255,255,255),//mid
                              Qt::white,//text
                              Qt::red,//bright_text
                              QColor(34,34,34),//base
                              QColor(53,53,53));//window
        palette.setColor(QPalette::Highlight, QColor(0,85,255));
        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::ToolTipBase, QColor(55,55,55));
        palette.setColor(QPalette::ToolTipText, QColor(255,255,255));
        break;
    }


    paletteInput = palette;
    //paletteInput.setColor(QPalette::Window, ErrorBackground);
    //paletteInput.setColor(QPalette::Base, InputBase);

    paletteError = palette;
    paletteError.setColor(QPalette::Base, ErrorBase);

    paletteErrorButton = palette;
    paletteErrorButton.setColor(QPalette::Button, ErrorBase);
}

Settings::Theme Settings::theme() const
{
    return mTheme;
}

void Settings::setTheme(Theme theme)
{
    beginGroup("Style");
    setValue("Theme", theme);
    endGroup();
    initTheme();
}

QString Settings::style()
{
    beginGroup("Style");
    QString style = value("Style", "Fusion").toString();
    endGroup();
    return style;
}

void Settings::setStyle(const QString &style)
{
    beginGroup("Style");
    setValue("Style", style);
    endGroup();
}

bool Settings::useSystemFont()
{
    beginGroup("Style");
    bool system = value("UseSystemFont", true).toBool();
    endGroup();
    return system;
}

void Settings::setUseSystemFont(bool system)
{
    beginGroup("Style");
    setValue("UseSystemFont", system);
    endGroup();
}

void Settings::setFont(const QFont &font)
{
    beginGroup("Style");
    setValue("Font", font);
    endGroup();
    this->font = font;
}

QString Settings::settingsDir() const
{
    return QFileInfo(fileName()).absolutePath() + "/";
}

QString Settings::databasePath()
{
    QString path;
    beginGroup("General");
    path = value("database").toString();
    endGroup();
    return path;
}

void Settings::setDatabasePath(const QString& path)
{
    beginGroup("General");
    setValue("database", path);
    endGroup();
}

QString Settings::databaseDir()
{
    return QFileInfo(databasePath()).absolutePath() + "/";
}

QString Settings::dataDir() const
{
    return settingsDir() + "data/";
}

QString Settings::lastProgramVersion()
{
    QString version;
    beginGroup("General");
    version = value("version").toString();
    endGroup();
    return version;
}
