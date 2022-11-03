#pragma once

class QWidget;
class QMainWindow;

namespace ads
{
class CDockManager;
}

void qtRestoreGeometry(QWidget* widget);
void qtSaveGeometry(QWidget* widget);

void qtRestoreState(QMainWindow* window);
void qtSaveState(QMainWindow* window);

void qtRestoreState(ads::CDockManager* manager);
void qtSaveState(ads::CDockManager* manager);