#include "CustomStyles.h"

CustomStyles::CustomStyles(QWidget *parent) : QWidget(parent)
{
	setVisible(false);
	ensurePolished();
}

CustomStyles &CustomStyles::instance()
{
	static CustomStyles i;
	return i;
}
