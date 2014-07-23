#ifndef COLORSETTINGS_H
#define COLORSETTINGS_H

#include <QDialog>
#include "ui_colorsettings.h"

class Colorsettings : public QDialog
{
	Q_OBJECT

public:
	Colorsettings(QWidget *parent = 0);
	~Colorsettings();
	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();

signals:
	//signal that OK button was clicked 
	void buttonClick(const QString& txt);

private:
	Ui::Colorsettings ui;
};

#endif // COLORSETTINGS_H
