#include "QtHelpers.h"

#include <QWidget>
#include <QMainWindow>
#include <QSettings>

static QString getKey(QWidget* widget, const QString& suffix)
{
    auto className = widget->metaObject()->className();
    return QString("%1-%2").arg(className, suffix);
}

void qtRestoreGeometry(QWidget* widget)
{
    auto key = getKey(widget, "geometry");
    QSettings settings;
    if (settings.contains(key))
        widget->restoreGeometry(settings.value(key).toByteArray());
}

void qtSaveGeometry(QWidget* widget)
{
    auto key = getKey(widget, "geometry");
    QSettings settings;
    settings.setValue(key, widget->saveGeometry());
}

void qtRestoreState(QMainWindow* window)
{
    auto key = getKey(window, "state");
    QSettings settings;
    if (settings.contains(key))
        window->restoreState(settings.value(key).toByteArray());
}

void qtSaveState(QMainWindow* window)
{
    auto key = getKey(window, "state");
    QSettings settings;
    settings.setValue(key, window->saveState());
}
