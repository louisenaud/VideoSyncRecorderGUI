#ifndef DEPTHSETTINGS_H
#define DEPTHSETTINGS_H

#include <QDialog>
#include "ui_depthsettings.h"

class Depthsettings : public QDialog
{
	Q_OBJECT

public:
	Depthsettings(QWidget *parent = 0);
	~Depthsettings();
	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();

signals:
	//signal that OK button was clicked 
	void buttonClick(const QString& txt);

private:
	Ui::Depthsettings ui;
};

#endif // DEPTHSETTINGS_H
