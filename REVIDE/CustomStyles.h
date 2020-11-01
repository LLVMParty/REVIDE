#ifndef CUSTOMSTYLES_H
#define CUSTOMSTYLES_H

#include <QWidget>
#include <QDebug>

class CustomStyles : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(QColor testColor MEMBER testColor /*READ getTestColor WRITE setTestColor NOTIFY testColorChanged */)

public:
	QColor getTestColor() const
	{
		qDebug() << "getTestColor";
		return testColor;
	}

	void setTestColor(QColor color)
	{
		qDebug() << "setTestColor" << color;
		testColor = color;
	}

	explicit CustomStyles(QWidget *parent = nullptr);

	static CustomStyles& instance();

	QColor testColor;

public slots:
	void testColorChanged()
	{
		qDebug() << "testColorChanged";
	}
};

#endif // CUSTOMSTYLES_H
