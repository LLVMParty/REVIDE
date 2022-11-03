#include "QtHelpers.h"

#include "DockManager.h"

#include <QWidget>
#include <QMainWindow>
#include <QSettings>

static QString getKey(QWidget* widget, const QString& suffix)
{
    auto objectName = widget->objectName();
    if (objectName.isEmpty())
        objectName = widget->metaObject()->className();
    return QString("%1-%2").arg(objectName, suffix);
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

void qtRestoreState(ads::CDockManager* manager)
{
    auto key = getKey(manager, "state");
    QSettings settings;
    if (settings.contains(key))
        manager->restoreState(settings.value(key).toByteArray());
}

void qtSaveState(ads::CDockManager* manager)
{
    auto key = getKey(manager, "state");
    QSettings settings;
    settings.setValue(key, manager->saveState());
}
